use std::fs;

fn parse_rotation(rotation: &str) -> i32 {
    (if rotation.starts_with("L") { -1 } else { 1 })
        * rotation[1..]
            .parse::<i32>()
            .expect("Failed to parse rotation")
}

const DIAL_LEN: i32 = 100;

fn part1(rotations: &Vec<&str>) -> i32 {
    let mut position = 50;
    let mut password = 0;
    for &rotation in rotations {
        position = ((position + parse_rotation(rotation)) % DIAL_LEN + DIAL_LEN) % DIAL_LEN;
        password += (position == 0) as i32;
    }
    password
}

fn part2(rotations: &Vec<&str>) -> i32 {
    let mut position = 50;
    let mut password = 0;
    for &rotation in rotations {
        let delta = parse_rotation(rotation);
        password += if position + delta <= 0 {
            (position != 0) as i32 - (position + delta) / DIAL_LEN
        } else {
            (position + delta) / DIAL_LEN
        };
        position = ((position + delta) % DIAL_LEN + DIAL_LEN) % DIAL_LEN;
    }
    password
}

fn main() {
    let file_name = "day1.in";
    let content = fs::read_to_string(file_name).expect("Failed to read input file");
    let rotations: Vec<&str> = content.lines().collect();
    println!("Part 1: {}", part1(&rotations));
    println!("Part 2: {}", part2(&rotations));
}
