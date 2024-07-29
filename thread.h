#ifndef THREAD_HEADER
#define THREAD_HEADER
#include<functional>
class Thread
{
public:
	using threadfunc = std::function<void()>;
	Thread(threadfunc func);
	~Thread();
	void start();//启动线程
private:
	threadfunc func_;
};
#endif THREAD_HEADER