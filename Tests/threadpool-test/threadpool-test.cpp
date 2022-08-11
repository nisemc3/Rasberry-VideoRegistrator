#define BOOST_TEST_MODULE threadpool
#define BOOST_TEST_DYN_LINK

#include "threadpool-test.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(attach)
{
    ThreadPoolTest thread_pool_test;
    ConnectorTest connector_test;

    connector_test.addComponent(&thread_pool_test);

    BOOST_CHECK(thread_pool_test.getConnector_Test());
}

BOOST_AUTO_TEST_CASE(set_global_stop)
{
    ThreadPoolTest thread_pool_test;
    ConnectorTest connector_test;

    connector_test.addComponent(&thread_pool_test);

    auto test_func = [&]()
    {
    };

    thread_pool_test.createThreadWithTask_Test(test_func);

    thread_pool_test.setGlobalStop_Test();

    BOOST_CHECK(thread_pool_test.getState() && !thread_pool_test.getConnector_Test());
}

BOOST_AUTO_TEST_CASE(running_thread)
{
    ThreadPoolTest thread_pool_test;

    static bool test_check {false};

    auto test_func = [&]()
    {
        test_check = true;
    };

    thread_pool_test.runningThread_Test(test_func, 0);

    BOOST_CHECK(test_check);

    thread_pool_test.StopPool();
}

BOOST_AUTO_TEST_CASE(create_thread_with_task)
{
    ThreadPoolTest thread_pool_test;

    static uint16_t test_check {};

    auto test_func = [&]()
    {
        ++test_check;
    };

    thread_pool_test.createThreadWithTask_Test(test_func);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    BOOST_CHECK(thread_pool_test.getQueue().size() == 1);

    thread_pool_test.createThreadWithTask_Test(test_func);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    BOOST_CHECK(test_check == thread_pool_test.getThreadIdentificator());

    thread_pool_test.StopPool();
}
