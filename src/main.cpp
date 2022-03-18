#include <iostream>
#include "Delegate.h"

bool PrintValue1(int val)
{
	std::cout << "Common function PrintValue1: " << val << std::endl;
	return true;
}

int PrintValue2(int val, int val1)
{
	std::cout << "Common function PrintValue2: " << val << ", " << val1 << std::endl;
	return val1;
}

void PrintValue3(int val, int val1, double val2)
{
	std::cout << "Common function PrintValue3: " << val << ", " << val1 << ", " << val2 << std::endl;
}

class A
{
public:
	A()
	{

	}
	~A()
	{

	}

	void PrintValueDouble(double val)
	{
		std::cout << "A::PrintValueDouble: " << val << std::endl;
	}

	bool PrintValueInt(int val)
	{
		std::cout << "A::PrintValueInt: " << val << std::endl;
		return false;
	}

	void PrintValue0()
	{
		std::cout << "A::PrintValue0(void): " << std::endl;
	}
};

int main()
{
	Delegate<bool, int> d = Delegate<bool, int>::FromFunction<&PrintValue1>();
	bool ret = d.Execute(10);
	std::cout << "d Ret :" << ret << std::endl;

	DelegateNParam<bool, int> n1 = DelegateNParam<bool, int>::FromFunction<&PrintValue1>();
	ret = n1.Execute(10);
	std::cout << "n1 Ret :" << ret << std::endl;

	DelegateNParam<int, int, int> n2 = DelegateNParam<int, int, int>::FromFunction<&PrintValue2>();
	std::cout << "n2 Ret :" << n2.Execute(20, 30) << std::endl;

	DelegateNParam<void, int, int, double> n3 = DelegateNParam<void, int, int, double>::FromFunction<&PrintValue3>();
	n3.Execute(40, 50, 0.001);

	A a;
	Delegate<void, double> ad = Delegate<void, double>::FromMethod<A, &A::PrintValueDouble>(&a);
	ad.Execute(3.1415926);

	DelegateNParam<void> n4 = DelegateNParam<void>::FromMethod<A, &A::PrintValue0>(&a);
	n4.Execute();

	DelegateNParam<bool, int> n5 = DelegateNParam<bool, int>::FromMethod<A, &A::PrintValueInt>(&a);

	//-----------------------
	DelegateEvent<bool, int> event;
	event += n1;
	event += n5;
	std::cout << "DelegateEvent::Execute :" << std::endl;
	event.Execute(888);

	std::cin.get();
	return 0;
}
