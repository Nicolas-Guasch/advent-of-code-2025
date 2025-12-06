use std::collections::VecDeque;
use std::fs;
use std::ops::{Index, IndexMut};

#[derive(Debug)]
struct Grid {
    data: Vec<Vec<char>>,
    height: usize,
    width: usize,
}

impl Grid {
    fn new(input: &str) -> Self {
        let data: Vec<Vec<char>> = input.lines().map(|line| line.chars().collect()).collect();

        let height = data.len();
        let width = if height > 0 { data[0].len() } else { 0 };

        Grid {
            data,
            height,
            width,
        }
    }

    fn in_bounds(&self, i: isize, j: isize) -> bool {
        0 <= i && i < self.height as isize && 0 <= j && j < self.width as isize
    }

    fn positions(&self) -> impl Iterator<Item = (usize, usize)> + '_ {
        (0..self.height).flat_map(move |i| (0..self.width).map(move |j| (i, j)))
    }

    fn neighbors(&self, i: usize, j: usize) -> impl Iterator<Item = (usize, usize)> + '_ {
        let i = i as isize;
        let j = j as isize;
        (i - 1..=i + 1).flat_map(move |ni| {
            (j - 1..=j + 1).filter_map(move |nj| {
                if ni == i && nj == j {
                    return None;
                }

                if self.in_bounds(ni, nj) {
                    Some((ni as usize, nj as usize))
                } else {
                    None
                }
            })
        })
    }

    fn count_neighbors_where<F>(&self, i: usize, j: usize, predicate: F) -> usize
    where
        F: Fn(char) -> bool,
    {
        self.neighbors(i, j)
            .filter(|&pos| predicate(self[pos]))
            .count()
    }
}
impl Index<(usize, usize)> for Grid {
    type Output = char;

    fn index(&self, (i, j): (usize, usize)) -> &Self::Output {
        &self.data[i][j]
    }
}

impl IndexMut<(usize, usize)> for Grid {
    fn index_mut(&mut self, (i, j): (usize, usize)) -> &mut Self::Output {
        &mut self.data[i][j]
    }
}

fn part1(input: &str) -> i32 {
    let grid = Grid::new(input);
    grid.positions()
        .filter(|&(i, j)| {
            let is_roll = grid[(i, j)] == '@';
            let neighbors = grid.count_neighbors_where(i, j, |c| c == '@');
            is_roll && neighbors < 4
        })
        .count() as i32
}

fn part2(input: &str) -> i32 {
    let mut grid = Grid::new(input);
    let mut queue: VecDeque<(usize, usize)> = grid
        .positions()
        .filter(|&(i, j)| {
            let is_roll = grid[(i, j)] == '@';
            let neighbors = grid.count_neighbors_where(i, j, |c| c == '@');
            is_roll && neighbors < 4
        })
        .collect();

    let mut accessible_rolls = 0;
    while let Some((i, j)) = queue.pop_front() {
        accessible_rolls += 1;
        grid[(i, j)] = '.';
        queue.extend(grid.neighbors(i, j).filter(|&(ii, jj)| {
            let is_roll = grid[(ii, jj)] == '@';
            let neighbors = grid.count_neighbors_where(ii, jj, |c| c == '@');
            is_roll && neighbors == 3
        }))
    }
    accessible_rolls
}

fn main() {
    let file_name = "day4.in";
    let input = fs::read_to_string(file_name).expect("Failed to read input file");

    println!("Part 1: {}", part1(&input));
    println!("Part 2: {}", part2(&input));
}
