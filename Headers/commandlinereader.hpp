#ifndef COMMANDLINEREADER_H
#define COMMANDLINEREADER_H

#include "Headers/io.hpp"

#include <list>
#include <mutex>
#include <condition_variable>

/**
 * @brief Осуществляет чтение команд из командной строки.
 * Запускается в работу метомод readLoop, который является бесконечным циклом, для завершения работы цикла
 * необходимо запустить метод setGlobalStop.
 */
class CommandLineReader: public ICommandLineReader
{
public:
    CommandLineReader();
    ~CommandLineReader();

    void readLoop();

protected:
    void attach(IConnector* connector) override;
    void setGlobalStop() override;

    void checkCommand(std::string& command);
};

#endif // COMMANDLINEREADER_H
