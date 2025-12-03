#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>

std::string part1(std::ifstream &inputFile) {
    int totalJoltage = 0;
    for (const auto &batteryBank :
         std::views::istream<std::string>(inputFile)) {
        int maxJoltage = 0;
        int highestBattery = batteryBank[0] - '0';
        for (int i = 1; i < std::ssize(batteryBank); i++) {
            int batteryJoltage = batteryBank[i] - '0';
            int producedJoltage = highestBattery * 10 + batteryJoltage;
            maxJoltage = std::max(maxJoltage, producedJoltage);
            highestBattery = std::max(highestBattery, batteryJoltage);
        }
        totalJoltage += maxJoltage;
    }
    return std::format(
        "The total output joltage with two batteries per bank is: {}",
        totalJoltage);
}

constexpr int DIGITS = 12;

int maxJoltagePosition(std::string_view batteryBank, int pos, int window) {
    auto subrange = batteryBank.substr(pos, window);
    auto it = std::ranges::max_element(subrange);
    return pos + std::distance(subrange.begin(), it);
}

long long int greedy(std::string_view batteryBank) {
    long long int result = 0;
    int digits = DIGITS;
    int pos = 0;
    while (digits) {
        int window = (std::ssize(batteryBank) - pos) - digits + 1;
        int bestPos = maxJoltagePosition(batteryBank, pos, window);
        result = result * 10 + batteryBank[bestPos] - '0';
        digits--;
        pos = bestPos + 1;
    }
    return result;
}

std::string part2(std::ifstream &inputFile) {
    long long int totalJoltage = 0;
    for (const auto &batteryBank :
         std::views::istream<std::string>(inputFile)) {
        totalJoltage += greedy(batteryBank);
    }
    return std::format(
        "The total output joltage with twelve battery per bank is: {}",
        totalJoltage);
}

int main() {
    namespace fs = std::filesystem;
    fs::path inputFileName = "day3.in";

    if (!fs::exists(inputFileName)) {
        std::cerr << std::format("File {} does not exist.",
                                 inputFileName.string())
                  << std::endl;
        return 1;
    }

    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << std::format("Couldn't open {}.", inputFileName.string())
                  << std::endl;
        return 1;
    }

    std::cout << part1(inputFile) << std::endl;
    inputFile.close();
    inputFile.open(inputFileName);
    if (!inputFile) {
        std::cerr << std::format("Couldn't open {}.", inputFileName.string())
                  << std::endl;
        return 1;
    }
    std::cout << part2(inputFile) << std::endl;
    return 0;
}
