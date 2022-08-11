#include "Headers/threadpool.hpp"
#include <iostream>

ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::runningThread(std::function<void()> newTask, uint16_t thread_identificator)
{
    newTask();

    threads_to_detach_.push(thread_identificator);
    std::this_thread::yield();
}

void ThreadPool::attach(IConnector* connector)
{
    connector_ = connector;
}

void ThreadPool::setGlobalStop()
{
    std::unique_lock<std::mutex> lock(mutex_);

    global_stop_ = true;

    for (auto& item : pool_)
    {
        if (item.second.joinable())
            item.second.join();
    }

    if (connector_)
        connector_->detach(this);

    connector_ = nullptr;
}

void ThreadPool::createThreadWithTask(std::function<void()> newTask)
{
    std::unique_lock<std::mutex> lock(mutex_);

    if (!global_stop_)
    {
        while (!threads_to_detach_.empty())
        {
            pool_.find(threads_to_detach_.front())->second.detach();
            threads_to_detach_.pop();
        }

        pool_.emplace(thread_identificator_, std::thread(&ThreadPool::runningThread, this, newTask, thread_identificator_));
        ++thread_identificator_;
    }
}
