#ifndef VIDEOCAPTUREMANAGER_H
#define VIDEOCAPTUREMANAGER_H

#include "Headers/videocaptureclient.hpp"

#include <condition_variable>
#include <list>

/**
 * @brief Осуществляет контроль над клиентами захвата видео:
 * - получает конфигурации для создания новых клиентов,
 * - закрывает неактульных клиентов,
 * - передают текущий статус хранилища,
 * - переправляет информации о записи видео менеджеру хранилища.
 */
class VideoCaptureManager: public IVideoCaptureManager, public ISubject
{
public:
    VideoCaptureManager();
    ~VideoCaptureManager();
    /**
     * @brief создает клиентов записи видео по указанным настройкам.
     */
    void initiateVideoCaptureClients(const std::vector<VideoServer::VideoSettings>& settings) override;
    /**
     * @brief отсоединяет клиент записи видео.
     */
    void detach(IObserver* observer) override;

protected:
    /**
     * @brief закрывает клиентов, которые не указаны при новой инициализации.
     */
    void closeUnusedClients(const std::vector<VideoServer::VideoSettings>& settings);
    /**
     * @brief добавляет в реестр хранилища созданный файл.
     */
    void addFileToStorage(const std::string& data) override;
    /**
     * @brief получает информацию от хранилища о текущем состоянии.
     */
    void storageChangedState(bool state) override;
    /**
     * @brief присоединяет класс-посредник, через который осуществляется взаимодействие разных классов.
     */
    void attach(IConnector* connector) override;
    /**
     * @brief получает сигнал о необходимости завершить работу.
     */
    void setGlobalStop() override;
    /**
     * @brief создает объект, который записывает видео по заданным настройкам.
     */
    void createVideoCaptureClient(const VideoServer::VideoSettings& settings);
    /**
     * @brief список созданных объектов в которых происходит запись видео.
     */
    std::list<VideoCaptureClient*> clients_ {};
    std::mutex mutex_list_;
    std::condition_variable wait_for_clients_stop_;
};

#endif // VIDEOCAPTUREMANAGER_H
