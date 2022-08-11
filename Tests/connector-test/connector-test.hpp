#ifndef CONNECTORTEST_HPP
#define CONNECTORTEST_HPP

#include <Sources/connector.cpp>

class ConnectorTest: public Connector
{
public:
    ConnectorTest(){}
    ~ConnectorTest(){}

    IVideoCaptureManager* getVideoCaptureManager();
    ICommandLineReader* getCommandLineReader();
    IStorageManager* getStorageManager();
    IThreadPool* getThreadPool();

    void createNewThread_Test(std::function<void()> newTask);
    void addFileToStorage_Test(const std::string& data);
    void notifyStorageChangedState_Test(const bool state);
    void notifyGlobalStop_Test();
    void storageReady_Test();

    void detach_Test(IVideoCaptureManager* component);
    void detach_Test(ICommandLineReader* component);
    void detach_Test(IStorageManager* component);
    void detach_Test(IThreadPool* component);

    bool getGlobalStopState();
};

class VideoCaptureManagerTest: public IVideoCaptureManager
{
public:
    VideoCaptureManagerTest();
    ~VideoCaptureManagerTest();
    void attach(IConnector* connector) override;
    void setGlobalStop() override;
    void initiateVideoCaptureClients(const std::vector<VideoServer::VideoSettings>& settings) override;
    void storageChangedState(bool state) override;

    bool state_ {false};
    std::vector<VideoServer::VideoSettings> settings_ {};
};

class CommandLineReaderTest: public ICommandLineReader
{
public:
    void attach(IConnector* connector) override;
    void setGlobalStop() override;
};

class StorageManagerTest: public IStorageManager
{
public:
    void attach(IConnector* connector) override;
    void setGlobalStop() override;
    void addFileToStorage(const std::string& data) override;
    const std::vector<VideoServer::VideoSettings>& getSettings() override;

    std::vector<VideoServer::VideoSettings> settings_ {{"test", "test2"}};
    std::string data_{};
};

class ThreadPoolTest: public IThreadPool
{
public:
    void attach(IConnector* connector) override;
    void setGlobalStop() override;
    void createThreadWithTask(std::function<void ()> newTask) override;
};

inline VideoCaptureManagerTest::VideoCaptureManagerTest(){}

inline VideoCaptureManagerTest::~VideoCaptureManagerTest(){}

inline void VideoCaptureManagerTest::attach([[maybe_unused]] IConnector* connector){}

inline void VideoCaptureManagerTest::setGlobalStop(){}

inline void VideoCaptureManagerTest::initiateVideoCaptureClients([[maybe_unused]] const std::vector<VideoServer::VideoSettings>& settings)
{
    settings_ = settings;
}

inline void VideoCaptureManagerTest::storageChangedState([[maybe_unused]] bool state)
{
    state_ = state;
}

inline void CommandLineReaderTest::attach([[maybe_unused]] IConnector* connector){}

inline void CommandLineReaderTest::setGlobalStop(){}

inline void StorageManagerTest::attach([[maybe_unused]] IConnector* connector){}

inline void StorageManagerTest::setGlobalStop(){}

inline void StorageManagerTest::addFileToStorage([[maybe_unused]] const std::string& data)
{
    data_ = data;
}

inline const std::vector<VideoServer::VideoSettings>& StorageManagerTest::getSettings()
{
    return settings_;
}

inline void ThreadPoolTest::attach([[maybe_unused]] IConnector* connector){}

inline void ThreadPoolTest::setGlobalStop(){}

inline void ThreadPoolTest::createThreadWithTask([[maybe_unused]] std::function<void()> newTask)
{
    newTask();
}

inline IVideoCaptureManager* ConnectorTest::getVideoCaptureManager()
{
    return video_capture_manager_;
}

inline ICommandLineReader* ConnectorTest::getCommandLineReader()
{
    return command_line_reader_;
}

inline IStorageManager* ConnectorTest::getStorageManager()
{
    return storage_manager_;
}

inline IThreadPool* ConnectorTest::getThreadPool()
{
    return thread_pool_;
}

inline void ConnectorTest::createNewThread_Test(std::function<void()> newTask)
{
    createNewThread(newTask);
}

inline void ConnectorTest::addFileToStorage_Test(const std::string& data)
{
    addFileToStorage(data);
}

inline void ConnectorTest::notifyStorageChangedState_Test(const bool state)
{
    notifyStorageChangedState(state);
}

inline void ConnectorTest::notifyGlobalStop_Test()
{
    notifyGlobalStop();
}

inline void ConnectorTest::storageReady_Test()
{
    storageReady();
}

inline void ConnectorTest::detach_Test(IVideoCaptureManager* component)
{
    detach(component);
}

inline void ConnectorTest::detach_Test(ICommandLineReader* component)
{
    detach(component);
}

inline void ConnectorTest::detach_Test(IStorageManager* component)
{
    detach(component);
}

inline void ConnectorTest::detach_Test(IThreadPool* component)
{
    detach(component);
}

inline bool ConnectorTest::getGlobalStopState()
{
    return global_stop_;
}

#endif // CONNECTORTEST_HPP
