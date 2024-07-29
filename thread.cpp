#include"thread.h"
#include<thread>
Thread::Thread(threadfunc func):func_(func)
{

}
Thread::~Thread()
{

}
void Thread:: start()
{
	std::thread t(func_);
	//离开该函数作用域，线程对象就析构了，但是线程函数不能析构，因为后面还要用，所以要将线程对象和线程函数分离开来
	t.detach();

}