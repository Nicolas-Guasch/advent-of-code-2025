use std::fs;

fn parse_rotation(rotation: &str) -> i32 {
    (if rotation.starts_with("L") { -1 } else { 1 })
        * rotation[1..]
            .parse::<i32>()
            .expect("Failed to parse rotation")
}

const DIAL_LEN: i32 = 100;

fn part1(rotations: &Vec<&str>) -> i32 {
    let (_final_position, password) = rotations.iter().map(|&r| parse_rotation(r)).fold(
        (50, 0),
        |(position, password), rotation| {
            let next_position = (position + rotation).rem_euclid(DIAL_LEN);
            (next_position, password + (next_position == 0) as i32)
        },
    );
    password
}

fn part2(rotations: &Vec<&str>) -> i32 {
    let (_final_position, password) = rotations.iter().map(|&r| parse_rotation(r)).fold(
        (50, 0),
        |(position, password), rotation| {
            let shifted_pos = position + rotation;
            (
                shifted_pos.rem_euclid(DIAL_LEN),
                password
                    + if shifted_pos <= 0 {
                        (position != 0) as i32 - shifted_pos / DIAL_LEN
                    } else {
                        shifted_pos / DIAL_LEN
                    },
            )
        },
    );
    password
}

fn main() {
    let file_name = "day1.in";
    let content = fs::read_to_string(file_name).expect("Failed to read input file");
    let rotations: Vec<&str> = content.lines().collect();
    println!("Part 1: {}", part1(&rotations));
    println!("Part 2: {}", part2(&rotations));
}
