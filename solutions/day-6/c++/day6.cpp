#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

std::string part1(std::string_view input) {
    std::vector<std::string_view> mathLines =
        input | std::views::split('\n') |
        std::views::transform(
            [](auto &&line) { return std::string_view(line); }) |
        std::ranges::to<std::vector>();
    std::ranges::reverse(mathLines);

    std::vector<char> operators;
    operators.reserve(mathLines[0].size());
    std::vector<std::vector<long long int>> operands;
    operands.reserve(mathLines[0].size());
    for (const auto &[index, symbol] : mathLines[0] | std::views::enumerate |
                                           std::views::filter([](auto &&pair) {
                                               return std::get<1>(pair) != ' ';
                                           })) {
        operators.push_back(symbol);
        operands.push_back(std::vector<long long int>());
        std::vector<long long int> &currentProblem = operands.back();

        auto nextOperatorIndex = index;
        while (nextOperatorIndex + 1LL == std::ssize(mathLines[0]) ||
               (nextOperatorIndex + 1LL < std::ssize(mathLines[0]) &&
                mathLines[0][nextOperatorIndex + 1LL] == ' ')) {
            nextOperatorIndex++;
        }
        for (auto operandLine :
             mathLines | std::views::drop(1) |
                 std::views::transform([index, nextOperatorIndex](auto &&line) {
                     return line | std::views::drop(index) |
                            std::views::take(nextOperatorIndex - index);
                 })) {
            int operandValue = 0;
            for (auto digit : operandLine) {
                if (digit != ' ') {
                    operandValue = operandValue * 10 + (digit - '0');
                }
            }
            currentProblem.push_back(operandValue);
        }
    }
    long long int grandTotal = std::ranges::fold_left(
        std::views::zip_transform(
            [](auto op, auto values) {
                return std::ranges::fold_left(
                    values, op == '+' ? 0 : 1,
                    [op](long long int acc, long long int val) {
                        return op == '+' ? acc + val : acc * val;
                    });
            },
            operators, operands),
        0LL, std::plus<long long int>());

    return std::format("The grand total for this worksheet is {}.", grandTotal);
}

std::string part2(std::string_view input) {
    std::vector<std::string_view> mathLines =
        input | std::views::split('\n') |
        std::views::transform(
            [](auto &&line) { return std::string_view(line); }) |
        std::ranges::to<std::vector>();
    std::ranges::reverse(mathLines);

    std::vector<char> operators;
    operators.reserve(mathLines[0].size());
    std::vector<std::vector<long long int>> operands;
    operands.reserve(mathLines[0].size());
    for (const auto &[index, symbol] : mathLines[0] | std::views::enumerate |
                                           std::views::filter([](auto &&pair) {
                                               return std::get<1>(pair) != ' ';
                                           })) {
        operators.push_back(symbol);
        operands.push_back(std::vector<long long int>());
        std::vector<long long int> &currentProblem = operands.back();

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
        for (const auto &[a, b, c, d] :
             std::views::zip(operandLines[3], operandLines[2], operandLines[1],
                             operandLines[0])) {
            int operandValue = 0;
            for (auto digit : {a, b, c, d}) {
                if (digit != ' ') {
                    operandValue = operandValue * 10 + (digit - '0');
                }
            }
            currentProblem.push_back(operandValue);
        }
    }
    long long int grandTotal = std::ranges::fold_left(
        std::views::zip_transform(
            [](auto op, auto values) {
                return std::ranges::fold_left(
                    values, op == '+' ? 0 : 1,
                    [op](long long int acc, long long int val) {
                        return op == '+' ? acc + val : acc * val;
                    });
            },
            operators, operands),
        0LL, std::plus<long long int>());

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