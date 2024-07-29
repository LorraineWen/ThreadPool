#ifndef THREADPOOL_HEADER
#define  THREADPOOL_HEADER
#include<queue>
#include<memory>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<functional>
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
	void setMode( ThreadPoolMode mode);//设置线程池的模式'
	void setTaskQueMaxThreshHold(size_t threshhold);//设置task任务队列上线阈值
	void start(size_t initThreadSize = 4);//启动线程池
	void submitTask(std::shared_ptr<Task> task);//向线程池中提交任务
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
private:
	std::vector<std::unique_ptr<Thread>>threadQue_;//线程列表
	size_t initThreadSize_;//初始的线程数量

	std::queue<std::shared_ptr<Task>> taskQue_;//任务队列
	std::atomic_int taskSize_;//任务的数量，原子类型
	size_t taskQueMaxThreshHold_;//任务队列数量上线阈值

	std::mutex taskQueMtx_;//保证任务队列的线程安全
	std::condition_variable notFull_;//表示任务队列不满
	std::condition_variable notEmpty_;//表示任务队列不空

	ThreadPoolMode poolMode_;//当前线程池的工作模式

	void threadFunc();//线程函数
};
#endif THREADPOOL_HEADER