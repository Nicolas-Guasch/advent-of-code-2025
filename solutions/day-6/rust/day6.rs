use std::convert::TryFrom;
use std::fs;
use std::str::from_utf8;

#[derive(Debug)]
enum Operator {
    Add,
    Multiply,
}

impl TryFrom<&str> for Operator {
    type Error = String;

    fn try_from(s: &str) -> Result<Self, Self::Error> {
        match s {
            "+" => Ok(Operator::Add),
            "*" => Ok(Operator::Multiply),
            _ => Err(format!("Invalid operator: {}", s)),
        }
    }
}

impl TryFrom<char> for Operator {
    type Error = String;

    fn try_from(c: char) -> Result<Self, Self::Error> {
        match c {
            '+' => Ok(Operator::Add),
            '*' => Ok(Operator::Multiply),
            _ => Err(format!("Invalid operator: {}", c)),
        }
    }
}

#[derive(Debug)]
struct Problem {
    op: Operator,
    operands: Vec<i64>,
}
impl Problem {
    fn evaluate(&self) -> i64 {
        match self.op {
            Operator::Add => self.operands.iter().sum(),
            Operator::Multiply => self.operands.iter().product(),
        }
    }
}

fn partition_problems(input: &str) -> Vec<Problem> {
    let mut lines: Vec<Vec<&str>> = input
        .lines()
        .map(|line| line.split_whitespace().collect::<Vec<&str>>())
        .collect();

    let operators = lines.pop().unwrap();
    let operand_rows = lines;
    (0..operand_rows[0].len())
        .map(|i| {
            let op =
                Operator::try_from(operators[i]).expect("Operator should be either '+' or '*'");
            let operands = operand_rows
                .iter()
                .map(|row| row[i].parse::<i64>().expect("Operand should be a number"))
                .collect();
            Problem { op, operands }
        })
        .collect()
}

fn part1(input: &str) -> i64 {
    partition_problems(input)
        .iter()
        .map(|problem| problem.evaluate())
        .sum()
}

fn partition_problems_vertical(input: &str) -> Vec<Problem> {
    let lines: Vec<&[u8]> = input.lines().map(|row| row.as_bytes()).collect();
    let columns = (0..lines[0].len())
        .map(|i| {
            let col_bytes = lines.iter().map(|row| row[i]).collect::<Vec<u8>>();
            String::from_utf8(col_bytes).unwrap()
        })
        .collect::<Vec<String>>();
    let problem_data = columns.split(|column| column.trim().is_empty());

    fn parse_problem(problem: &[String]) -> Problem {
        let op = Operator::try_from(problem[0].chars().last().unwrap())
            .expect("Operator should be either '+' or '*'");
        let operands = problem
            .iter()
            .map(|col| {
                col.trim_end_matches(['+', '*'])
                    .trim()
                    .parse::<i64>()
                    .expect("Operand should consist only of digits")
            })
            .collect::<Vec<i64>>();
        Problem { op, operands }
    }
    problem_data.map(parse_problem).collect()
}

fn part2(input: &str) -> i64 {
    partition_problems_vertical(input)
        .iter()
        .map(|problem| problem.evaluate())
        .sum()
}

fn main() {
    let file_name = "day6.in";
    let input = fs::read_to_string(file_name).expect(".in files should be present in this folder.");
    println!("Part1: {}", part1(&input));
    println!("Part2: {}", part2(&input));
}
