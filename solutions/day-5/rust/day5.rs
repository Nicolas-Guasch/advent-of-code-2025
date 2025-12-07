use std::fs;

fn parse_input(
    input: &str,
) -> (
    impl Iterator<Item = (i64, i64)> + '_,
    impl Iterator<Item = i64> + '_,
) {
    let (ranges, ids) = input
        .split_once("\n\n")
        .expect("Failed to split ranges and ids");
    let ranges = ranges
        .lines()
        .map(|line| line.split_once("-").expect("Failed to split range"))
        .map(|(l, r)| {
            (
                l.parse::<i64>().expect("Failed to parse l from range"),
                r.parse::<i64>().expect("Failed to parse r from range"),
            )
        });
    let ids = ids
        .lines()
        .map(|line| line.parse::<i64>().expect("Failed to parse id"));
    (ranges, ids)
}

fn part1(input: &str) -> i64 {
    let (ranges, ids) = parse_input(input);
    let mut ranges = ranges.collect::<Vec<(i64, i64)>>();
    let mut ids = ids.collect::<Vec<i64>>();
    ranges.sort_unstable();
    ids.sort_unstable();

    let mut fresh = 0;
    let mut id_iter = ids.into_iter().peekable();
    for (l, r) in ranges {
        while id_iter.peek().is_some_and(|&id| id < l) {
            id_iter.next();
        }
        while id_iter.peek().is_some_and(|&id| id <= r) {
            id_iter.next();
            fresh += 1;
        }
    }
    fresh
}

fn part2(input: &str) -> i64 {
    let (ranges, _ids) = parse_input(input);
    let mut ranges = ranges.collect::<Vec<(i64, i64)>>();
    ranges.sort_unstable();

    let (fresh_count, _max_id) =
        ranges
            .into_iter()
            .fold((0, i64::MIN), |(count, max_seen), range| {
                let (l, r) = range;
                if r > max_seen {
                    let l = l.max(max_seen + 1);
                    (count + r - l + 1, r)
                } else {
                    (count, max_seen)
                }
            });
    fresh_count
}

fn main() {
    let file_name = "day5.in";
    let input = fs::read_to_string(file_name).expect("Failed to read input file");
    println!("Part 1: {}", part1(&input));
    println!("Part 2: {}", part2(&input));
}
