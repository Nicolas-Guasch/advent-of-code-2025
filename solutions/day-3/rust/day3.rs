use std::fs;

fn max_joltage(battery_bank: &str) -> i64 {
    let joltages: Vec<i64> = battery_bank
        .chars()
        .map(|c| c.to_digit(10).expect("Failed to parse digit") as i64)
        .collect();
    let max_prefixes = joltages.iter().scan(0, |acc, &x| {
        *acc = (*acc).max(x);
        Some(*acc)
    });
    joltages
        .iter()
        .skip(1)
        .zip(max_prefixes)
        .map(|(d, p)| p * 10 + d)
        .max()
        .expect("Failed to find max joltage")
}

fn part1(input: &[&str]) -> i64 {
    input.iter().map(|&s| max_joltage(s)).sum()
}

fn max_joltage_position(values: &[i64], pos: usize, window: usize) -> usize {
    values[pos..pos + window]
        .iter()
        .enumerate()
        .max_by_key(|(i, &x)| (x, std::cmp::Reverse(*i)))
        .expect("Failed to find max joltage position")
        .0
        + pos
}

const DIGITS: usize = 12;

fn greedy(battery_bank: &str) -> i64 {
    let joltages: Vec<i64> = battery_bank
        .chars()
        .map(|c| c.to_digit(10).expect("Failed to parse digit") as i64)
        .collect();

    (1..=DIGITS)
        .rev()
        .fold((0, 0), |(pos, current_joltage), d| {
            let window = joltages.len() - pos - d + 1;
            let next_digit = max_joltage_position(&joltages, pos, window);
            (
                next_digit + 1,
                current_joltage * 10 + joltages[next_digit] as i64,
            )
        })
        .1
}

fn part2(input: &[&str]) -> i64 {
    input.iter().map(|&s| greedy(s)).sum()
}

fn main() {
    let file_name = "day3.in";
    let content = fs::read_to_string(file_name).expect("Failed to read input file");
    let input = content.lines().collect::<Vec<&str>>();
    println!("Part 1: {}", part1(&input));
    println!("Part 2: {}", part2(&input));
}
