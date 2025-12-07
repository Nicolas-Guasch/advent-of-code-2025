#include <algorithm>
#include <climits>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

auto splitOnce(std::string_view s, std::string_view delim) {
    size_t splitPos = s.find(delim);
    return std::pair{s.substr(0, splitPos), s.substr(splitPos + delim.size())};
}

auto parseLLI(std::string_view s) {
    long long int result = 0;
    std::from_chars(s.data(), s.data() + s.size(), result);
    return result;
}

auto parseRanges(std::string_view input) {
    return input | std::views::split('\n') |
           std::views::transform([](auto line) {
               auto [l, r] = splitOnce(std::string_view(line), "-");
               return std::pair{parseLLI(l), parseLLI(r)};
           }) |
           std::ranges::to<std::vector>();
}

auto parseIds(std::string_view input) {
    return input | std::views::split('\n') |
           std::views::transform(
               [](auto line) { return parseLLI(std::string_view(line)); }) |
           std::ranges::to<std::vector>();
}

std::string part1(std::string_view input) {
    auto [rangeInput, IdInput] = splitOnce(input, "\n\n");
    auto freshIds = parseRanges(rangeInput);
    auto ids = parseIds(IdInput);

    std::ranges::sort(freshIds);
    std::ranges::sort(ids);

    auto idPos = 0z;
    int fresh = 0;
    for (auto [l, r] : freshIds) {
        if (idPos == std::ssize(ids))
            break;
        while (idPos < std::ssize(ids) && ids[idPos] < l)
            idPos++;
        while (idPos < std::ssize(ids) && ids[idPos] <= r) {
            idPos++;
            fresh++;
        }
    }
    return std::format("{} of the available ingredient IDs are fresh", fresh);
}

std::string part2(std::string_view input) {
    auto [rangeInput, _] = splitOnce(input, "\n\n");
    std::vector<std::pair<long long int, long long int>> freshIds =
        parseRanges(rangeInput);
    std::ranges::sort(freshIds);

    auto [freshCount, maxId] = std::ranges::fold_left(
        freshIds, std::pair{0LL, std::numeric_limits<long long int>::min()},
        [](auto acc, auto range) {
            auto [count, maxSeen] = acc;
            auto [l, r] = range;
            if (r > maxSeen) {
                l = std::max(l, maxSeen + 1);
                count += r - l + 1;
                maxSeen = r;
            }
            return std::pair{count, maxSeen};
        });
    return std::format("There are {} fresh ingredient IDs", freshCount);
}

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

    std::ostringstream buffer;
    buffer << inputFile.rdbuf();
    std::string input = buffer.str();
    std::println("{}", part1(input));
    std::println("{}", part2(input));
}