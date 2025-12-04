#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <ranges>
#include <vector>

constexpr int DELTA_I[8] = {-1, -1, -1, +0, +1, +1, +1, +0},
              DELTA_J[8] = {-1, +0, +1, +1, +1, +0, -1, -1};
int countAdjacentRolls(int i, int j, std::vector<std::string> &diagram) {
    int rolls = 0;
    for (int k = 0; k < 8; k++) {
        int ii = i + DELTA_I[k], jj = j + DELTA_J[k];
        rolls += 0 <= ii && ii < ssize(diagram) && 0 <= jj &&
                 jj < ssize(diagram) && diagram[ii][jj] == '@';
    }
    return rolls;
}

bool accessible(int i, int j, std::vector<std::string> &diagram) {
    return countAdjacentRolls(i, j, diagram) < 4;
}

bool just_unlocked(int i, int j, std::vector<std::string> &diagram) {
    return countAdjacentRolls(i, j, diagram) == 3;
}

std::string part1(std::ifstream &inputFile) {
    std::vector<std::string> diagram =
        std::views::istream<std::string>(inputFile) |
        std::ranges::to<std::vector>();
    int accessibleRolls = 0;
    for (int i = 0; i < std::ssize(diagram); i++) {
        for (int j = 0; j < std::ssize(diagram[i]); j++) {
            accessibleRolls +=
                diagram[i][j] == '@' && accessible(i, j, diagram);
        }
    }
    return std::format("There are {} rolls of paper accessible by forklift.",
                       accessibleRolls);
}

struct pos {
    int i;
    int j;
};

std::string part2(std::ifstream &inputFile) {
    std::vector<std::string> diagram =
        std::views::istream<std::string>(inputFile) |
        std::ranges::to<std::vector>();

    int accessibleRolls = 0;
    std::queue<pos> toRemove;
    for (int i = 0; i < ssize(diagram); i++) {
        for (int j = 0; j < ssize(diagram); j++) {
            if (diagram[i][j] == '@' && accessible(i, j, diagram))
                toRemove.emplace(i, j);
        }
    }

    while (!toRemove.empty()) {
        accessibleRolls++;
        auto [i, j] = toRemove.front();
        toRemove.pop();
        diagram[i][j] = '.';
        for (int k = 0; k < 8; k++) {
            int ii = i + DELTA_I[k], jj = j + DELTA_J[k];
            if (0 <= ii && ii < ssize(diagram) && 0 <= jj &&
                jj < ssize(diagram[i]) && diagram[ii][jj] == '@' &&
                just_unlocked(ii, jj, diagram))
                toRemove.emplace(ii, jj);
        }
    }

    return std::format(
        "There are {} rolls of paper accessible by forklift with removal.",
        accessibleRolls);
}

int main() {
    namespace fs = std::filesystem;
    fs::path inputFileName = "day4.in";

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