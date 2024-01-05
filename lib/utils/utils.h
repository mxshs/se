#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

template<std::input_iterator T>
class WordIter {
    public:
        WordIter(T itr, T end): iter(itr), iend(end), offset(0) {};

        std::string word;
        int offset;

        WordIter& operator*() {
            return *this;
        }

        WordIter& operator++() {
            forward();

            return *this;
        }

        WordIter operator++(int) {
            WordIter temp = *this;

            forward();

            return temp;
        }

        bool operator==(const WordIter& other) const  {
            return iter == other.iter;
        }

        bool operator!=(const WordIter& other) const  {
            return iter != other.iter;
        }

        WordIter begin() const {
            return *this;
        }

        WordIter end() const {
            return WordIter(iend, iend);
        }

    private:
        T iter;
        T iend;

        std::vector<std::string> percentEncoded = {
            "%20",
            "%21",
            "%22",
            "%23",
            "%24",
            "%25",
            "%26",
            "%27",
            "%28",
            "%29",
            "%2A",
            "%2B",
            "%2C",
            "%2F",
            "%3a",
            "%3B",
            "%3D",
            "%3F",
            "%40",
            "%5B",
            "%5D",
        };

        bool valid() {
            return (!isspace(*iter) && (isalnum(*iter) || *iter == '_' || *iter == '-' || *iter == '.'));
        }

        void skipPercentEncoded() {
            if (*iter != '%') return;

            T temp = iter++;

            if (iter == iend) {
                return;
            }
           
            std::string combination = std::string("%") + *iter;
            ++iter, combination += *iter;

            if (
                find(percentEncoded.begin(), percentEncoded.end(), combination) 
                != percentEncoded.end()
            ) {
                ++iter;
                offset += 4;
                
                return;
            }

            iter = temp;
            offset += 1;
        }

        void escape() {
            while (iter != iend && !isalnum(*iter)) {
                if (*iter == '%') skipPercentEncoded();
                else {
                    offset += 1;
                    ++iter;
                }
            }
        }

        void forward() {
            word = "";

            while (iter != iend && valid()) {
                word += *iter;
                ++iter;
                offset += 1;
            }

            escape();
        }
};

std::string formatIndent(std::vector<std::string>, int);
