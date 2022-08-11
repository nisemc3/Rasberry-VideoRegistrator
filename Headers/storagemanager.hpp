#ifndef StorageManager_H
#define StorageManager_H

#include "Headers/functions.hpp"
#include "Headers/io.hpp"

#include <condition_variable>
#include <unistd.h> // для поиска имени пользователя linux
#include <vector>
#include <map>
#include <mutex>
#include <system_error>
#include <fstream> // для работы с открытием файлов
#include <list>

/**
 * @brief Осуществляет контроль над состоянием хранилища:
 * - считывает файл конфигурации,
 * - следит за свободным местом,
 * - следит за статусом доступа к хранилищу.
 *
 * Запускается в работу метомод storageControlLoop, который является бесконечным циклом, для завершения работы цикла
 * необходимо запустить метод setGlobalStop.
 */
class StorageManager : public IStorageManager
{
public:
    StorageManager();
    ~StorageManager();
    /**
     * @brief запускает работу объекта, бесконечный цикл наблюдения за доступностью съемного диска.
     */
    void storageControlLoop();
    /**
     * @brief флаг о готовности объекта к работе.
     */
    bool ready_;

    const std::vector<VideoServer::VideoSettings>& getSettings() override;

protected:
    /**
     * @brief функция интерфейса, добавляет файл в список сохраненных файлов .
     */
    void addFileToStorage(const std::string& file_path) override;
    /**
     * @brief функция базового компонента, регистрирует кому и от кого будут идти сигналы.
     */
    void attach(IConnector* connector) override;
    /**
     * @brief функция базового компонента, меняет значение флага global_stop на false.
     */
    void setGlobalStop() override;
    /**
     * @brief инициализирует поля класса.
     */
    void initiateMembers();
    /**
     * @brief проверяет существует ли необходимое минимальное место в хранилище.
     */
    void checkForStorageSpace();
    /**
     * @brief выдает свободное место в хранилище, мегабайты.
     */
    uint64_t getAvalibleStorageSpace();
    /**
     * @brief уведомляет, что связь с хранилищем потеряно .
     */
    void notifyStorageChangedState(bool state);
    /**
     * @brief цикл, в котором происходит опрос флешки на её существование.
     */
    void controlingLoop();
    /**
     * @brief заполнение резервированных названий для исключение их удаления при очистке места.
     */
    void openStorage();
    /**
     * @brief поиск и открытие файла конфигураций в корневой папке съемного хранилища.
     */
    void openSettings();
    /**
     * @brief поиск и запись всех файлов хранилища в контейнер.
     */
    void parseStorage();
    /**
     * @brief указывает, зарезервировано ли название файла (для исключения удаления)
     */
    bool isReserved(const std::string& name_to_compare);
    /**
     * @brief создает папки, куда будут записываться видео.
     */
    void createFolders();
    /**
     * @brief выводит время последнего изменения файла в секундах (с начала отсчета в системе (1970 год).
     */
    uint64_t getFileChangeTime(const std::string& file_path);
    /**
     * @brief настройки, полученные из файла кофигураций, пара = (папка для сохранения видео) - (url ссылка на источник видео).
     */
    std::vector<VideoServer::VideoSettings> video_settings_file_;
    /**
     * @brief файлы, которые не будут удаляться при освобождении места (settings.txt и тд.).
     */
    std::vector<std::string> reserved_names_;
    /**
     * @brief список файлов, полученный после parseStorage, ключ = дата последнего изменения, значение = путь к файлу.
     */
    std::multimap<uint64_t, std::string> storaged_files_;
    /**
     * @brief путь к файлу конфигураций.
     */
    std::string path_to_settings_;
    /**
     * @brief путь в корневую папку съемных хранилищ.
     */
    std::string path_to_storage_;
    /**
     * @brief наименования хранилища.
     */
    std::string name_of_storage_;
    /**
     * @brief наименования файла конфигураций, где прописываются url и названия камер.
     */
    std::string settings_name_;
    /**
     * @brief наибольший размер одного пятиминутного видео в мегабайтах, вычисленно опытным путем (разброс от 100 мб до 500 мб).
     */
    uint16_t video_size_;
    /**
     * @brief для конвертации кило - мега.
     */
    float convert_number_;
    /**
     * @brief минимальное количество места, которое должно оставаться свободным, выражается в мегабайтах.
     */
    uint64_t minimal_free_space_;
    /**
     * @brief таймер, который начинает работать при инициализации объекта (для повторного парса хранилища на файлы).
     */
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> parse_timer_start_;
    /**
     * @brief время в секундах, через которое необходимо парсить заново хранилище, для безопасности.
     */
    float parse_lifetime_;
    /**
     * @brief просмотр ошибок функций std::filesystem.
     */
    std::error_code error_code_;
    /**
     * @brief объект, который работает с открытием файлов.
     */
    std::ifstream file_opener_;
    /**
     * @brief флаг, который показывает успешное открытие хранилища и открытие файла конфигураций.
     */
    bool storage_opened_success_;
    /**
     * @brief флаг, который показывает успешное открытие файла конфигураций.
     */
    bool settings_opened_success_;
    /**
     * @brief мьютекс, который блокируется при удалении файлов при очистке места и при добавлении нового файла в хранилище.
     */
    std::mutex mutex_;
};

#endif // StorageManager_H
