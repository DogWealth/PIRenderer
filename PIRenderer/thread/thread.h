#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <functional>

class ThreadPool
{
public:
	ThreadPool(int numThreads);
	~ThreadPool();

	template<class F, class... Args>
	void Enqueue(F&& f, Args&&... args);//函数模板里&&是万能引用

	bool Empty() { return tasks.empty(); }
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;

	std::mutex mtx;
	std::condition_variable condition;

	bool stop;
};

template<class F, class ...Args>
inline void ThreadPool::Enqueue(F&& f, Args && ...args)
{
	std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	{
		std::unique_lock<std::mutex> lock(mtx);
		tasks.emplace(std::move(task));
	}
	condition.notify_one();
}
