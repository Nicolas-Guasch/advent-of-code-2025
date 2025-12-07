use std::fs;

fn parse_input(input: &str) -> (Vec<(i64, i64)>, Vec<i64>) {
    let (ranges, ids) = input
        .split_once("\n\n")
        .expect("Failed to split ranges and ids");
    let ranges = ranges
        .lines()
        .map(|line| {
            let (l, r) = line.split_once("-").expect("Failed to split range");
            (
                l.parse::<i64>().expect("Failed to parse l from range"),
                r.parse::<i64>().expect("Failed to parse r from range"),
            )
        })
        .collect();
    let ids = ids
        .lines()
        .map(|line| line.parse::<i64>().expect("Failed to parse id"))
        .collect();
    (ranges, ids)
}

fn part1(input: &str) -> i64 {
    let (mut ranges, mut ids) = parse_input(input);
    ranges.sort();
    ids.sort();

    let mut fresh = 0;
    let mut id_pos = 0;
    for range in ranges {
        let (l, r) = range;
        while id_pos < ids.len() && ids[id_pos] < l {
            id_pos += 1;
        }
        while id_pos < ids.len() && ids[id_pos] <= r {
            id_pos += 1;
            fresh += 1;
        }
    }
    fresh
}

fn part2(input: &str) -> i64 {
    let (mut ranges, _ids) = parse_input(input);
    ranges.sort();

    let (fresh_count, _max_id) = ranges
        .iter()
        .fold((0, i64::MIN), |(count, max_seen), range| {
            let &(l, r) = range;
            if r > max_seen {
                let l = std::cmp::max(l, max_seen + 1);
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
