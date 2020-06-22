// [dependencies]
// num = "0.3"

use num::integer::gcd;
use std::collections::HashSet;

fn yellowstone_sequence() -> impl std::iter::Iterator<Item = u32> {
    let mut sequence: HashSet<u32> = HashSet::new();
    let mut min = 1;
    let mut n = 0;
    let mut n1 = 1;
    let mut n2 = 0;
    std::iter::from_fn(move || {
        n2 = n1;
        n1 = n;
        if n < 3 {
            n += 1;
        } else {
            n = min;
            while !(!sequence.contains(&n) && gcd(n1, n) == 1 && gcd(n2, n) > 1) {
                n += 1;
            }
        }
        sequence.insert(n);
        while sequence.contains(&min) {
            sequence.remove(&min);
            min += 1;
        }
        Some(n)
    })
}

fn main() {
    println!("First 30 Yellowstone numbers:");
    for y in yellowstone_sequence().take(30) {
        print!("{} ", y);
    }
    println!();
}
