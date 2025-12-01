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

    //std::vector<int> rotations;
    int dialPosition = 50;
    int password = 0;
    #define DIALLEN 100
    std::string line;
    while (inputFile >> line) {
        int rotation  = (line[0] == 'L' ? -1 : 1) * std::stoi(line.substr(1));
        //rotations.push_back(rotation);
        dialPosition = ((dialPosition + rotation) % DIALLEN + DIALLEN) % DIALLEN;
        if (!dialPosition) password++;
    }

    std::cout << "Door password: " <<password << std::endl;
    
    return 0;
}
