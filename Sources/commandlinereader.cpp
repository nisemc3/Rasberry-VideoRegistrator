#include "Headers/commandlinereader.hpp"

#include <iostream>

CommandLineReader::CommandLineReader()
{}

CommandLineReader::~CommandLineReader()
{
}

void CommandLineReader::checkCommand(std::string& command)
{
    if (command == "exit")
    {
        global_stop_ = true;
        if (connector_)
            connector_->notifyGlobalStop();
    }
}

void CommandLineReader::attach(IConnector* connector)
{
    connector_ = connector;
}

void CommandLineReader::setGlobalStop()
{
    global_stop_ = true;
}

void CommandLineReader::readLoop()
{
    std::string text;
    std::cout << "Enter \"exit\" to stop" << '\n';

    while (!global_stop_)
    {
        std::getline(std::cin, text);
        checkCommand(text);

        if (text == "exit")
        {
            break;
        }
    }

    if (connector_)
        connector_->detach(this);

    connector_ = nullptr;
}

