#include <filesystem>
#include <fstream>
#include <iterator>
#include "../utils/utils.h"
#include "cache.h"

void Cache::cacheAll() {
    std::string path = "/path/to/dir";

    for (auto entry: std::filesystem::recursive_directory_iterator(path)) {
        this->cacheFile(entry);
    }
}

void Cache::cacheFile(std::filesystem::directory_entry file) {
    std::ifstream reader(file.path());

    auto start = std::istreambuf_iterator<char>(reader);
    auto end = std::istreambuf_iterator<char>();

    WordIter<std::istreambuf_iterator<char>> itr(start, end);

    std::map<std::string, int> seen;

    for (auto w: itr) {
        if (seen.contains(w.word)) {
            seen[w.word] += 1;
        } else {
            seen.insert({w.word, 1});
        }
    }

    for (const auto &[key, value]: seen) {
        if (this->Map.contains(key)) {
            this->Map[key].push_back({file.path(), value});
        } else {
            this->Map.insert({
                key,
                std::vector<std::pair<std::string, int>>(
                    {std::pair<std::string, int>(file.path(), value)}
                )
            });
        }
    }

    reader.close();

    this->JSONdump();
}

// TODO: temporary stuff to make output readable, move to some json parser later
void Cache::JSONdump() {
    std::string c = "[\n";

    for (auto &[key, value]: this->Map) {
        std::string wordObject = "\t{\n\t\t" + key + ": [\n";
        std::vector<std::string> pairs;

        for (auto &[path, count]: value) {
            std::string wordPath = "\"path\": \"" + path + "\",";
            std::string wordCount = "\"count\": " + std::to_string(count) + ",";

            pairs.push_back(wordPath);
            pairs.push_back(wordCount);
        }

        wordObject += formatIndent(pairs, int(3));
        c += wordObject + "\t\t]\n\t},\n";
    }

    std::ofstream dump("test.json");

    dump << c + "}";
};

void Cache::loadCache() {
    std::ifstream reader("test.json");

    auto start = std::istreambuf_iterator<char>(reader);
    auto end = std::istreambuf_iterator<char>();

    WordIter<std::istreambuf_iterator<char>> itr(start, end);
    ++itr;

    while (itr != itr.end()) {

        std::string word = itr.word;
        
        this->Map.insert({word, std::vector<std::pair<std::string, int>>()});

        while (true) {
            ++itr;
            if (itr.word != "path") {
                break;
            }
            ++itr;

            std::string path;

            while (itr.word != "count") {
                path += "/" + itr.word;
                ++itr;
            }
            ++itr;

            int count = std::stoi(itr.word);

            this->Map[word].push_back(std::pair<std::string, int>(path, count));
        }
    }
}

