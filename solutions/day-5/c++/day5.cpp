#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::string, std::string> splitOnce(std::string s,
                                               std::string delim) {
    int splitPos = static_cast<int>(s.find(delim));
    return std::make_tuple(s.substr(0, splitPos),
                           s.substr(splitPos + ssize(delim)));
}

void splitLines(std::string &s, std::vector<std::string> &out) {
    out.reserve(std::count(s.begin(), s.end(), '\n'));
    int start = 0, end = static_cast<int>(s.find("\n"));
    while (start < ssize(s)) {
        out.push_back(s.substr(start, end - start));
        start = end + 1;
        end = static_cast<int>(s.find('\n', start));
    }
}

void parseRanges(std::string input,
                 std::vector<std::pair<long long int, long long int>> &out) {
    std::vector<std::string> lines = {};
    splitLines(input, lines);
    for (auto line : lines) {
        auto [l, r] = splitOnce(line, "-");
        out.emplace_back(std::stoll(l), std::stoll(r));
    }
}

void parseIds(std::string input, std::vector<long long int> &out) {
    std::vector<std::string> lines = {};
    splitLines(input, lines);
    for (auto line : lines) {
        out.push_back(std::stoll(line));
    }
}

std::string part1(std::ifstream &inputFile) {
    std::ostringstream buffer;
    buffer << inputFile.rdbuf();
    auto [rangeInput, IdInput] = splitOnce(buffer.str(), "\n\n");
    rangeInput += "\n";
    std::vector<std::pair<long long int, long long int>> freshIds = {};
    std::vector<long long int> ids = {};
    parseRanges(rangeInput, freshIds);
    parseIds(IdInput, ids);

    std::sort(freshIds.begin(), freshIds.end());
    std::sort(ids.begin(), ids.end());

    int idPos = 0, fresh = 0;
    for (auto [l, r] : freshIds) {
        if (idPos == ssize(ids))
            break;
        while (idPos < ssize(ids) && ids[idPos] < l)
            idPos++;
        while (idPos < ssize(ids) && ids[idPos] <= r) {
            idPos++;
            fresh++;
        }
    }
    return std::format("{} of the available ingredient IDs are fresh", fresh);
}

std::string part2(std::ifstream &inputFile) { return "Part 2"; }

int main() {
    namespace fs = std::filesystem;
    fs::path inputFileName = "day5.in";

    if (!fs::exists(inputFileName)) {
        std::cerr << std::format("File {} does not exist.",
                                 inputFileName.string())
                  << std::endl;
        return 1;
    }

    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << std::format("Couldn't open {}", inputFileName.string())
                  << std::endl;
        return 1;
    }

    std::cout << part1(inputFile) << std::endl;

    inputFile.close();
    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << std::format("Couldn't open {}", inputFileName.string())
                  << std::endl;
        return 1;
    }

    std::cout << part2(inputFile) << std::endl;
}