#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Grid {
  private:
    static constexpr int DELTA_I[8] = {-1, -1, -1, +0, +1, +1, +1, +0},
                         DELTA_J[8] = {-1, +0, +1, +1, +1, +0, -1, -1};

    std::vector<std::string> data;

  public:
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

    int height() const { return static_cast<int>(ssize(data)); }

    int width() const { return static_cast<int>(ssize(data[0])); }
};

struct Graph {
  private:
    int lastId = -1;
    int width;
    std::map<int, std::vector<int>> edges;
    std::vector<int> diagramSkyline;

    void dfs(int node, std::unordered_set<int> &visited) {
        visited.insert(node);
        for (auto neighbor : edges[node]) {
            if (!visited.contains(neighbor)) {
                dfs(neighbor, visited);
            }
        }
    }

    void dagSum(int node, std::map<int, long long int> &subTreeSize) {
        subTreeSize[node] = 2 - ssize(edges[node]);
        for (auto neighbor : edges[node]) {
            if (!subTreeSize.contains(neighbor)) {
                dagSum(neighbor, subTreeSize);
            }
            subTreeSize[node] += subTreeSize[neighbor];
        }
    }

  public:
    Graph(int _n) : width(_n) { diagramSkyline = std::vector<int>(width, -1); }

    void addNode(int j) {
        diagramSkyline[j] = ++lastId;
        auto neighbors = {j > 0 ? diagramSkyline[j - 1] : -1,
                          j < width - 1 ? diagramSkyline[j + 1] : -1};
        edges[lastId] = neighbors |
                        std::views::filter([](auto id) { return id != -1; }) |
                        std::ranges::to<std::vector>();
    }

    std::unordered_set<int> dfs(int startJ) {
        std::unordered_set<int> visited;
        dfs(diagramSkyline[startJ], visited);
        return visited;
    }

    long long int dagSum(int startJ) {
        std::map<int, long long int> subTreeSize;
        int root = diagramSkyline[startJ];
        dagSum(root, subTreeSize);
        return subTreeSize[root];
    }
};

auto parse(std::string_view input) {
    Grid diagram = Grid(input | std::views::split('\n') |
                        std::views::transform([](auto line) {
                            return std::string(std::string_view(line));
                        }) |
                        std::ranges::to<std::vector>());
    Graph graph = std::ranges::fold_right(
        diagram.positions() | std::views::filter([&diagram](auto pos) {
            auto [i, j] = pos;
            return diagram[i, j] == '^';
        }),
        Graph(diagram.width()), [](auto pos, auto acc) {
            auto [i, j] = pos;
            acc.addNode(j);
            return acc;
        });

    auto positions = diagram.positions();
    auto it = std::ranges::find_if(positions, [&diagram](auto pos) {
        auto [i, j] = pos;
        return diagram[i, j] == 'S';
    });
    auto [_, startCol] = *it;
    return std::pair{std::move(graph), startCol};
}

std::string part1(std::string_view input) {
    auto [graph, startCol] = parse(input);
    long long int result = graph.dfs(startCol).size();
    return std::format(
        "The beam will be split {} times in the manifold diagram.", result);
}

std::string part2(std::string_view input) {
    auto [graph, startCol] = parse(input);
    long long int result = graph.dagSum(startCol);
    return std::format("Following the many-worlds interpretation, the tachyon "
                       "would end up on {} different timelines.",
                       result);
}

int main() {
    namespace fs = std::filesystem;
    fs::path inputFileName = "day7.in";

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