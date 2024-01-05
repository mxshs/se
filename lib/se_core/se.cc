#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "se.h"
#include "../utils/utils.h"

bool tfidfComparator(const std::pair<std::string, float> &a,
        const std::pair<std::string, float> &b) {
    return (a.second > b.second);
}

std::vector<std::pair<std::string, float>> SE::search(std::string input) {
    std::string word;
    std::map<std::string, float> result;

    WordIter<std::string::iterator> itr(input.begin(), input.end());

    for (auto w: itr) {
        word = w.word;
        if (this->cache.Map.contains(word)) {
            auto df = this->cache.Map[word].size();

            for (auto w: this->cache.Map[word]) {
                float tfidf = float(w.second) / float(df);
                if (result.contains(w.first)) result[w.first] += tfidf;
                else result[w.first] = tfidf;
            }
        }
    }


    std::vector<std::pair<std::string, float>> top;

    for (auto const &[key, val]: result) {
        top.push_back(std::pair<std::string, float>(key, val));
    }

    sort(top.begin(), top.end(), tfidfComparator);

    return std::vector<std::pair<std::string, float>>(top.begin(), top.begin() + 5);
}

std::string textMatch(std::string path, std::string query) {
    std::vector<std::string> words{};
    auto wBeg = query.begin();
    auto wEnd = query.end();

    WordIter<std::string::iterator> wItr(wBeg, wEnd);

    for (auto w: wItr) {
        words.push_back(w.word);
    }

    std::ifstream reader(path);
    int start = 0;
    int bestMatch = 0;
    int curMatch = 0;
    int bestSegment = 0;

    auto fileBeg = std::istreambuf_iterator<char>(reader);
    auto fileEnd = std::istreambuf_iterator<char>();

    WordIter<std::istreambuf_iterator<char>> fileItr(fileBeg, fileEnd);

    for (auto w: fileItr) {
        std::string word = w.word;

        if (find(words.begin(), words.end(), word) <= words.end()) {
            curMatch++;
        }
        if (fileItr.offset - start > 1000) {
            if (curMatch > bestMatch) {
                bestMatch = curMatch;
                bestSegment = start;
            }
            start = fileItr.offset;
            curMatch = 0;
        }

        ++fileItr;
    }

    std::string result;
    reader.seekg(bestSegment, std::ios_base::beg);

    for (int idx = 0; idx < 1000; idx++) {
        if (reader.eof()) break;
        result += reader.get();
    }

    reader.close();

    return result;
}

std::string cleanText(std::string text) {
    std::string res;

    for (int idx = 0; idx < text.length(); idx++) {
        char c = text[idx];

        if (c == 0 || isalnum(c)) {
            res.push_back(c);
        }
    }

    return res;
}

