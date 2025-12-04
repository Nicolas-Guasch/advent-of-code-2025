use std::cmp::Ordering;
use std::fs;

fn max_joltage(battery_bank: &str) -> i64 {
    let mut joltages = battery_bank.bytes().map(|b| (b - b'0') as i64);

    let first_joltage = match joltages.next() {
        Some(d) => d,
        None => return 0,
    };

    joltages
        .fold(
            (first_joltage, 0),
            |(max_digit, max_joltage), current_digit| {
                let produced_joltage = max_digit * 10 + current_digit;
                (
                    max_digit.max(current_digit),
                    max_joltage.max(produced_joltage),
                )
            },
        )
        .1
}

fn part1(input: &str) -> i64 {
    input.lines().map(max_joltage).sum()
}

fn max_joltage_position(values: &[u8], pos: usize, window: usize) -> (usize, u8) {
    let (offset, &max_value) = values[pos..pos + window]
        .iter()
        .enumerate()
        .max_by(|(_, a), (_, b)| a.cmp(b).then(Ordering::Greater))
        .expect("Failed to find max joltage position");

    (pos + offset, max_value)
}

const DIGITS: usize = 12;

fn greedy(battery_bank: &str) -> i64 {
    let bytes = battery_bank.as_bytes();

    let mut result = [0u8; DIGITS];
    let mut current_pos = 0;

    for remaining in (1..=DIGITS).rev() {
        let window = bytes.len() - current_pos - remaining + 1;
        let (next_pos, byte) = max_joltage_position(bytes, current_pos, window);
        result[DIGITS - remaining] = byte;
        current_pos = next_pos + 1;
    }

    let s = str::from_utf8(&result).expect("Failed to convert to string");
    s.parse::<i64>().expect("Failed to parse string to i64")
}

fn part2(input: &str) -> i64 {
    input.lines().map(greedy).sum()
}

fn main() {
    let file_name = "day3.in";
    let input = fs::read_to_string(file_name).expect("Failed to read input file");
    println!("Part 1: {}", part1(&input));
    println!("Part 2: {}", part2(&input));
}
