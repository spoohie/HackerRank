#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <vector>

struct Query {
    std::string scope;
    std::string attribute;

    Query(std::string line) {
        std::replace(line.begin(), line.end(), '~', ' ');
        std::stringstream stream(line);
        stream >> scope;
        stream >> attribute;
    }
};

class TagStack {
    std::deque<std::string> mStack;

public:
    void push(std::string tag) {
        mStack.push_back(tag);
    }

    void pop() {
        mStack.pop_back();
    }

    std::string getAsString() const {
        std::string s;
        for (const auto &tag : mStack) {
            s = s.empty() ? tag : s + "." + tag;
        }
        return s;
    }
};

class TagMap {
    const char END_SIGN = '/';

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mAttributes;
    TagStack mCurrentScope;

    void parseTag(std::string tag) {
        if (tag[1] == END_SIGN) {
            stepOut();
            return;
        }
        tag = tag.substr(1, tag.size() - 2);
        tag = std::regex_replace(tag, std::regex(" = "), " ");
        std::stringstream stream(tag);
        std::string currentTag;
        stream >> currentTag;

        std::unordered_map<std::string, std::string> attributes;
        std::string attrName, attrVal;
        while (stream >> attrName) {
            stream >> attrVal;
            attrVal = attrVal.substr(1, attrVal.size() - 2);
            attributes[attrName] = attrVal;
            attrName.clear(); attrVal.clear();
        }

        stepIn(currentTag);
        mAttributes[scope()] = attributes;
    }

    void stepIn(std::string tag) {
        mCurrentScope.push(tag);
    }

    void stepOut() {
        mCurrentScope.pop();
    }

    std::string scope() const {
        return mCurrentScope.getAsString();
        
    }

public:
    TagMap(const std::vector<std::string>& source) {
        for(auto& line : source) {
            parseTag(line);
        }
    }

    std::string getAttribute(const Query& q) const {
        auto it = mAttributes.find(q.scope);
        if (it != mAttributes.end()) {
            auto itInner = it->second.find(q.attribute);
            if (itInner != it->second.end()) {
                return itInner->second;
            }
        }
        return "Not Found!";
    }
};

std::tuple<int, int> getNumOfLines() {
    std::string firstLine;
    int numOfSrc, numOfQrs;
    std::getline(std::cin, firstLine);
    std::stringstream stream(firstLine);
    stream >> numOfSrc;
    stream >> numOfQrs;
    return std::make_tuple(numOfSrc, numOfQrs);
}

auto getLines(int numOfLines) {
    std::vector<std::string> lines;
    std::string line;
    for (int i = 0; i < numOfLines; i++) {
        std::getline(std::cin, line);
        lines.push_back(line);
    }
    return lines;
}

int main() {
    int numOfSrc, numOfQrs;
    std::tie(numOfSrc, numOfQrs) = getNumOfLines();
    std::vector<std::string> source = getLines(numOfSrc);
    std::vector<std::string> queries = getLines(numOfQrs);

    TagMap t = TagMap(source);

    for (auto& q : queries) {
        std::cout << t.getAttribute(Query(q)) << "\n";
    }
    return 0;
}