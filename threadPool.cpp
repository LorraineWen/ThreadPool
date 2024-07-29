#include"threadpool.h"
#include<iostream>
const int TASK_MAX_THRESHHOLD = 1024;
ThreadPool::ThreadPool():initThreadSize_(4),taskSize_(0), taskQueMaxThreshHold_(TASK_MAX_THRESHHOLD),poolMode_(ThreadPoolMode::MODE_FIXED)
{

}
ThreadPool::~ThreadPool()
{


}
void ThreadPool::setMode(ThreadPoolMode mode)
{
	poolMode_ = mode;
}
void ThreadPool::setTaskQueMaxThreshHold(size_t maxthreshold)
{
	taskQueMaxThreshHold_ = maxthreshold;
}
void ThreadPool::start(size_t initThreadSize)
{
	initThreadSize_ = initThreadSize;
	//创建线程
	for (int i = 0; i < initThreadSize_; i++)
	{
		std::unique_ptr<Thread> ptr =  std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
		threadQue_.emplace_back(std::move(ptr));
	}
	//启动线程

	for (int i = 0; i < initThreadSize_; i++)
	{
		threadQue_[i]->start();
	}
}
//给线程池提交任务，用户调用该接口，传入任务对象，生产任务
void ThreadPool::submitTask(std::shared_ptr<Task> task)
{
	//拿到锁
	std::unique_lock<std::mutex> lock(taskQueMtx_);

	//线程间通过条件变量进行通信，等待任务队列有空余
	//用户提价任务，最长阻塞时间不能超过1s，否则提交失败
	if(!notFull_.wait_for(lock, std::chrono::seconds(1),[&]()->bool {return taskQue_.size() < taskQueMaxThreshHold_; }))
		{
			std::cerr << "taskque is full, task submits fail" << std::endl;
			return;
		}

	//任务队列有空余，将传入的任务加入队列
	taskQue_.emplace(task);
	taskSize_++;

	//放置了新的任务，所以队列不为空了，用条件变量notEmpty发送通知，分配线程执行任务
	notEmpty_.notify_all();
}
//线程函数，线程池的所有线程从任务队列中获取任务来执行
void ThreadPool::threadFunc()
{
	std::shared_ptr<Task> sp;
	for (;;)
	{
		{
			//获取锁
			std::unique_lock<std::mutex> lock(taskQueMtx_);
			std::cout << std::this_thread::get_id() << "尝试获取任务" << std::endl;
			//等待notEmpty条件
			notEmpty_.wait(lock, [&]()->bool {return taskSize_ > 0; });

			//从任务队列中获取一个任务
			sp = taskQue_.front();
			taskQue_.pop();
			taskSize_--;
			std::cout << std::this_thread::get_id() << "获取任务成功" << std::endl;
			//还有任务，通知其他线程执行剩余任务
			if (taskSize_ > 0)
			{
				notEmpty_.notify_all();
			}
			//取出一个任务之后就可以通知继续加入任务了
			notFull_.notify_all();
		}
		//制造这个作用域的作用就是当局部变量lock离开该作用域后，这把锁就会自动释放掉，而此时当前线程已经获取到了一个任务了，其他线程也可以第一时间拿到锁
		//获取任务，而不用等待当前线程获取完任务还要执行完这个任务之后才能拿到锁
		if (sp != nullptr)
		{
			sp->run();
		}

	}
	
}