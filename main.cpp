#include <iostream>
#include<chrono>
#include"threadpool.h"
class task1:public Task
{
	void run()
	{
		std::cout << std::this_thread::get_id() <<"start"<< std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(6));
		std::cout << std::this_thread::get_id() << "end"<<std::endl;
	}
};
int main()
{
	ThreadPool threadpool;
	threadpool.start(3);
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	threadpool.submitTask(std::make_shared<task1>());
	getchar();
	return 0;
}