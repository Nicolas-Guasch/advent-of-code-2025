// Solution for day 2 of the Advent of Code 2025
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

constexpr long long int MAXID = 10000017929LL;
constexpr int MAXDIGITS = 11;
constexpr int MAXPATTERN = 99999 + 1;

struct interval {
    long long int a;
    long long int b;
};

long long int viewToLLI(std::string_view sv) {
    long long int result = 0;
    std::from_chars(sv.data(), sv.data() + sv.size(), result);
    return result;
}

void parseIntervals(std::ifstream &inputFile,
                    std::vector<interval> &intervals) {
    std::string line;
    std::getline(inputFile, line);

    using std::operator""sv;
    constexpr auto delim{","sv};
    constexpr auto dash{"-"sv};

    for (const auto subrange : std::views::split(line, delim)) {
        std::string_view token_view(subrange);
        std::vector<long long int> parts =
            std::views::split(token_view, dash) |
            std::views::transform(
                [](auto &&pair) { return viewToLLI(std::string_view(pair)); }) |
            std::ranges::to<std::vector>();

        intervals.emplace_back(parts[0], parts[1]);
    }
};

long long int findMaxId(std::ifstream &inputFile) {
    std::vector<interval> intervals;
    parseIntervals(inputFile, intervals);
    long long int maxId =
        std::max_element(intervals.begin(), intervals.end(),
                         [](auto &&i1, auto &&i2) { return i1.b < i2.b; })
            ->b;
    return maxId;
}

bool included(long long int id, std::vector<interval> &intervals) {
    return std::any_of(
        intervals.begin(), intervals.end(),
        [id](auto &&interval) { return id >= interval.a && id <= interval.b; });
}

std::string part1(std::ifstream &inputFile) {
    std::vector<interval> intervals;
    parseIntervals(inputFile, intervals);

    std::sort(intervals.begin(), intervals.end(),
              [](auto &&i1, auto &&i2) { return i1.a < i2.a; });

    long long int invalidSum = 0;
    for (int i = 0; i < MAXPATTERN; i++) {
        std::string digits = std::to_string(i);
        long long int invalidId = std::stoll(digits + digits);
        if (included(invalidId, intervals)) {
            invalidSum += invalidId;
        }
    }

    return std::format("Sum of invalid IDs: {}", invalidSum);
}

long long int repeatDigits(long long int id, int times) {
    std::string digits = std::to_string(id);
    std::string invalidId = "";
    while (times--) {
        invalidId += digits;
    }
    if (invalidId.size() > MAXDIGITS) {
        return LLONG_MAX;
    }
    return std::stoll(invalidId);
}

std::string part2(std::ifstream &inputFile) {
    std::vector<interval> intervals;
    parseIntervals(inputFile, intervals);

    std::sort(intervals.begin(), intervals.end(),
              [](auto &&i1, auto &&i2) { return i1.a < i2.a; });

    std::set<long long int> invalidIds = {};
    for (int t = 2; t < MAXDIGITS; t++) {
        for (int i = 0; repeatDigits(i, t) <= MAXID; i++) {
            long long int invalidId = repeatDigits(i, t);
            if (included(invalidId, intervals)) {
                invalidIds.insert(invalidId);
            }
        }
    }

    return std::format(
        "Sum of invalid IDs, : {}",
        std::accumulate(invalidIds.begin(), invalidIds.end(), 0LL));
}

int main() {
    std::string inputFileName = "day2.in";
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open " << inputFileName << "." << std::endl;
        return 1;
    }
    std::cout << part1(inputFile) << std::endl;
    inputFile.close();
    inputFile.open(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open " << inputFileName << "." << std::endl;
        return 1;
    }

    std::cout << part2(inputFile) << std::endl;

    return 0;
}
