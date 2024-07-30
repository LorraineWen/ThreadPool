#ifndef RESULT_HEADER
#define RESULT_HEADER
#include<memory>
#include<atomic>
#include"semaphore.h"
#include"task.h"
#include"any.h"
//接收提交到线程池的task任务执行完成后的返回值类型
class Result
{
public:
	Result(std::shared_ptr<Task>task, bool isValid = true);
	~Result() = default;

private:
	Any any_;//存储任务的返回值
	Semaphore sem_;//线程通信信号量
	std::shared_ptr<Task> task_;//指向对应返回值的任务对象
	std::atomic_bool isValid_;//返回值是否有效

};

#endif RESULT_HEADER