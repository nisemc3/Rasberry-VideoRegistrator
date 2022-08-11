#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Headers/io.hpp"

#include <condition_variable>
#include <list>
#include <mutex>
#include <string>

/**
 * @brief класс-посредник, через него взаимодействуют между собой компаненты программы.
 * - также отвечает за корректное завершение работы программы.
 */
class Connector: public IConnector
{
public:
    Connector();
    ~Connector();

    void addComponent(IVideoCaptureManager* component);
    void addComponent(ICommandLineReader* component);
    void addComponent(IStorageManager* component);
    void addComponent(IThreadPool* component);

    void waitForCorrectGlobalStop();

protected:
    void createNewThread(std::function<void()> newTask) override;
    /**
     * @brief перенаправляет к StorageManager информацию о создании нового файла.
     */
    void addFileToStorage(const std::string& data) override;
    /**
     * @brief команда о статусе соединения с хранилищем.
     */
    void notifyStorageChangedState(const bool state) override;
    /**
     * @brief делает рассылку компонентам о необходимости прекратить все циклы и завершить работу.
     */
    void notifyGlobalStop() override;
    /**
     * @brief уведомление, что связь с хранилищем установлена, менеджер хранилища сделал необходимые приготовления.
     */
    void storageReady() override;

    void detach(IVideoCaptureManager* component) override;
    void detach(ICommandLineReader* component) override;
    void detach(IStorageManager* component) override;
    void detach(IThreadPool* component) override;

    IVideoCaptureManager* video_capture_manager_ {nullptr};
    ICommandLineReader* command_line_reader_ {nullptr};
    IStorageManager* storage_manager_ {nullptr};
    IThreadPool* thread_pool_ {nullptr};

    bool global_stop_ {false};
    std::mutex mutex_global_stop_;
    std::condition_variable condition_global_stop_;
};

#endif // CONNECTOR_H
