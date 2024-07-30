#ifndef ANY_HEADER
#define ANY_HEADER
#include<memory>
#include<iostream>
//可以接收任意数据的类型
class Any
{
public:
	Any() = default;//将构造函数和析构函数定义为C++的默认实现，可以让编译器在编译C++代码的时候做出更多的优化
	//其实就是代替 Any(){}
	~Any() = default;

	Any(const Any&) = delete;//禁用Any的拷贝构造函数
	Any& operator=(const Any&) = delete;//禁用了拷贝赋值运算符
	Any(Any&&) = default;//声明移动构造函数，使用default关键字将其定义为默认实现，编译器会自动生成移动构造函数
	Any& operator=(Any&&) = default;//声明移动赋值运算符，并定义为默认实现，编译器会自动生成移动赋值运算符

	//这个构造函数可以让Any类型接收任意其他的数据
	template<typename T> Any(T data):base_(std::make_unique<Derive<T>>(data)){}
	template<typename T> T cast_()
	{
		Derive<T>* pd = dynamic_cast<Derive<T*>>(base_.get());
		if (pd == nullptr)
		{
			throw "type is incompatible"
		}
		return pd->data_;
	}
private:
	class Base
	{
	public:
		virtual ~Base() = default;
	};
	template<typename T> class Derive :public Base
	{
	public:
		Derive(T data) :data_(data) {}
		T data_;
	};

	//定义一个基类的指针
	std::unique_ptr<Base> base_;
};
#endif ANY_HEADER