#include "Headers/storagemanager.hpp"

#include <filesystem>
#include <iostream>
#include <thread>

void StorageManager::addFileToStorage(const std::string& file_path)
{
    std::unique_lock<std::mutex> lock (mutex_);
    storaged_files_.emplace(getFileChangeTime(file_path), file_path);
}

void StorageManager::setGlobalStop()
{
    global_stop_ = true;
}

void StorageManager::attach(IConnector* connector)
{
    connector_ = connector;
}

void StorageManager::notifyStorageChangedState(bool state)
{
    std::unique_lock<std::mutex> lock (mutex_);
    storage_opened_success_ = state;

    if (connector_)
        connector_->notifyStorageChangedState(state);
}

void StorageManager::controlingLoop()
{
    while (true)
    {
        std::filesystem::directory_entry(path_to_storage_, error_code_);
        notifyStorageChangedState(!error_code_.value());
        error_code_.clear();

        if (!storage_opened_success_ || global_stop_)
            break;

        checkForStorageSpace();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void StorageManager::openStorage()
{
#ifdef COMPILE_ARM
    path_to_storage_ = "/media/" + std::string(getlogin()) + "/";
    while (!global_stop_)
    {
        std::filesystem::directory_iterator path_to_media(path_to_storage_);
        if (std::filesystem::begin(path_to_media) == std::filesystem::end(path_to_media))
        {
            std::cerr << "ERROR! Insert usb storage" << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            continue;
        }
        path_to_storage_ = std::filesystem::begin(path_to_media)->path();
        uintmax_t storage_free_space = std::filesystem::space(path_to_storage_).available * convert_number_ * convert_number_;
        if (storage_free_space <= minimal_free_space_)
        {
            std::cerr << "NOT ENOUGHT SPACE! Insert another usb storage " << path_to_storage_ << '\n';
            continue;
        }
        break;
    }
    path_to_settings_ = path_to_storage_ + settings_name_;
#else
    std::filesystem::directory_entry storage(path_to_storage_);
    while(!storage.exists() && !global_stop_)
    {
        std::cerr << "ERROR! Unable to open storage  " << path_to_storage_ << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        storage.refresh();
    }
#endif
}

void StorageManager::openSettings()
{
    video_settings_file_.clear();

    std::string name_of_folder {};
    VideoServer::url url_of_camera {};
    VideoServer::path path_to_save {};

    file_opener_.open(path_to_settings_);
    if (!file_opener_.is_open())
    {
        std::cerr << "ERROR! Unable to open settings file " << path_to_settings_ << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        settings_opened_success_ = false;
        return;
    }

    while (std::getline(file_opener_, name_of_folder))
    {
        if (std::getline(file_opener_, url_of_camera))
        {
            path_to_save = path_to_storage_ + name_of_folder + "/";
            video_settings_file_.push_back({path_to_save, url_of_camera});
        }
    }

    if (file_opener_.is_open())
    {
        minimal_free_space_ = video_settings_file_.size() * video_size_;
        file_opener_.close();
    }

    settings_opened_success_ = true;
}

void StorageManager::parseStorage()
{
    std::filesystem::recursive_directory_iterator storage(path_to_storage_);
    for (const auto& object: storage)
    {
        if (!object.is_directory())
        {
            if (isReserved(object.path().filename()))
                continue;
            storaged_files_.emplace(getFileChangeTime(object.path()), object.path());
        }
    }
}

uint64_t StorageManager::getFileChangeTime(const std::string& file_path)
{
     return static_cast<uint64_t>(std::filesystem::last_write_time(file_path).time_since_epoch().count());
}

bool StorageManager::isReserved(const std::string& name_to_compare)
{
    for (const auto& name: reserved_names_)
    {
        if (!name_to_compare.compare(name))
            return true;
    }
    return false;
}

void StorageManager::createFolders()
{
    for (const auto& folder: video_settings_file_)
    {
        std::filesystem::create_directories(folder.first);
    }
}

void StorageManager::checkForStorageSpace()
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = end - parse_timer_start_;

    if (elapsed_seconds.count() >= parse_lifetime_ )
    {
        std::unique_lock<std::mutex> lock (mutex_);
        storaged_files_.clear();
        parseStorage();
        parse_timer_start_ = std::chrono::system_clock::now();
    }

    if (!storaged_files_.empty())
        while ((getAvalibleStorageSpace() <= minimal_free_space_) && !storaged_files_.empty())
        {
            std::unique_lock<std::mutex> lock (mutex_);
            std::filesystem::remove(storaged_files_.begin()->second);
            storaged_files_.erase(storaged_files_.begin());
        }
}

void StorageManager::storageControlLoop()
{
    while (!storage_opened_success_ && !global_stop_)
    {
        openStorage();
        openSettings();

        if (settings_opened_success_)
        {
            parseStorage();
            createFolders();
            storage_opened_success_ = true;

            if (connector_)
                connector_->storageReady();

            controlingLoop();
        }
    }

    if (connector_)
        connector_->detach(this);

    connector_ = nullptr;
}

const std::vector<VideoServer::VideoSettings>& StorageManager::getSettings()
{
    return video_settings_file_;
}

void StorageManager::initiateMembers()
{
    reserved_names_.clear();
    storaged_files_.clear();
    video_settings_file_.clear();

    ready_ = false;
    storage_opened_success_ = false;
    settings_opened_success_ = false;
    convert_number_ = 1.0f / 1024.0f;
    video_size_ = 500;
    settings_name_ = "settings.txt";
    name_of_storage_ = "T7/";
    path_to_storage_ = "/media/" + std::string(getlogin()) + "/" + name_of_storage_;
    path_to_settings_ = path_to_storage_ + settings_name_;
    parse_timer_start_ = std::chrono::system_clock::now();
    parse_lifetime_ = 86400.0f; // 24 часа.

    reserved_names_.push_back(settings_name_);
}

uint64_t StorageManager::getAvalibleStorageSpace()
{
    return static_cast<uint64_t>(static_cast<float>(std::filesystem::space(path_to_storage_).available) * convert_number_ * convert_number_);
}

StorageManager::StorageManager()
{
    initiateMembers();
}

StorageManager::~StorageManager()
{
}
