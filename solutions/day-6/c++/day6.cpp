#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct Problem {
    char op;
    std::vector<long long int> operands;
};

std::vector<Problem> partitionProblems(std::string_view input, bool vertical) {
    std::vector<std::string_view> mathLines =
        input | std::views::split('\n') |
        std::views::transform(
            [](auto &&line) { return std::string_view(line); }) |
        std::ranges::to<std::vector>();
    std::ranges::reverse(mathLines);

    std::vector<Problem> problems;
    problems.reserve(mathLines[0].size() -
                     std::ranges::count(mathLines[0], ' '));
    for (const auto &[index, symbol] : mathLines[0] | std::views::enumerate |
                                           std::views::filter([](auto &&pair) {
                                               return std::get<1>(pair) != ' ';
                                           })) {
        problems.emplace_back(symbol, std::vector<long long int>());
        std::vector<long long int> &currentProblem = problems.back().operands;

        auto nextOperatorIndex = index;
        while (nextOperatorIndex + 1LL == std::ssize(mathLines[0]) ||
               (nextOperatorIndex + 1LL < std::ssize(mathLines[0]) &&
                mathLines[0][nextOperatorIndex + 1LL] == ' ')) {
            nextOperatorIndex++;
        }
        auto operandLines =
            mathLines | std::views::drop(1) |
            std::views::transform([index, nextOperatorIndex](auto &&line) {
                return line | std::views::drop(index) |
                       std::views::take(nextOperatorIndex - index);
            }) |
            std::ranges::to<std::vector>();
        if (vertical) {
            long long int numLines = operandLines.size();
            for (auto pos : std::views::iota(0, std::ssize(operandLines[0]))) {
                long long int operandValue = 0;
                for (auto line : std::views::iota(0LL, numLines)) {
                    auto digit = operandLines[numLines - 1 - line][pos];
                    if (digit != ' ') {
                        operandValue = operandValue * 10 + (digit - '0');
                    }
                }
                currentProblem.push_back(operandValue);
            }
        } else {

            for (auto operandLine : operandLines) {
                long long int operandValue = 0;
                for (auto digit : operandLine) {
                    if (digit != ' ') {
                        operandValue = operandValue * 10 + (digit - '0');
                    }
                }
                currentProblem.push_back(operandValue);
            }
        }
    }
    return problems;
}

long long int evaluateProblem(const Problem &problem) {
    char op = problem.op;
    return std::ranges::fold_left(problem.operands, op == '+' ? 0 : 1,
                                  [op](long long int acc, long long int val) {
                                      return op == '+' ? acc + val : acc * val;
                                  });
}

long long int calculateGrandTotal(const std::vector<Problem> &problems) {
    return std::ranges::fold_left(problems |
                                      std::views::transform(evaluateProblem),
                                  0LL, std::plus<long long int>());
}

std::string part1(std::string_view input) {
    auto problems = partitionProblems(input, false);
    long long int grandTotal = calculateGrandTotal(problems);

    return std::format("The grand total for this worksheet is {}.", grandTotal);
}

std::string part2(std::string_view input) {
    auto problems = partitionProblems(input, true);
    long long int grandTotal = calculateGrandTotal(problems);

    return std::format("The grand total for this worksheet, with right to left "
                       "column evaluation, is {}.",
                       grandTotal);
}

int main() {
    namespace fs = std::filesystem;
    fs::path inputFileName = "day6.in";

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
    return 0;
}