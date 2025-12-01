#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string inputFileName = "day1.in";
    std::ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Couldn't open " << inputFileName << "." << std::endl;
        return 1;
    }

    int dialPosition = 50;
    int password = 0;
    #define DIALLEN 100
    std::string line;
    while (inputFile >> line) {
        int rotation  = (line[0] == 'L' ? -1 : 1) * std::stoi(line.substr(1));
        password += 
            (dialPosition + rotation <= 0) ?
            (dialPosition != 0) - (dialPosition + rotation) / DIALLEN:
            (dialPosition + rotation) / DIALLEN;
        dialPosition = ((dialPosition + rotation) % DIALLEN + DIALLEN) % DIALLEN;
    }

    std::cout << "Door password: " << password << std::endl;
    
    return 0;
}
