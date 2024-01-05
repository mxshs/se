#include "string"
#include "filesystem"
#include "vector"
#include "map"
#include "../cache/cache.h"

std::string textMatch(std::string, std::string);
std::string cleanText(std::string);

struct SE {
    public:
        SE(): cache(Cache()){};
        std::vector<std::pair<std::string, float>> search(std::string);
    private:
        Cache cache;
        float TFIDF(std::string, std::string);
};
