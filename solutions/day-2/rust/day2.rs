use std::{collections::HashSet, fs};

fn upper_bound(intervals: &[(i64, i64)]) -> i64 {
    intervals
        .iter()
        .map(|&(_a, b)| b)
        .max()
        .expect("Failed to find upper bound")
}

fn digits_number(id: i64) -> usize {
    id.to_string().len()
}

fn parse_intervals(input: &str) -> Vec<(i64, i64)> {
    input
        .trim()
        .split(',')
        .map(|interval| {
            interval
                .split_once('-')
                .map(|(a, b)| {
                    (
                        a.parse::<i64>().expect("Failed to parse interval number"),
                        b.parse::<i64>().expect("Failed to parse interval number"),
                    )
                })
                .expect("Failed to parse interval")
        })
        .collect::<Vec<(i64, i64)>>()
}

fn repeat_digits(id: i64, times: usize) -> i128 {
    id.to_string()
        .repeat(times)
        .parse::<i128>()
        .expect("Failed to parse repeated digits")
}

fn included(id: i64, intervals: &[(i64, i64)]) -> bool {
    intervals.iter().any(|&(a, b)| a <= id && id <= b)
}
fn part1(input: &str) -> i64 {
    let intervals = parse_intervals(input);
    let max_id: i64 = upper_bound(&intervals);
    (0..max_id)
        .take_while(|&id| repeat_digits(id, 2) <= max_id as i128)
        .map(|id| repeat_digits(id, 2))
        .filter(|&id| included(id as i64, &intervals))
        .sum::<i128>() as i64
}

fn part2(input: &str) -> i64 {
    let intervals = parse_intervals(input);
    let max_id: i64 = upper_bound(&intervals);
    let max_digits: usize = digits_number(max_id);
    (2..max_digits)
        .flat_map(|t| {
            (0..max_id)
                .take_while(move |&id| repeat_digits(id, t) <= max_id as i128)
                .map(move |id| repeat_digits(id, t))
                .filter(|&id| included(id as i64, &intervals))
        })
        .collect::<HashSet<i128>>()
        .iter()
        .sum::<i128>() as i64
}

fn main() {
    let file_name = "day2.in";
    let content = fs::read_to_string(file_name).expect("Failed to read input file");
    let input: &str = content.lines().next().expect("Failed to read input line");
    println!("Part 1: {}", part1(input));
    println!("Part 2: {}", part2(input));
}
