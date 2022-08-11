#include "Headers/connector.hpp"

#include <chrono>
#include <thread>

Connector::Connector()
{
}

Connector::~Connector()
{
}

void Connector::addComponent(IVideoCaptureManager* component)
{
    video_capture_manager_ = component;
    video_capture_manager_->attach(this);
}

void Connector::addComponent(ICommandLineReader* component)
{
    command_line_reader_ = component;
    command_line_reader_->attach(this);
}

void Connector::addComponent(IStorageManager* component)
{
    storage_manager_ = component;
    storage_manager_->attach(this);
}

void Connector::addComponent(IThreadPool* component)
{
    thread_pool_ = component;
    thread_pool_->attach(this);
}

void Connector::notifyStorageChangedState(const bool state)
{
    if (video_capture_manager_ )
        video_capture_manager_->storageChangedState(state);
}

void Connector::notifyGlobalStop()
{
    std::unique_lock<std::mutex> lock(mutex_global_stop_);

    if (video_capture_manager_ )
        video_capture_manager_->setGlobalStop();

    if (storage_manager_ )
        storage_manager_->setGlobalStop();

    global_stop_ = true;

    condition_global_stop_.notify_one();
}

void Connector::storageReady()
{
    if (video_capture_manager_ != nullptr && storage_manager_ )
        video_capture_manager_->initiateVideoCaptureClients(storage_manager_->getSettings());
}

void Connector::detach([[maybe_unused]]IVideoCaptureManager* component)
{
    video_capture_manager_ = nullptr;
    condition_global_stop_.notify_one();
}

void Connector::detach([[maybe_unused]] ICommandLineReader* component)
{
    command_line_reader_ = nullptr;
    condition_global_stop_.notify_one();
}

void Connector::detach([[maybe_unused]] IStorageManager* component)
{
    storage_manager_ = nullptr;
    condition_global_stop_.notify_one();
}

void Connector::detach([[maybe_unused]] IThreadPool* component)
{
    thread_pool_ = nullptr;
}

void Connector::addFileToStorage(const std::string& data)
{
    if (storage_manager_ )
        storage_manager_->addFileToStorage(data);
}

void Connector::waitForCorrectGlobalStop()
{
    std::unique_lock<std::mutex> lock(mutex_global_stop_);
    condition_global_stop_.wait(lock, [this] {return global_stop_;});

    while (true)
    {
        if (thread_pool_ )
            thread_pool_->setGlobalStop();

        if ((storage_manager_ == nullptr) && (video_capture_manager_ == nullptr) && (command_line_reader_ == nullptr))
            break;

        condition_global_stop_.wait(lock);
    }

}

void Connector::createNewThread(std::function<void()> newTask)
{
    if (thread_pool_ )
        thread_pool_->createThreadWithTask(newTask);
}
