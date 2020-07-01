// References:
// https://en.wikipedia.org/wiki/Full_reptend_prime
// https://en.wikipedia.org/wiki/Primitive_root_modulo_n#Finding_primitive_roots

struct BitArray {
    array: Vec<u32>,
}

impl BitArray {
    fn new(size: usize) -> BitArray {
        BitArray {
            array: vec![0; (size + 31) / 32],
        }
    }
    fn get(&self, index: usize) -> bool {
        let bit = 1 << (index & 31);
        (self.array[index >> 5] & bit) != 0
    }
    fn set(&mut self, index: usize, new_val: bool) {
        let bit = 1 << (index & 31);
        if new_val {
            self.array[index >> 5] |= bit;
        } else {
            self.array[index >> 5] &= !bit;
        }
    }
}

struct Sieve {
    composite: BitArray,
}

impl Sieve {
    fn new(limit: usize) -> Sieve {
        let mut sieve = Sieve {
            composite: BitArray::new(limit / 2),
        };
        let mut p = 3;
        while p * p <= limit {
            if !sieve.composite.get(p / 2 - 1) {
                let inc = p * 2;
                let mut q = p * p;
                while q <= limit {
                    sieve.composite.set(q / 2 - 1, true);
                    q += inc;
                }
            }
            p += 2;
        }
        sieve
    }
    fn is_prime(&self, n: usize) -> bool {
        if n < 2 {
            return false;
        }
        if n % 2 == 0 {
            return n == 2;
        }
        !self.composite.get(n / 2 - 1)
    }
}

fn modpow(mut base: usize, mut exp: usize, n: usize) -> usize {
    if n == 1 {
        return 0;
    }
    let mut result: usize = 1;
    base %= n;
    while exp > 0 {
        if (exp & 1) == 1 {
            result = (result * base) % n;
        }
        base = (base * base) % n;
        exp >>= 1;
    }
    result
}

fn is_long_prime(sieve: &Sieve, prime: usize) -> bool {
    if 10 % prime == 0 {
        return false;
    }
    let n: usize = prime - 1;
    let mut m: usize = n;
    let mut p: usize = 2;
    while p * p <= n {
        if sieve.is_prime(p) && m % p == 0 {
            if modpow(10, n / p, prime) == 1 {
                return false;
            }
            while m % p == 0 {
                m /= p;
            }
        }
        p += 1;
    }
    if m == 1 {
        return true;
    }
    modpow(10, n / m, prime) != 1
}

fn long_primes(limit1: usize, limit2: usize) {
    let sieve = Sieve::new(limit2);
    let mut count: usize = 0;
    let mut limit: usize = limit1;
    let mut prime: usize = 3;
    while prime < limit2 {
        if sieve.is_prime(prime) && is_long_prime(&sieve, prime) {
            if prime < limit1 {
                print!("{} ", prime);
            }
            if prime > limit {
                print!("\nNumber of long primes up to {}: {}", limit, count);
                limit *= 2;
            }
            count += 1;
        }
        prime += 2;
    }
    println!("\nNumber of long primes up to {}: {}", limit, count);
}

fn main() {
    long_primes(500, 8192000);
}
