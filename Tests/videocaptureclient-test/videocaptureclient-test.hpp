#ifndef VIDEOCAPTURECLIENTTEST_HPP
#define VIDEOCAPTURECLIENTTEST_HPP

#include <Sources/videocaptureclient.cpp>

class VideoCaptureClientTest: public VideoCaptureClient
{
public:
    VideoCaptureClientTest():
        VideoCaptureClient(nullptr, {})
    {}

    VideoCaptureClientTest(const VideoServer::VideoSettings& settings):
        VideoCaptureClient(nullptr, settings)
    {}

    ~VideoCaptureClientTest(){}

    void readWriteLoop_Test();
    void openCameraByURL_Test();
    void openFileToSave_Test();

    bool getGlobalStop_Test();
    void setGlobalStop_Test(bool state);

    bool getStorageState();
};

inline void VideoCaptureClientTest::readWriteLoop_Test()
{
    readWriteLoop();
}

inline void VideoCaptureClientTest::openCameraByURL_Test()
{
    openCameraByURL();
}

inline void VideoCaptureClientTest::openFileToSave_Test()
{
    openFileToSave();
}

inline bool VideoCaptureClientTest::getGlobalStop_Test()
{
    return global_stop_;
}

inline void VideoCaptureClientTest::setGlobalStop_Test(bool state)
{
    global_stop_ = state;
}

inline bool VideoCaptureClientTest::getStorageState()
{
    return storage_exists_;
}





#endif // VIDEOCAPTURECLIENTTEST_HPP
