#include "Headers/videocaptureclient.hpp"
#include "Headers/functions.hpp"

#include <thread>
#include <iostream>

VideoCaptureClient::VideoCaptureClient(ISubject* subject, const VideoServer::VideoSettings& settings)
    :settings_(settings), subject_(subject)
{
    initiateMembers();
}

void VideoCaptureClient::readWriteLoop()
{
    auto start = std::chrono::system_clock::now();

    while (!global_stop_ && storage_exists_)
    {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed_seconds = end - start;

        if (elapsed_seconds.count() >= cut_seconds_)
        {
           openFileToSave();
           start = std::chrono::system_clock::now();
        }

        capture_.read(frame_);

        if (frame_.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        writer_.write(frame_);
    }
}

void VideoCaptureClient::openCameraByURL()
{
    capture_.release();
    capture_.open(source_url_, cv::CAP_FFMPEG);

    if (!capture_.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera " << source_url_ << '\n';
        return;
    }

    frame_size_ = cv::Size(static_cast<int32_t>(capture_.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast<int32_t>(capture_.get(cv::CAP_PROP_FRAME_HEIGHT)));
}

void VideoCaptureClient::openFileToSave()
{
    writer_.release();

    std::string filename = path_to_save_ + VideoServer::getCurrentDate() + ".avi";
    writer_.open(filename, codec_, fps_, frame_size_, is_color_);

    if (!writer_.isOpened())
    {
        std::cerr << "Could not create the output video file for write on path " << path_to_save_ << '\n';
        return;
    }

    subject_->addFileToStorage(filename);
}

void VideoCaptureClient::initiateMembers()
{
    capture_.release();
    writer_.release();

    global_stop_ = false;
    path_to_save_ = settings_.first;
    source_url_ = settings_.second;
    storage_exists_ = false;
    is_color_ = true;
    codec_ = cv::VideoWriter::fourcc('X','V','I','D');
    fps_ = 25.0;
    cut_seconds_ = 300.0f;
}

void VideoCaptureClient::startClient()
{
    while (!global_stop_)
    {
        openCameraByURL();

        if (capture_.isOpened())
        {
            openFileToSave();

            if (capture_.isOpened())
                readWriteLoop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    subject_->detach(this);
}

VideoServer::VideoSettings VideoCaptureClient::getSettings() const
{
    return settings_;
}

void VideoCaptureClient::storageChangedState(bool state)
{
    storage_exists_ = state;
}

void VideoCaptureClient::setGlobalStop()
{
    global_stop_ = true;
}

VideoCaptureClient::~VideoCaptureClient()
{
}
