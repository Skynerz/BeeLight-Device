#include <Arduino.h>

#include <iostream>

#include "BeeLog.hpp"

void BeeLog::init() {
    Serial.begin(115200);
}

void BeeLog::debug(const std::string& tag, const std::string& message) {
    std::cout << "[D]-" << tag << "- " << message << std::endl;
}

void BeeLog::info(const std::string& tag, const std::string& message) {
    std::cout << "[I]-" << tag << "- " << message << std::endl;
}

void BeeLog::warn(const std::string& tag, const std::string& message) {
    std::cout << "[W]-" << tag << "- " << message << std::endl;
}

void BeeLog::error(const std::string& tag, const std::string& message) {
    std::cerr << "[E]-" << tag << "- " << message << std::endl;
}
