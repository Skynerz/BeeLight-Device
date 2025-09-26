#ifndef BEELOG_HPP
#define BEELOG_HPP

#include <string>

class BeeLog {
public:
    explicit BeeLog(std::string tag) : tag_m(tag)
    {
    }

    static void debug(std::string tag, std::string message);
    static void info(std::string tag, std::string message);
    static void error(std::string tag, std::string message);

    void debug(std::string message)
    {
        BeeLog::debug(tag_m, message);
    }

    void info(std::string message)
    {
        BeeLog::info(tag_m, message);
    }

    void error(std::string message)
    {
        BeeLog::error(tag_m, message);
    }

private:
    std::string tag_m;
};

#endif