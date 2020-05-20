// [dependencies]
// primal = "0.2"

fn main() {
    use std::collections::BTreeMap;
    let mut transitions = BTreeMap::new();
    let mut prev = 0;
    let limit = 100_000_000;
    for n in primal::Primes::all().take(limit) {
        let digit = n % 10;
        if prev != 0 {
            let key = (prev, digit);
            if let Some(v) = transitions.get_mut(&key) {
                *v += 1;
            } else {
                transitions.insert(key, 1);
            }
        }
        prev = digit;
    }
    println!("First {} prime numbers:", limit);
    for ((from, to), c) in & transitions {
        let freq = 100.0*(*c as f32)/(limit as f32);
        println!("{} -> {}: count = {:7}, frequency = {:.2} %", from, to, c, freq);
    }
}
