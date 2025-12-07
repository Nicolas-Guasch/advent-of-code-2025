#include <algorithm>
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

struct Grid {
    std::vector<std::string> data;

    Grid(std::vector<std::string> _data) : data(std::move(_data)) {}

    auto operator[](this auto &&self, int i, int j) -> decltype(auto) {
        return self.data[i][j];
    }

    bool inBounds(int i, int j) const {
        return 0 <= i && i < ssize(data) && 0 <= j && j < ssize(data[i]);
    }

    auto positions() const {
        return std::views::cartesian_product(
            std::views::iota(0, static_cast<int>(ssize(data))),
            std::views::iota(0, static_cast<int>(ssize(data[0]))));
    }

    auto neighbors(int i, int j) const {
        return std::views::iota(0, 8) | std::views::transform([=](int k) {
                   return std::pair{i + DELTA_I[k], j + DELTA_J[k]};
               }) |
               std::views::filter([this](auto pos) {
                   return inBounds(pos.first, pos.second);
               });
    }
};
int countAdjacentRolls(int i, int j, const Grid &diagram) {
    int rolls = 0;
    for (auto [ii, jj] : diagram.neighbors(i, j)) {
        rolls += diagram[ii, jj] == '@';
    }
    return rolls;
}

bool accessible(int i, int j, const Grid &diagram) {
    return countAdjacentRolls(i, j, diagram) < 4;
}

bool just_unlocked(int i, int j, const Grid &diagram) {
    return countAdjacentRolls(i, j, diagram) == 3;
}

std::string part1(std::ifstream &inputFile) {
    Grid diagram = {std::views::istream<std::string>(inputFile) |
                    std::ranges::to<std::vector>()};
    auto accessibleRolls =
        std::ranges::count_if(diagram.positions(), [&](auto pos) {
            auto [i, j] = pos;
            return diagram[i, j] == '@' && accessible(i, j, diagram);
        });
    return std::format("There are {} rolls of paper accessible by forklift.",
                       accessibleRolls);
}

std::string part2(std::ifstream &inputFile) {
    Grid diagram = {std::views::istream<std::string>(inputFile) |
                    std::ranges::to<std::vector>()};

    int accessibleRolls = 0;
    std::queue<std::pair<int, int>> toRemove;
    for (auto [i, j] : diagram.positions()) {
        if (diagram[i, j] == '@' && accessible(i, j, diagram))
            toRemove.emplace(i, j);
    }

    while (!toRemove.empty()) {
        accessibleRolls++;
        auto [i, j] = toRemove.front();
        toRemove.pop();
        diagram[i, j] = '.';
        for (auto [ii, jj] : diagram.neighbors(i, j)) {
            if (diagram[ii, jj] == '@' && just_unlocked(ii, jj, diagram))
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
    inputFile.clear();
    inputFile.seekg(0);
    std::cout << part2(inputFile) << std::endl;
}