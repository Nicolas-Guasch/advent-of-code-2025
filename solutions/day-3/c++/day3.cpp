#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string part1(std::ifstream &inputFile) {
    int totalJoltage = 0;
    std::string batteryBank;
    while (inputFile >> batteryBank) {
        int maxJoltage = 0;
        int highestBattery = batteryBank[0] - '0';
        for (int i = 1; i < (int)batteryBank.size(); i++) {
            int batteryJoltage = batteryBank[i] - '0';
            int producedJoltage = highestBattery * 10 + batteryJoltage;
            maxJoltage = std::max(maxJoltage, producedJoltage);
            highestBattery = std::max(highestBattery, batteryJoltage);
        }
        totalJoltage += maxJoltage;
    }
    std::stringstream ss;
    ss << "The total output joltage is: " << totalJoltage;
    return ss.str();
}

#define DIGITS 12

long long int dp(int size, int digits, std::string &batteryBank,
                 std::vector<std::vector<long long int>> &memo) {
    if (!digits)
        return 0;
    if (!size)
        return -1;
    if (memo[size][digits] != -2)
        return memo[size][digits];
    long long int &result = memo[size][digits] = -1;
    long long int joltage = batteryBank[size - 1] - '0';
    long long int off = dp(size - 1, digits, batteryBank, memo),
                  on = dp(size - 1, digits - 1, batteryBank, memo);
    if (on != -1)
        on = on * 10 + joltage;
    result = std::max(off, on);
    return result;
}

std::string part2(std::ifstream &inputFile) {
    long long int totalJoltage = 0;
    std::string batteryBank;
    while (inputFile >> batteryBank) {
        std::vector<std::vector<long long int>> memo(
            batteryBank.size() + 1, std::vector<long long int>(DIGITS + 1, -2));
        totalJoltage += dp((int)batteryBank.size(), DIGITS, batteryBank, memo);
    }
    std::stringstream ss;
    ss << "The total output joltage is: " << totalJoltage;
    return ss.str();
}
int main() {
    std::string inputFileName = "day3.in";
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open" << inputFileName << "." << std::endl;
        return 1;
    }

    std::cout << part1(inputFile) << std::endl;
    inputFile.close();
    inputFile.open(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open" << inputFileName << "." << std::endl;
        return 1;
    }
    std::cout << part2(inputFile) << std::endl;
    return 0;
}
