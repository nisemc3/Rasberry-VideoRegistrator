#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ctime>
#include <string>

namespace VideoServer
{
    using path = std::string;
    using url = std::string;
    using VideoSettings = std::pair<path, url>;

    inline std::string getCurrentDate()
    {
        std::time_t curtime;
        struct std::tm* loctime;
        char buffer[17];
        std::time(&curtime);
        loctime = std::localtime(&curtime);
        std::strftime(buffer, 17, "%Y_%m_%d-%H_%M", loctime);
        return buffer;
    }
}

#endif // FUNCTIONS_H
