#pragma once

#include <string>

class Log
{
    public:
        static void msg(std::string str);
        static void err(std::string str);
        static void errAndQuit(std::string str);
};
