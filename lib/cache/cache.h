#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Cache {
    public:
        Cache() {
            // TODO: find a better way to deal with file_time -> sys_clock conversions
            using clock = std::filesystem::file_time_type::clock;

            auto upd = std::filesystem::last_write_time("test.json");
            auto now = std::chrono::system_clock::now();
            auto since = std::chrono::duration<double>((now - clock::to_sys(upd)));

            if (since.count() / 60 > 30) {
                this->cacheAll();
            } else {
                loadCache();
            }
        }
        void cacheAll();
        void cacheFile(std::filesystem::directory_entry);
        void loadCache();
        std::map<std::string, std::vector<std::pair<std::string, int>>> Map;
    private:
        void JSONdump();
};
