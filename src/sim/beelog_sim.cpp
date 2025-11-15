#include "BeeLog.hpp"
#include <iostream>

#if WIN32
#include <fstream>
#include <lvgl.h>
std::ofstream out;
static void lv_log(lv_log_level_t level, const char *buf)
{
    std::cout << buf;
}
#endif

void BeeLog::init() {
#if WIN32
    lv_log_register_print_cb(&lv_log);
    out.open("beelog.log");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());                // redirect std::cout to out.txt
#endif
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
