#include "BeeLog.hpp"
#include <iostream>

void BeeLog::debug(std::string tag, std::string message)
{
    std::cout << "[D]-" << tag << " - " << message;
}

void BeeLog::info(std::string tag, std::string message)
{
    std::cout << "[I]-" << tag << " - " << message;
}

void BeeLog::error(std::string tag, std::string message)
{
    std::cerr << "[E]-" << tag << " - " << message;
}
