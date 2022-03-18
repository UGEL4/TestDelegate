#pragma once
#include <iostream>

//one param with return value
template <typename RetValueType, typename ParamType>
class Delegate
{
public:
	Delegate()
	{
		m_pObj = nullptr;
		mFunc = nullptr;
	}
	~Delegate()
	{

	}

	typedef RetValueType(*NewFunc)(void*, ParamType);

private:
	static Delegate Create(void* p, NewFunc func)
	{
		Delegate Temp;
		Temp.m_pObj = p;
		Temp.mFunc = func;
		return Temp;
	}

	template <RetValueType(*F)(ParamType)>
	static RetValueType CommonFunc(void* p, ParamType a)
	{
		return (F)(a);
	}

	template <class ClassName, RetValueType(ClassName::*ClassFunc)(ParamType)>
	static RetValueType ClassMethod(void* p, ParamType a)
	{
		ClassName* ptr = (ClassName*)p;
		return (ptr->*ClassFunc)(a);
	}

public:
	template <class ClassName, RetValueType(ClassName::*ClassFunc)(ParamType)>
	static Delegate FromMethod(ClassName* p)
	{
		return Create(p, &ClassMethod<ClassName, ClassFunc>);
	}

	template <RetValueType(*F)(ParamType)>
	static Delegate FromFunction()
	{
		return Create(nullptr, &CommonFunc<F>);
	}

	RetValueType Execute(ParamType a) const
	{
		return (*mFunc)(m_pObj, a);
	}

private:
	void* m_pObj;
	NewFunc mFunc;
};

//no param or more then one param with/without return value
template <typename RetValueType, typename ... ParamType>
class DelegateNParam
{
public:
	DelegateNParam()
	{
		m_pObj = nullptr;
		mFunc = nullptr;
	}
	~DelegateNParam()
	{

	}

	typedef RetValueType(*NewFunc)(void*, ParamType ...);

private:
	static DelegateNParam Create(void* p, NewFunc func)
	{
		DelegateNParam Temp;
		Temp.m_pObj = p;
		Temp.mFunc = func;
		return Temp;
	}

	template <RetValueType(*F)(ParamType...)>
	static RetValueType CommonFunc(void* p, ParamType ... param)
	{
		return (F)(param...);
	}

	template <class ClassName, RetValueType(ClassName::*ClassFunc)(ParamType...)>
	static RetValueType ClassMethod(void* p, ParamType ... param)
	{
		ClassName* ptr = (ClassName*)p;
		return (ptr->*ClassFunc)(param...);
	}

public:
	template <class ClassName, RetValueType(ClassName::*ClassFunc)(ParamType...)>
	static DelegateNParam FromMethod(ClassName* p)
	{
		return Create(p, &ClassMethod<ClassName, ClassFunc>);
	}

	template <RetValueType(*F)(ParamType...)>
	static DelegateNParam FromFunction()
	{
		return Create(nullptr, &CommonFunc<F>);
	}

	RetValueType Execute(ParamType ... param) const
	{
		return (*mFunc)(m_pObj, param...);
	}

	bool operator ==(const DelegateNParam& right) const
	{
		return (m_pObj == right.m_pObj && mFunc == right.mFunc);
	}

	operator bool() const
	{
		return mFunc != nullptr;
	}

	bool operator !() const
	{
		return !(operator bool());
	}

private:
	void* m_pObj;
	NewFunc mFunc;
};

//delegate list
#include <vector>
template <typename RetValueType, typename ... ParamType>
class DelegateEvent
{
public:
	DelegateEvent()
	{
		mDelegateList.clear();
	}
	~DelegateEvent()
	{
		mDelegateList.clear();
	}

	void operator +=(const DelegateNParam<RetValueType, ParamType...>& handler)
	{
		this->Add(handler);
	}

	void Add(const DelegateNParam<RetValueType, ParamType...>& handler)
	{
		if (!Has(handler))
			mDelegateList.emplace_back(handler);
	}

	void operator -=(const DelegateNParam<RetValueType, ParamType...>& handler)
	{
		this->Remove(handler);
	}

	void Remove(const DelegateNParam<RetValueType, ParamType...>& handler)
	{
		for (size_t i = 0; i < mDelegateList.size(); i++)
		{
			if (handler == mDelegateList[i])
			{
				mDelegateList.erase(mDelegateList.begin() + i);
				return;
			}
		}
	}

	bool Has(const DelegateNParam<RetValueType, ParamType...>& handler)
	{
		for (size_t i = 0; i < mDelegateList.size(); i++)
		{
			if (handler == mDelegateList[i])
				return true;
		}
		return false;
	}

	bool IsVaild() const
	{
		return mDelegateList.size() > 0;
	}

	void Reset()
	{
		mDelegateList.clear();
	}

	void Execute(ParamType ... param) const
	{
		for (auto& delegate : mDelegateList)
		{
			delegate.Execute(param...);
		}
	}
private:
	std::vector<DelegateNParam<RetValueType, ParamType...>> mDelegateList;
};
