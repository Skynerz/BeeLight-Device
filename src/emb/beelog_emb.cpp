#include "BeeLog.hpp"
#include <Arduino.h>
#include <iostream>

void BeeLog::init() {
    Serial.begin(115200);
}

void BeeLog::debug(std::string tag, std::string message)
{
    std::cout << "[D]-" << tag << "- " << message << std::endl;
}

void BeeLog::info(std::string tag, std::string message)
{
    std::cout << "[I]-" << tag << "- " << message << std::endl;
}

void BeeLog::error(std::string tag, std::string message)
{
    std::cerr << "[E]-" << tag << "- " << message << std::endl;
}
