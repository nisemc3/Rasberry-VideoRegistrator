#define BOOST_TEST_MODULE videocaptureclient
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "videocaptureclient-test.hpp"

BOOST_AUTO_TEST_CASE(set_global_stop)
{
    VideoCaptureClientTest video_capture_test;

    video_capture_test.setGlobalStop_Test(false);
    BOOST_CHECK(!video_capture_test.getGlobalStop_Test());

    video_capture_test.setGlobalStop();
    BOOST_CHECK(video_capture_test.getGlobalStop_Test());
}

BOOST_AUTO_TEST_CASE(storage_changed_state)
{
    VideoCaptureClientTest video_capture_test;

    video_capture_test.storageChangedState(false);
    BOOST_CHECK(!video_capture_test.getStorageState());

    video_capture_test.storageChangedState(true);
    BOOST_CHECK(video_capture_test.getStorageState());
}

BOOST_AUTO_TEST_CASE(start_client)
{

}

BOOST_AUTO_TEST_CASE(get_settings)
{
    VideoServer::VideoSettings settings_test {"path1", "url1"};
    VideoCaptureClientTest video_capture_test (settings_test);

    BOOST_CHECK(settings_test == video_capture_test.getSettings());
}

BOOST_AUTO_TEST_CASE(read_write_loop)
{

}

BOOST_AUTO_TEST_CASE(open_camera_by_URL)
{

}

BOOST_AUTO_TEST_CASE(open_file_to_save)
{

}

