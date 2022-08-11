#include "Headers/functions.hpp"
#include "Headers/videocapturemanager.hpp"
#include "Headers/threadpool.hpp"
#include "Headers/commandlinereader.hpp"
#include "Headers/storagemanager.hpp"
#include "Headers/connector.hpp"

#include <iostream>

void startStorageManager(StorageManager& storage_manager)
{
    storage_manager.storageControlLoop();
}

void readLineLoop(CommandLineReader& line_reader)
{
    line_reader.readLoop();
}

int main()
{
    Connector connector;

    ThreadPool thread_pool;
    StorageManager store_manager;
    CommandLineReader line_reader;
    VideoCaptureManager video_capture_manager;

    connector.addComponent(&thread_pool);
    connector.addComponent(&store_manager);
    connector.addComponent(&line_reader);
    connector.addComponent(&video_capture_manager);

    thread_pool.createThreadWithTask(std::bind(&startStorageManager, std::ref(store_manager)));
    thread_pool.createThreadWithTask(std::bind(&readLineLoop, std::ref(line_reader)));

    connector.waitForCorrectGlobalStop();
}
