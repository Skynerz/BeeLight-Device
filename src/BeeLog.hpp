#ifndef BEELOG_HPP
#define BEELOP_HPP

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
        debug(tag_m, message);
    }

    void info(std::string message)
    {
        info(tag_m, message);
    }

    void error(std::string message)
    {
        error(tag_m, message);
    }

private:
    std::string tag_m;
};

#endif