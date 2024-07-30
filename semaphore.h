#ifndef SEMAPHORE_HEADER
#define SEMAPHORE_HEADER
#include<mutex>
#include<condition_variable>
/*
* 由于执行task的线程和提交task的线程不是同一个线程，当提交task的线程需要获取task执行完毕后的返回值时，执行task的线程在执行完task后
* 需要通知提交task的任务停止阻塞，可以拿到返回值了,这就需要进行线程间的通信，方式可以有两种，一种是通过条件变量，一种是通过信号量，由于这个
* 通信相当简单所以可以用信号量来完成。
*/
class Semaphore
{
public:
	Semaphore(size_t size=0):initsize_(size){}
	//增加信号量
	void post()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		initsize_++;
		//通知其他线程可以获取信号量了
		con_.notify_all();
	}
	//获取信号量
	void wait()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		//等待信号量有资源数，没有资源数就阻塞线程
		con_.wait(lock, [&]()->bool {return initsize_ > 0; });
		initsize_--;
	}

private:
	size_t initsize_;
	std::mutex mtx_;
	std::condition_variable con_;
};
#endif SEMAPHORE_HEADER