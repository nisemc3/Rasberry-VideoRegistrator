#ifndef IO_H
#define IO_H

#include "Headers/functions.hpp"

#include <functional>
#include <memory>
#include <string>

class IConnector;

class BaseComponent
{
public:
    virtual ~BaseComponent(){}

    virtual void attach(IConnector* connector) = 0;
    virtual void setGlobalStop() = 0;

protected:
    IConnector* connector_ {nullptr};
    bool global_stop_ {false};
};

class IStorageManager: public BaseComponent
{
public:
    virtual ~IStorageManager(){}
    virtual void addFileToStorage(const std::string& data) = 0;
    virtual const std::vector<VideoServer::VideoSettings>& getSettings() = 0;
};

class IVideoCaptureManager: public BaseComponent
{
public:
    virtual ~IVideoCaptureManager(){}

    virtual void initiateVideoCaptureClients(const std::vector<VideoServer::VideoSettings>& settings) = 0;
    virtual void storageChangedState(bool state) = 0;
};

class ICommandLineReader: public BaseComponent
{
public:
    virtual ~ICommandLineReader(){}
};

class IThreadPool: public BaseComponent
{
public:
    virtual ~IThreadPool(){}
    virtual void createThreadWithTask(std::function<void()> newTask) = 0;
};

class IConnector
{
public:
    virtual ~IConnector(){}

    virtual void createNewThread(std::function<void()> newTask) = 0;
    virtual void addFileToStorage(const std::string& data) = 0;
    virtual void notifyStorageChangedState(const bool state) = 0;
    virtual void notifyGlobalStop() = 0;
    virtual void storageReady() = 0;

    virtual void detach(IVideoCaptureManager* component) = 0;
    virtual void detach(ICommandLineReader* component) = 0;
    virtual void detach(IStorageManager* component) = 0;
    virtual void detach(IThreadPool* component) = 0;
};

class IObserver
{
public:
    virtual ~IObserver(){}

    virtual void setGlobalStop() = 0;
    virtual void storageChangedState(bool state) = 0;
};

class ISubject
{
public:
    virtual ~ISubject(){}

    virtual void addFileToStorage(const std::string& data) = 0;
    virtual void detach(IObserver* observer) = 0;
};

#endif // IO_H
