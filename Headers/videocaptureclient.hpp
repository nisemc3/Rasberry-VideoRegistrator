#ifndef VIDEOCAPTURECLIENT_H
#define VIDEOCAPTURECLIENT_H

#include "Headers/io.hpp"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

/**
 * @brief Осуществляет подключение к ip камере, сохраняет видео в указанные папки из
 * файла конфигураций.
 * - нарезает видео по 5 минут,
 * - следит за состоянием подключения к камере.
 *
 * Запускается в работу методом startClient, который является бесконечным циклом, для завершения работы цикла
 * необходимо запустить метод setGlobalStop.
 */
class VideoCaptureClient: public IObserver
{
public:
    VideoCaptureClient() = delete;
    VideoCaptureClient(ISubject* subject, const VideoServer::VideoSettings& settings);
    ~VideoCaptureClient();
    /**
     * @brief получает сигнал о необходимости завершить работу.
     */
    void setGlobalStop() override;
    /**
     * @brief получает информацию от хранилища о текущем состоянии.
     */
    void storageChangedState(bool state) override;
    /**
     * @brief запускает в работу объект.
     */
    void startClient();
    /**
     * @brief получить настройки сохраняемого видео.
     */
    VideoServer::VideoSettings getSettings() const;

protected:
    /**
     * @brief цикл, в котором объект получает покадрово изображение из видео.
     */
    void readWriteLoop();
    /**
     * @brief подключается к видео по ссылке url.
     */
    void openCameraByURL();
    /**
     * @brief создает файл и записывает туда кадры из потока.
     */
    void openFileToSave();
    /**
     * @brief инициализация приватных полей.
     */
    void initiateMembers();

    const VideoServer::VideoSettings settings_;
    /**
     * @brief кадр, получаемый из потока.
     */
    cv::Mat frame_;
    /**
     * @brief объект отрытия потока видео по ссылке url.
     */
    cv::VideoCapture capture_;
    /**
     * @brief объект записи кадров в файл.
     */
    cv::VideoWriter writer_;
    /**
     * @brief размер получаемого кадра из видео по ссылке.
     */
    cv::Size frame_size_;
    /**
     * @brief путь для создания файла.
     */
    VideoServer::path path_to_save_;
    /**
     * @brief ссылка url на поток.
     */
    VideoServer::url source_url_;
    /**
     * @brief флаг о состоянии хранилища.
     */
    bool storage_exists_;
    /**
     * @brief флаг, является ли видео поток цветным.
     */
    bool is_color_;
    /**
     * @brief флаг о необходимости остановить объект.
     */
    bool global_stop_;
    /**
     * @brief кодек записи кадров в файл.
     */
    int32_t codec_;
    /**
     * @brief количество кадров в секунду для записи кадров в файл.
     */
    double fps_;
    /**
     * @brief размер нарезки видео в секундах.
     */
    float cut_seconds_;
    /**
     * @brief размер нарезки видео в секундах.
     */
    ISubject* subject_;
};

#endif // VIDEOCAPTURECLIENT_H
