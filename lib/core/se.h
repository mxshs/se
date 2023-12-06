#include "string"
#include "filesystem"
#include "vector"
#include "map"

namespace se
{
    struct Cache {
        public:
            Cache(std::string);
            int getTotalFrequency(std::string);
            std::vector<std::string> getLocations(std::string);
            void updateCache(std::string);
        private:
            std::map<std::string, int> Total;
            std::map<std::string, std::vector<std::string>> Loc;
            time_t updateStamp;
            void scanAllDocs(std::string);
            void cacheFile(std::filesystem::directory_entry);
    };


    struct SE {
        public:
            SE(Cache cache);
            std::vector<std::pair<std::string, float>> search(std::string);
        private:
            Cache cache;
            float TFIDF(std::string, std::string);
    };
}

