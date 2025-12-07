#include <fstream>
#include <iostream>
#include <string>

constexpr int DIALLEN = 100;

std::string part1(std::ifstream &inputFile) {
    int dialPosition = 50;
    int password = 0;
    std::string line;
    while (inputFile >> line) {
        int rotation = (line[0] == 'L' ? -1 : 1) * std::stoi(line.substr(1));
        dialPosition =
            ((dialPosition + rotation) % DIALLEN + DIALLEN) % DIALLEN;
        if (!dialPosition)
            password++;
    }

    return std::format("Door password: {}", password);
}

std::string part2(std::ifstream &inputFile) {
    int dialPosition = 50;
    int password = 0;
    std::string line;
    while (inputFile >> line) {
        int rotation = (line[0] == 'L' ? -1 : 1) * std::stoi(line.substr(1));
        password +=
            (dialPosition + rotation <= 0)
                ? (dialPosition != 0) - (dialPosition + rotation) / DIALLEN
                : (dialPosition + rotation) / DIALLEN;
        dialPosition =
            ((dialPosition + rotation) % DIALLEN + DIALLEN) % DIALLEN;
    }

    return std::format("Door password using password method 0x434C49434B: {}",
                       password);
}

int main() {
    std::string inputFileName = "day1.in";
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile) {
        std::cerr << "Couldn't open " << inputFileName << "." << std::endl;
        return 1;
    }

    std::cout << part1(inputFile) << std::endl;
    inputFile.clear();
    inputFile.seekg(0);
    std::cout << part2(inputFile) << std::endl;

    return 0;
}
