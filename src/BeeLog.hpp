#ifndef BEELOG_HPP
#define BEELOG_HPP

#include <string>

class BeeLog {
   public:
    explicit BeeLog(std::string tag) : tag_m(tag) {
        init();
    }

    static void debug(const std::string& tag, const std::string& message);
    static void info(const std::string& tag, const std::string& message);
    static void warn(const std::string& tag, const std::string& message);
    static void error(const std::string& tag, const std::string& message);

    void debug(const std::string& message) const {
        BeeLog::debug(tag_m, message);
    }

    void info(const std::string& message) const {
        BeeLog::info(tag_m, message);
    }

    void warn(const std::string& message) const {
        BeeLog::warn(tag_m, message);
    }

    void error(const std::string& message) const {
        BeeLog::error(tag_m, message);
    }

   private:
    std::string tag_m;
    void init();
};

#endif
