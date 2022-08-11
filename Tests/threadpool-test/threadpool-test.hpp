#ifndef THREADPOOLTEST_HPP
#define THREADPOOLTEST_HPP

#include <Sources/threadpool.cpp>

class ThreadPoolTest: public ThreadPool
{
public:
    void createThreadWithTask_Test(std::function<void ()> newTask);

    void attach_Test(IConnector* connector);
    void setGlobalStop_Test();
    void runningThread_Test(std::function<void()> newTask, uint16_t thread_identificator);

    IConnector* getConnector_Test() const;
    bool getState() const;
    uint16_t getThreadIdentificator() const;
    std::queue<uint16_t> getQueue() const;
    void StopPool();
};

class ConnectorTest: public IConnector
{
public:
    ConnectorTest();
    ~ConnectorTest();

    virtual void createNewThread(std::function<void()> newTask) override;
    virtual void addFileToStorage(const std::string& data) override;
    virtual void notifyStorageChangedState(const bool state) override;
    virtual void notifyGlobalStop() override;
    virtual void storageReady() override;

    void detach(IVideoCaptureManager* component) override;
    void detach(ICommandLineReader* component) override;
    void detach(IStorageManager* component) override;
    void detach(IThreadPool* component) override;

    void addComponent(IThreadPool* component);

    IThreadPool* thread_pool_ {nullptr};
    bool state_{false};
};

inline void ThreadPoolTest::createThreadWithTask_Test(std::function<void()> newTask)
{
    createThreadWithTask(newTask);
}

inline void ThreadPoolTest::attach_Test(IConnector* connector)
{
    attach(connector);
}

inline void ThreadPoolTest::setGlobalStop_Test()
{
    setGlobalStop();
}

inline void ThreadPoolTest::runningThread_Test(std::function<void()> newTask, uint16_t thread_identificator)
{
    runningThread(newTask, thread_identificator);
}

inline IConnector* ThreadPoolTest::getConnector_Test() const
{
    return connector_;
}

inline bool ThreadPoolTest::getState() const
{
    return global_stop_;
}

inline uint16_t ThreadPoolTest::getThreadIdentificator() const
{
    return thread_identificator_;
}

inline std::queue<uint16_t> ThreadPoolTest::getQueue() const
{
    return threads_to_detach_;
}

inline void ThreadPoolTest::StopPool()
{
    for (auto& thread: pool_)
        if (thread.second.joinable())
            thread.second.join();
}

inline ConnectorTest::ConnectorTest(){}

inline ConnectorTest::~ConnectorTest(){}

void ConnectorTest::createNewThread([[maybe_unused]] std::function<void ()> newTask){}

void ConnectorTest::addFileToStorage([[maybe_unused]] const std::string& data){}

void ConnectorTest::notifyStorageChangedState(bool state)
{
    state_ = state;
}

void ConnectorTest::notifyGlobalStop()
{
    if (thread_pool_ != nullptr)
        thread_pool_->setGlobalStop();
}

void ConnectorTest::storageReady(){}

void ConnectorTest::detach([[maybe_unused]]  IVideoCaptureManager* component){}

void ConnectorTest::detach([[maybe_unused]]  ICommandLineReader* component){}

void ConnectorTest::detach([[maybe_unused]] IStorageManager* component){}

void ConnectorTest::detach([[maybe_unused]]  IThreadPool* component)
{
    thread_pool_ = nullptr;
}

void ConnectorTest::addComponent(IThreadPool* component)
{
    thread_pool_ = component;
    component->attach(this);
}


#endif // THREADPOOLTEST_HPP
