#include "thread.h"

ThreadPool::ThreadPool(int numThreads)
	: stop(false)
{
	for (int i = 0; i < numThreads; i++)
	{
		//thread不支持拷贝和移动构造，只能用emplace_back
		threads.emplace_back([this] {
			while (1)
			{
				std::unique_lock<std::mutex> lock(mtx);
				condition.wait(lock, [this] {
					return !tasks.empty() || stop;
					});

				if (stop && tasks.empty())
					return;

				std::function<void()> task(std::move(tasks.front()));
				tasks.pop();
				lock.unlock();

				task();
			}
			});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		stop = true;
	}

	condition.notify_all();
	for (auto& t : threads) //不能拷贝只能引用
	{
		t.join();
	}
}
