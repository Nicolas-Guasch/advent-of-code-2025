#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

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

int maxJoltagePosition(std::string &batteryBank, int pos, int window) {
    int maxJoltage = 0;
    int maxPos = -1;
    for (int i = pos; i < pos + window; i++) {
        int joltage = batteryBank[i] - '0';
        if (joltage > maxJoltage) {
            maxJoltage = joltage;
            maxPos = i;
        }
    }
    return maxPos;
}

long long int greedy(std::string &batteryBank) {
    long long int result = 0;
    int digits = DIGITS;
    int pos = 0;
    while (digits) {
        int window = ((int)batteryBank.size() - pos) - digits + 1;
        int bestPos = maxJoltagePosition(batteryBank, pos, window);
        result = result * 10 + batteryBank[bestPos] - '0';
        digits--;
        pos = bestPos + 1;
    }
    return result;
}

std::string part2(std::ifstream &inputFile) {
    long long int totalJoltage = 0;
    std::string batteryBank;
    while (inputFile >> batteryBank) {
        totalJoltage += greedy(batteryBank);
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
