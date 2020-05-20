struct BitArray {
    array : Vec<u32>
}

impl BitArray {
    fn new(size : usize) -> BitArray {
        BitArray { array : vec![0; (size+31)/32] }
    }
    fn get(&self, index : usize) -> bool {
        let bit = 1 << (index & 31);
        (self.array[index >> 5] & bit) != 0
    }
    fn set(&mut self, index : usize, new_val : bool) {
        let bit = 1 << (index & 31);
        if new_val {
            self.array[index >> 5] |= bit;
        } else {
            self.array[index >> 5] &= !bit;
        }
    }
}

struct Sieve {
    composite : BitArray
}

impl Sieve {
    fn new(limit : usize) -> Sieve {
        let size : usize = 1 + 2 * (limit/2);
        let mut sieve = Sieve { composite : BitArray::new(size/2) };
        let mut p = 3;
        while p * p <= size {
            if !sieve.composite.get(p/2 - 1)  {
                let inc = p * 2;
                let mut q = p * p;
                while q <= size {
                    sieve.composite.set(q/2 - 1, true);
                    q += inc;
                }
            }
            p += 2;
        }
        sieve
    }
    fn is_prime(&self, n : usize) -> bool {
        if n < 2 {
            return false;
        }
        if n % 2 == 0 {
            return n == 2;
        }
        !self.composite.get(n/2 - 1)
    }
}

// See https://en.wikipedia.org/wiki/Prime_number_theorem#Approximations_for_the_nth_prime_number
fn upper_bound_for_nth_prime(n : usize) -> usize {
    let x = n as f64;
    (x * (x.ln() + x.ln().ln())) as usize
}

fn compute_transitions(limit : usize) {
    use std::collections::BTreeMap;
    let mut transitions = BTreeMap::new();
    let mut prev = 2;
    let mut count = 0;
    let sieve = Sieve::new(upper_bound_for_nth_prime(limit));
    let mut n = 3;
    while count < limit {
        if sieve.is_prime(n) {
            count += 1;
            let digit = n % 10;
            let key = (prev, digit);
            if let Some(v) = transitions.get_mut(&key) {
                *v += 1;
            } else {
                transitions.insert(key, 1);
            }
            prev = digit;
        }
        n += 2;
    }
    println!("First {} prime numbers:", limit);
    for ((from, to), c) in & transitions {
        let freq = 100.0*(*c as f32)/(limit as f32);
        println!("{} -> {}: count = {:7}, frequency = {:.2} %", from, to, c, freq);
    }
}

fn main() {
    compute_transitions(1000000);
    println!();
    compute_transitions(100000000);
}
