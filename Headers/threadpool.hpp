#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Headers/io.hpp"

#include <list>
#include <map>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

/**
 * @brief Осуществляет контроль над созданием потоков, для корректной остановки потоков
 * необходимо из main потока вызвать метод setGlobalStop
 */
class ThreadPool: public IThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    /**
     * @brief Добавляет поток с указанным заданием, необходимо использовать std::bind для
     * передачи задания с дополнительными параметрами.
     */
    void createThreadWithTask(std::function<void()> newTask) override;

protected:
    void attach(IConnector* connector) override;
    /**
     * @brief делает закрытие всех дополнительных потоков, необходимо вызывать из main потока.
     */
    void setGlobalStop() override;
    /**
     * @brief функция, запускающая задание в новом потоке.
     */
    void runningThread(std::function<void()> newTask, uint16_t thread_identificator);
    /**
     * @brief список созданных потоков.
     */
    std::map<uint16_t, std::thread> pool_ {};
    /**
     * @brief очередь потоков для остановки.
     */
    std::queue<uint16_t> threads_to_detach_ {};
    /**
     * @brief список созданных потоков.
     */
    uint16_t thread_identificator_ {};
    /**
     * @brief мьютекс, который блокируется при удалении или добавлении в список созданных потоков.
     */
    std::mutex mutex_;
};

#endif // THREADPOOL_H
