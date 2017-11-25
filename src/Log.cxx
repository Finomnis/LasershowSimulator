#include "Log.hxx"

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace
{
std::string getTime()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    auto tp = now.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
    std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(tp);

    char timeString[30];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", std::localtime(&now_c));

    std::string millis_string = std::to_string(int(millis.count()));
    while (millis_string.size() < 3)
        millis_string = std::string("0") + millis_string;


    return std::string(timeString) + "." + millis_string;
}
}

void Log::msg(std::string str)
{
    std::stringstream ss;
    ss << getTime() << " | " << str << std::endl;
    std::cout << ss.str() << std::flush;
}


void Log::err(std::string str)
{
    std::stringstream ss;
    ss << getTime() << " | ERROR: " << str << std::endl;
    std::cerr << ss.str() << std::flush;
}


void Log::errAndQuit(std::string str)
{
    std::stringstream ss;
    ss << getTime() << " | ERROR: " << str << std::endl;
    std::cerr << ss.str() << std::flush;
    std::exit(EXIT_FAILURE);
}
