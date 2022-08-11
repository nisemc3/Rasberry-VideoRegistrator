#define BOOST_TEST_MODULE storagemanager
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp> // GCOV_EXCL_LINE
#include <fstream>  // GCOV_EXCL_LINE
#include <iostream> // GCOV_EXCL_LINE
#include <filesystem>   // GCOV_EXCL_LINE
#include <algorithm>    // GCOV_EXCL_LINE

#include "storagemanager-test.hpp"

BOOST_AUTO_TEST_CASE(file_change_time)
{
    std::string test_file_path = "/home/" + std::string(getlogin()) + "/test_file_change_time.txt";
    std::ofstream create_test_file (test_file_path);
    BOOST_CHECK_MESSAGE(create_test_file.is_open(), "ERROR! Cannot create test file " + test_file_path);
    create_test_file.close();

    StorageManagerTest storage_manager_test;
    auto time = std::filesystem::last_write_time(test_file_path).time_since_epoch().count();
    auto time_check = storage_manager_test.getFileChangeTime_Test(test_file_path);
    BOOST_CHECK(static_cast<uint64_t>(time) == time_check);

    std::error_code error_on_remove;
    std::filesystem::remove(test_file_path, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test file " + test_file_path);
    error_on_remove.clear();
}

BOOST_AUTO_TEST_CASE(open_settings_file)
{
    std::string test_path_to_settings = "/home/" + std::string(getlogin()) + "/test_open_settings_file.txt";
    std::string check_folder_name {"test/test/test"};
    std::string check_url {"http://test"};

    std::ofstream create_test_file (test_path_to_settings);
    BOOST_CHECK_MESSAGE(create_test_file.is_open(), "ERROR! Cannot create test file " + test_path_to_settings);
    create_test_file << check_folder_name << std::endl << check_url << std::endl;
    create_test_file.close();

    StorageManagerTest storage_manager_test;
    std::string temp_path{"empty/"};
    storage_manager_test.setPathToStorage_Test(temp_path);
    storage_manager_test.setPathToSettings_Test(test_path_to_settings);
    storage_manager_test.openSettings_Test();
    check_folder_name = temp_path + check_folder_name + "/";
    BOOST_CHECK(!check_folder_name.compare(storage_manager_test.getFoldersToSave_Test()));
    BOOST_CHECK(!check_url.compare(storage_manager_test.getSourceUrl_Test()));

    std::error_code error_on_remove;
    std::filesystem::remove(test_path_to_settings, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test file " + test_path_to_settings);
    error_on_remove.clear();

    storage_manager_test.openSettings_Test();
    BOOST_CHECK(!storage_manager_test.getSettingsOpenedSuccess_Test());
}

BOOST_AUTO_TEST_CASE(parse_storage)
{
    std::string test_folder = "/home/" + std::string(getlogin()) + "/test_parse_storage/";
    std::error_code error_on_create_folder;
    std::filesystem::create_directory(test_folder, error_on_create_folder);
    BOOST_CHECK_MESSAGE(!error_on_create_folder, "ERROR! Cannot create test folder " + test_folder);
    error_on_create_folder.clear();

    std::string temp_filename = test_folder + "base1.txt";
    std::ofstream test_file (temp_filename);
    BOOST_CHECK_MESSAGE(test_file.is_open(), "ERROR! Cannot create test file " + temp_filename);
    test_file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // задержка для того, чтобы время создания файлов точно отличалось
    temp_filename = test_folder + "base2.txt";
    test_file.open(temp_filename);
    BOOST_CHECK_MESSAGE(test_file.is_open(), "ERROR! Cannot create test file " + temp_filename);
    test_file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::string test_folder_2 = test_folder + "a/";
    std::error_code error_on_create_folder_2;
    std::filesystem::create_directory(test_folder_2, error_on_create_folder_2);
    BOOST_CHECK_MESSAGE(!error_on_create_folder_2, "ERROR! Cannot create test folder " + test_folder_2);
    error_on_create_folder_2.clear();

    temp_filename = test_folder_2 + "a1.txt";
    test_file.open(temp_filename);
    BOOST_CHECK_MESSAGE(test_file.is_open(), "ERROR! Cannot create test file " + temp_filename);
    test_file.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    temp_filename = test_folder_2 + "a2.txt";
    test_file.open(temp_filename);
    BOOST_CHECK_MESSAGE(test_file.is_open(), "ERROR! Cannot create test file " + temp_filename);
    test_file.close();

    StorageManagerTest storage_manager_test;
    storage_manager_test.setReservedNames_Test({"a1.txt"});
    storage_manager_test.setPathToStorage_Test(test_folder);
    storage_manager_test.parseStorage_Test();

    std::multimap<uint64_t, std::string> control
    {
        {storage_manager_test.getFileChangeTime_Test(std::string(test_folder + "a/a2.txt")), std::string(test_folder + "a/a2.txt")},
        {storage_manager_test.getFileChangeTime_Test(std::string(test_folder + "base1.txt")), std::string(test_folder + "base1.txt")},
        {storage_manager_test.getFileChangeTime_Test(std::string(test_folder + "base2.txt")), std::string(test_folder + "base2.txt")}
    };
    uint8_t compare {};
    std::multimap<uint64_t, std::string>::iterator control_it = control.begin();
    for (const auto& testing: storage_manager_test.getStoragedFiles_test())
    {
        if ((testing.first == control_it->first) && (!testing.second.compare(control_it->second)))
             compare++;
        control_it++;
    }
    BOOST_CHECK(compare == static_cast<uint8_t>(control.size()));

    std::error_code error_on_remove;
    std::filesystem::remove_all(test_folder, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test files on path " + test_folder);
}

BOOST_AUTO_TEST_CASE(check_for_storage_space)
{
    std::string test_folder = "/home/" + std::string(getlogin()) + "/test_check_for_storage_space/";
    std::error_code error_on_create_folder;
    std::filesystem::create_directory(test_folder, error_on_create_folder);
    BOOST_CHECK_MESSAGE(!error_on_create_folder, "ERROR! Cannot create test folder " + test_folder);

    std::string test_file = test_folder + "test_check_for_storage_space.txt";
    std::ofstream test_file_create (test_file);
    BOOST_CHECK_MESSAGE(test_file_create.is_open(), "ERROR! Cannot create test file " + test_file);
    test_file_create.close();

    StorageManagerTest storage_manager_test;
    storage_manager_test.setMinimalFreeSpace_Test(999999999);
    std::multimap<uint64_t, std::string> storaged_files
    {
        {storage_manager_test.getFileChangeTime_Test(test_file), test_file}
    };
    storage_manager_test.setStoragedFiles_Test(storaged_files);
    storage_manager_test.checkForStorageSpace_Test();
    BOOST_CHECK(storage_manager_test.getStoragedFiles_test().empty());
    storaged_files.clear();

    std::error_code file_error;
    std::filesystem::is_directory(test_file, file_error);
    BOOST_CHECK_MESSAGE(file_error, "File error " + file_error.message());
    file_error.clear();

    test_file_create.open(test_file);
    BOOST_CHECK_MESSAGE(test_file_create.is_open(), "ERROR! Cannot create test file " + test_file);
    test_file_create.close();
    storaged_files.emplace(storage_manager_test.getFileChangeTime_Test(test_file), test_file);

    storage_manager_test.setMinimalFreeSpace_Test(0);
    storage_manager_test.setParseLifetime_Test(0.0f);
    storage_manager_test.setPathToStorage_Test(test_folder);
    storage_manager_test.checkForStorageSpace_Test();

    uint8_t compare {};
    std::multimap<uint64_t, std::string>::iterator control_it = storaged_files.begin();
    for (const auto& testing: storage_manager_test.getStoragedFiles_test())
    {
        if ((testing.first == control_it->first) && (!testing.second.compare(control_it->second)))
             compare++;
        control_it++;
    }
    BOOST_CHECK(compare == static_cast<uint8_t>(storaged_files.size()));

    std::error_code error_on_remove;
    std::filesystem::remove_all(test_folder, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test folder " + test_folder);
    error_on_remove.clear();
}

BOOST_AUTO_TEST_CASE(create_folders)
{
    std::string test_folder_1 = "/home/" + std::string(getlogin()) + "/test1_create_folders";
    std::string test_folder_2 = "/home/" + std::string(getlogin()) + "/test2_create_folders";
    std::vector<std::string> folders_to_save {test_folder_1, test_folder_2};

    StorageManagerTest storage_manager_test;
    storage_manager_test.setFoldersToSave_Test(folders_to_save);
    storage_manager_test.createFolders_Test();

    std::error_code error_on_enter;
    std::filesystem::directory_entry(test_folder_1, error_on_enter);
    BOOST_CHECK(!error_on_enter);
    error_on_enter.clear();
    std::filesystem::directory_entry(test_folder_2, error_on_enter);
    BOOST_CHECK(!error_on_enter);
    error_on_enter.clear();

    std::error_code error_on_remove;
    std::filesystem::remove(test_folder_1, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test folder " + test_folder_1);
    error_on_remove.clear();

    std::filesystem::remove(test_folder_2, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test folder " + test_folder_2);
    error_on_remove.clear();
}

BOOST_AUTO_TEST_CASE(add_file_to_storage)
{

    std::string path_to_file = "/home/" + std::string(getlogin()) + "/test_add_file_to_storage.txt";
    std::ofstream test_file (path_to_file);
    BOOST_CHECK_MESSAGE(test_file.is_open(), "ERROR! Cannot create test file " + path_to_file);
    test_file.close();

    StorageManagerTest storage_manager_test;
    storage_manager_test.addFileToStorage_Test(path_to_file);

    BOOST_CHECK(!storage_manager_test.getStoragedFiles_test().begin()->second.compare(path_to_file));

    std::error_code error_on_remove;
    std::filesystem::remove(path_to_file, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test file " + path_to_file);
    error_on_remove.clear();
}

BOOST_AUTO_TEST_CASE(open_storage)
{
    std::string test_folder = "/home/" + std::string(getlogin()) + "/test_open_storage";

    StorageManagerTest storage_manager_test;
    storage_manager_test.setPathToStorage_Test(test_folder);

    std::thread thread_for_create([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::error_code error_on_create;
        std::filesystem::create_directory(test_folder,error_on_create);
        BOOST_CHECK_MESSAGE(!error_on_create, "ERROR! Cannot create test folder " + test_folder);
        error_on_create.clear();
    });

    storage_manager_test.openStorage_Test();
    thread_for_create.join();

    std::error_code error_on_remove;
    std::filesystem::remove(test_folder, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test folder " + test_folder);
    error_on_remove.clear();
}

BOOST_AUTO_TEST_CASE(controling_loop)
{
    std::string test_folder = "/home/" + std::string(getlogin()) + "/test_controling_loop";

    std::error_code error_on_create_folder;
    std::filesystem::create_directory(test_folder, error_on_create_folder);
    BOOST_CHECK_MESSAGE(!error_on_create_folder, "ERROR! Cannot create test folder " + test_folder);

    ConnectorTest connector_test;
    StorageManagerTest storage_manager_test;
    connector_test.addComponent(&storage_manager_test);
    storage_manager_test.setPathToStorage_Test(test_folder);
    storage_manager_test.setMinimalFreeSpace_Test(0);

    std::thread thread_for_remove([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::error_code error_on_remove;
        std::filesystem::remove(test_folder, error_on_remove);
        BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test folder " + test_folder);
        error_on_remove.clear();
    });

    storage_manager_test.controlingLoop_Test();
    thread_for_remove.join();

    BOOST_CHECK(connector_test.state_ == false);
}

BOOST_AUTO_TEST_CASE(set_global_stop)
{
    ConnectorTest connector_test;
    StorageManagerTest storage_manager_test;
    connector_test.addComponent(&storage_manager_test);

    connector_test.notifyGlobalStop();
    BOOST_CHECK(storage_manager_test.getGlobalStop_Test() == true);
}

BOOST_AUTO_TEST_CASE(attach)
{
    ConnectorTest connector_test;
    StorageManagerTest storage_manager_test;
    connector_test.addComponent(&storage_manager_test);

    BOOST_CHECK(storage_manager_test.getConnector_Test() != nullptr);
}

BOOST_AUTO_TEST_CASE(storage_control_loop)
{
    std::string test_folder = "/home/" + std::string(getlogin()) + "/test_storage_control_loop/";
    std::error_code error_on_create_folder;
    std::filesystem::create_directory(test_folder, error_on_create_folder);
    BOOST_CHECK_MESSAGE(!error_on_create_folder, "ERROR! Cannot create test folder " + test_folder);

    std::string test_file = "test_storage_control_loop.txt";
    std::string test_path_to_settings = test_folder + test_file;
    std::string check_folder_name {"test"};
    std::string check_url {"http://test"};

    std::ofstream create_test_file (test_path_to_settings);
    BOOST_CHECK_MESSAGE(create_test_file.is_open(), "ERROR! Cannot create test file " + test_path_to_settings);
    create_test_file << check_folder_name << std::endl << check_url << std::endl;
    create_test_file.close();

    StorageManagerTest storage_manager_test;
    storage_manager_test.setPathToStorage_Test(test_folder);
    storage_manager_test.setPathToSettings_Test(test_path_to_settings);
    ConnectorTest connector_test;
    connector_test.addComponent(&storage_manager_test);

    std::thread thread_for_change_state([&]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        connector_test.notifyGlobalStop();
    });

    storage_manager_test.storageControlLoop();
    thread_for_change_state.join();

    BOOST_CHECK(storage_manager_test.getGlobalStop_Test());

    std::error_code error_on_remove;
    std::filesystem::remove_all(test_folder, error_on_remove);
    BOOST_CHECK_MESSAGE(!error_on_remove, "ERROR! Cannot remove test files on path " + test_folder);
}

BOOST_AUTO_TEST_CASE(get_settings)
{
    std::vector<VideoServer::VideoSettings> test_settings_file
    {
        {"folder1", "url1"},
        {"folder2", "url2"},
    };
    StorageManagerTest storage_manager_test;
    storage_manager_test.setVideoSettingsFile_Test(test_settings_file);

    BOOST_CHECK(storage_manager_test.getVideoSettingsFile_Test() == test_settings_file);
}
