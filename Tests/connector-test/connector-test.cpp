#define BOOST_TEST_MODULE connector
#define BOOST_TEST_DYN_LINK

#include "connector-test.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(add_component_and_detach)
{
    ConnectorTest connector_test;

    VideoCaptureManagerTest capture_test;
    CommandLineReaderTest command_test;
    StorageManagerTest storage_manager_test;
    ThreadPoolTest thread_pool_test;

    connector_test.addComponent(&capture_test);
    connector_test.addComponent(&command_test);
    connector_test.addComponent(&storage_manager_test);
    connector_test.addComponent(&thread_pool_test);

    BOOST_CHECK(connector_test.getVideoCaptureManager());
    BOOST_CHECK(connector_test.getCommandLineReader());
    BOOST_CHECK(connector_test.getStorageManager());
    BOOST_CHECK(connector_test.getThreadPool());

    connector_test.detach_Test(&capture_test);
    connector_test.detach_Test(&command_test);
    connector_test.detach_Test(&storage_manager_test);
    connector_test.detach_Test(&thread_pool_test);

    BOOST_CHECK(!connector_test.getVideoCaptureManager());
    BOOST_CHECK(!connector_test.getCommandLineReader());
    BOOST_CHECK(!connector_test.getStorageManager());
    BOOST_CHECK(!connector_test.getThreadPool());
}

BOOST_AUTO_TEST_CASE(create_new_thread)
{
    ConnectorTest connector_test;
    ThreadPoolTest thread_pool_test;

    connector_test.addComponent(&thread_pool_test);

    static bool check = false;

    auto function_test = []()
    {
        check = true;
        BOOST_CHECK(check);
    };

    connector_test.createNewThread_Test(function_test);

    BOOST_CHECK(check);
}

BOOST_AUTO_TEST_CASE(add_file_to_storage)
{
    ConnectorTest connector_test;
    StorageManagerTest storage_manager_test;

    connector_test.addComponent(&storage_manager_test);

    std::string test_data {"test"};
    connector_test.addFileToStorage_Test(test_data);

    BOOST_CHECK(storage_manager_test.data_ == test_data);
}

BOOST_AUTO_TEST_CASE(notify_storage_changed_state)
{
    ConnectorTest connector_test;
    VideoCaptureManagerTest capture_test;

    connector_test.addComponent(&capture_test);

    bool test_state {true};
    connector_test.notifyStorageChangedState_Test(test_state);

    BOOST_CHECK(capture_test.state_ == test_state);
}

BOOST_AUTO_TEST_CASE(notify_global_stop)
{
    ConnectorTest connector_test;

    VideoCaptureManagerTest capture_test;
    StorageManagerTest storage_manager_test;

    connector_test.addComponent(&capture_test);
    connector_test.addComponent(&storage_manager_test);

    connector_test.notifyGlobalStop_Test();

    BOOST_CHECK(connector_test.getGlobalStopState());
}

BOOST_AUTO_TEST_CASE(storage_ready)
{
    ConnectorTest connector_test;

    VideoCaptureManagerTest capture_test;
    StorageManagerTest storage_manager_test;

    connector_test.addComponent(&capture_test);
    connector_test.addComponent(&storage_manager_test);

    connector_test.storageReady_Test();

    BOOST_CHECK(capture_test.settings_ == storage_manager_test.settings_);
}

BOOST_AUTO_TEST_CASE(wait_for_correct_global_stop)
{
    ConnectorTest connector_test;
    connector_test.notifyGlobalStop_Test();

    StorageManagerTest storage_manager_test;
    ThreadPoolTest thread_pool_test;

    connector_test.notifyGlobalStop_Test();

    connector_test.addComponent(&storage_manager_test);
    connector_test.addComponent(&thread_pool_test);

    std::thread thread_for_create([&]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connector_test.detach_Test(&thread_pool_test);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connector_test.detach_Test(&storage_manager_test);
    });

    connector_test.waitForCorrectGlobalStop();
    thread_for_create.join();
    BOOST_CHECK(connector_test.getGlobalStopState());
}
