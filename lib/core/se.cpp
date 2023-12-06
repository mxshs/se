#include <algorithm>
#include <cctype>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "se.h"

using namespace std;
using namespace se;

Cache::Cache(string path):
    updateStamp(time(0)),
    Total(map<string, int>()),
    Loc(map<string, vector<string>>())
{
    this->updateCache(path);
}

int Cache::getTotalFrequency(string word) {
    if (this->Total.count(word) > 0) {
        return this->Total[word];
    }

    return 0;
}

vector<string> Cache::getLocations(string word) {
    if (this->Loc.count(word) > 0) {
        return this->Loc[word];
    }

    return vector<string>();
}

void Cache::updateCache(string path) {
    //if (difftime(time(0), this->updateStamp) < 360) return;

    this->scanAllDocs(path);

    this->updateStamp = time(0);
}

void Cache::scanAllDocs(string dir) {

    for (const filesystem::directory_entry &entry:
            filesystem::recursive_directory_iterator(dir)) {
        this->cacheFile(entry);
    }
}

void Cache::cacheFile(filesystem::directory_entry file) {
    ifstream reader;

    reader.open(file.path());

    set<string> seen;
    string word;

    while (!reader.eof()) {
        char c = reader.get();

        if (c == 0 || !isalnum(c)) {
            if (word.length() > 0) {
                if (seen.count(word) > 0) {
                    if (this->Total.count(word) > 0) {
                        this->Total[word] += 1;
                        this->Loc[word].push_back(file.path());
                    } else {
                        this->Total[word] = 1;
                        this->Loc[word] = vector<string>({file.path()});
                    }
                }

                seen.insert(word);
            }

            word = "";
        } else if (isalnum(c)) {
            word += c;
        }
    }
}

SE::SE(Cache cache): cache(cache) {};

bool tfidfComparator(const pair<string, float> &a,
        const pair<string, float> &b) {
    return (a.second > b.second);
}

vector<pair<string, float>> SE::search(string input) {
    string word;
    map<string, float> result;

    for (int idx = 0; idx < input.length(); idx++) {
        char c = input[idx];
        
        if (c == 0 || !isalnum(c)) {
            if (word.length() > 0) {
                for (string path: this->cache.getLocations(word)) {
                    float tfidf = this->TFIDF(word, path);
                    if (result.count(path) > 0) result[path] += tfidf;
                    else result[path] = tfidf;
                }
            }

            word = "";
        } else if (isalnum(c)) {
            word += c;
        }
    }

    vector<pair<string, float>> top;

    for (auto const &[key, val]: result) {
        top.push_back(pair<string, float>(key, val));
    }

    sort(top.begin(), top.end(), tfidfComparator);

    return vector<pair<string, float>>(top.begin(), top.begin() + 5);
}

float SE::TFIDF(string term, string path) {
    ifstream reader;
    float counter;
    string word;

    reader.open(path);

    while (!reader.eof()) {
        char c = reader.get();

        if (c == 0 || !isalnum(c)) {
            if (word.length() > 0) {
                if (word == term) counter++;
            }

            word = "";
        } else if (isalnum(c)) {
            word += c;
        }
    }

    return counter / this->cache.getTotalFrequency(term);
}

