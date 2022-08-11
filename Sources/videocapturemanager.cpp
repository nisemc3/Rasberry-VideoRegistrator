#include "Headers/videocapturemanager.hpp"

VideoCaptureManager::VideoCaptureManager()
{
}

VideoCaptureManager::~VideoCaptureManager()
{
}

void VideoCaptureManager::initiateVideoCaptureClients(const std::vector<VideoServer::VideoSettings>& settings)
{
    if (clients_.empty())
    {
        for (const auto& current_settings: settings)
        {
            if (connector_)
                connector_->createNewThread(std::bind(&VideoCaptureManager::createVideoCaptureClient, this, current_settings));
        }
        return;
    }

    closeUnusedClients(settings);
}

void VideoCaptureManager::detach(IObserver* observer)
{
    std::unique_lock<std::mutex> lock (mutex_list_);

    if (auto client = dynamic_cast<VideoCaptureClient*>(observer))
        clients_.remove(client);

    if (clients_.empty() && global_stop_)
        wait_for_clients_stop_.notify_one();
}

void VideoCaptureManager::closeUnusedClients(const std::vector<VideoServer::VideoSettings>& settings)
{
    std::unique_lock<std::mutex> lock (mutex_list_);
    auto settings_to_create_new_objects = settings;

    for (const auto& client: clients_)
    {
        bool compares {false};
        auto settings_to_delete = settings_to_create_new_objects.begin();
        auto max_settings = settings_to_create_new_objects.end() - 1;

        for (const auto& current_settings: settings)
        {
            if (client->getSettings() == current_settings)
            {
                compares = true;
                settings_to_create_new_objects.erase(settings_to_delete);
                break;
            }
            if (settings_to_delete < max_settings)
                ++settings_to_delete;
        }
        if (!compares)
            client->setGlobalStop();
    }

    for (const auto& current_settings: settings_to_create_new_objects)
    {
        if (connector_)
            connector_->createNewThread(std::bind(&VideoCaptureManager::createVideoCaptureClient, this, current_settings));
    }
}

void VideoCaptureManager::storageChangedState(bool state)
{
    for (auto& client: clients_)
    {
        client->storageChangedState(state);
    }
}

void VideoCaptureManager::attach(IConnector* connector)
{
    connector_ = connector;
}

void VideoCaptureManager::setGlobalStop()
{
    std::unique_lock<std::mutex> lock(mutex_list_);

    global_stop_ = true;

    for (const auto& client: clients_)
    {
        client->setGlobalStop();
    }

    while(!clients_.empty())
    {
        wait_for_clients_stop_.wait(lock, [this] {return clients_.empty();});
    }

    if (connector_)
        connector_->detach(this);

    connector_ = nullptr;
}

void VideoCaptureManager::addFileToStorage(const std::string& data)
{
    if (connector_)
        connector_->addFileToStorage(data);
}

void VideoCaptureManager::createVideoCaptureClient(const VideoServer::VideoSettings& settings)
{
    if (global_stop_)
        return;

    VideoCaptureClient client(this, settings);

    std::unique_lock<std::mutex> lock (mutex_list_);
    clients_.push_back(&client);
    lock.unlock();

    client.startClient();
}
