#ifndef THREADPOOL_HEADER
#define  THREADPOOL_HEADER
#include<queue>
#include<memory>
#include"task.h"
#include"thread.h"
 enum class ThreadPoolMode
{
	MODE_FIXED,
	MODE_CACHED,
};
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
	void setMode( ThreadPoolMode mode=ThreadPoolMode::MODE_FIXED);//设置线程池的模式
	void setInitialSize(size_t size=4);//设置线程池的初始线程数量
	void start();//启动线程池
	void submitTask();//向线程池中提交任务
	void ThreadFunc();//线程函数
private:
	std::queue<std::shared_ptr<Thread*>> threadQue_;//线程队列
	std::queue<std::shared_ptr<Task*>> taskQue_;//任务队列
};
#endif THREADPOOL_HEADER