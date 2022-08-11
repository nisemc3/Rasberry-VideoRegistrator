#ifndef StorageManager_TEST_H
#define StorageManager_TEST_H

#include <Sources/storagemanager.cpp>

class StorageManagerTest: public StorageManager
{
public:
    StorageManagerTest(){}
    ~StorageManagerTest(){}

    void checkForStorageSpace_Test();
    void openSettings_Test();
    void parseStorage_Test();
    void createFolders_Test();
    void addFileToStorage_Test(const std::string& file_path);
    void openStorage_Test();
    void controlingLoop_Test();

    void setPathToStorage_Test(const std::string& path);
    void setPathToSettings_Test(const std::string& path);
    void setMinimalFreeSpace_Test(uint64_t space);
    void setStoragedFiles_Test(const std::multimap<uint64_t, std::string>& storaged_files);
    void setReservedNames_Test(const std::vector<std::string>& reserved_names);
    void setFoldersToSave_Test(const std::vector<std::string>& folders);
    void setVideoSettingsFile_Test(const std::vector<VideoServer::VideoSettings>& file);
    void setParseLifetime_Test(float lifetime);

    auto getStoragedFiles_test() const;
    auto getFoldersToSave_Test() const;
    auto getSourceUrl_Test() const;
    auto getSettingsOpenedSuccess_Test() const;
    auto getFileChangeTime_Test(const std::string& file_path);
    auto getGlobalStop_Test() const;
    auto getConnector_Test() const;
    auto getVideoSettingsFile_Test();
};

class ConnectorTest: public IConnector
{
public:
    ConnectorTest(){}
    ~ConnectorTest(){}

    virtual void createNewThread(std::function<void()> newTask) override;
    virtual void addFileToStorage(const std::string& data) override;
    virtual void notifyStorageChangedState(const bool state) override;
    virtual void notifyGlobalStop() override;
    virtual void storageReady() override;

    void detach(IVideoCaptureManager* component) override;
    void detach(ICommandLineReader* component) override;
    void detach(IStorageManager* component) override;
    void detach(IThreadPool* component) override;

    void addComponent(IStorageManager* component);

    IStorageManager* storage_manager_ {nullptr};
    bool state_{true};
};

void ConnectorTest::createNewThread([[maybe_unused]] std::function<void ()> newTask){}

void ConnectorTest::addFileToStorage([[maybe_unused]] const std::string& data){}

void ConnectorTest::notifyStorageChangedState(bool state)
{
    state_ = state;
}

void ConnectorTest::notifyGlobalStop()
{
    if (storage_manager_ != nullptr)
        storage_manager_->setGlobalStop();
}

void ConnectorTest::storageReady(){}

void ConnectorTest::detach([[maybe_unused]]  IVideoCaptureManager* component){}

void ConnectorTest::detach([[maybe_unused]]  ICommandLineReader* component){}

void ConnectorTest::detach([[maybe_unused]] IStorageManager* component)
{
    storage_manager_ = nullptr;
}

void ConnectorTest::detach([[maybe_unused]]  IThreadPool* component){}

void ConnectorTest::addComponent(IStorageManager* component)
{
    storage_manager_ = component;
    component->attach(this);
}

void StorageManagerTest::checkForStorageSpace_Test()
{
    checkForStorageSpace();
}

void StorageManagerTest::openSettings_Test()
{
    openSettings();
}

void StorageManagerTest::setPathToStorage_Test(const std::string& path)
{
    path_to_storage_ = path;
}

void StorageManagerTest::setPathToSettings_Test(const std::string& path)
{
    path_to_settings_ = path;
}

void StorageManagerTest::setMinimalFreeSpace_Test(uint64_t space)
{
    minimal_free_space_ = space;
}

void StorageManagerTest::setStoragedFiles_Test(const std::multimap<uint64_t, std::string>& storaged_files)
{
    storaged_files_ = storaged_files;
}

void StorageManagerTest::setReservedNames_Test(const std::vector<std::string>& reserved_names)
{
    reserved_names_ = reserved_names;
}

void StorageManagerTest::setFoldersToSave_Test(const std::vector<std::string>& folders)
{
    video_settings_file_.clear();

    for (const auto& folder: folders)
    {
        video_settings_file_.push_back({folder, std::string()});
    }
}

void StorageManagerTest::setVideoSettingsFile_Test(const std::vector<VideoServer::VideoSettings>& file)
{
    video_settings_file_ = file;
}

void StorageManagerTest::setParseLifetime_Test(float lifetime)
{
    parse_lifetime_ = lifetime;
}

void StorageManagerTest::parseStorage_Test()
{
    parseStorage();
}

void StorageManagerTest::createFolders_Test()
{
    createFolders();
}

void StorageManagerTest::addFileToStorage_Test(const std::string& file_path)
{
    addFileToStorage(file_path);
}

void StorageManagerTest::openStorage_Test()
{
    openStorage();
}

void StorageManagerTest::controlingLoop_Test()
{
    controlingLoop();
}

auto StorageManagerTest::getStoragedFiles_test() const
{
    return storaged_files_;
}

auto StorageManagerTest::getFoldersToSave_Test() const
{
    return video_settings_file_.empty() ? std::string{} : video_settings_file_[0].first;
}

auto StorageManagerTest::getSourceUrl_Test() const
{
    return video_settings_file_.empty() ? std::string{} : video_settings_file_[0].second;
}

auto StorageManagerTest::getSettingsOpenedSuccess_Test() const
{
    return settings_opened_success_;
}

auto StorageManagerTest::getFileChangeTime_Test(const std::string& file_path)
{
    return getFileChangeTime(file_path);
}

auto StorageManagerTest::getVideoSettingsFile_Test()
{
    return getSettings();
}

auto StorageManagerTest::getGlobalStop_Test() const
{
    return global_stop_;
}

auto StorageManagerTest::getConnector_Test() const
{
    return connector_;
}

#endif // StorageManager_TEST_H
