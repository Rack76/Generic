#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

namespace Gen
{
    class Logger
    {
    public:
        static void logError(std::string& err)
        {
            std::cerr << "error : " + err + "\n";
        }
    };
}  // namespace Gen

#endif
