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

struct PrimeSieve {
    composite : BitArray
}

impl PrimeSieve {
    fn new(limit : usize) -> PrimeSieve {
        let mut sieve = PrimeSieve { composite : BitArray::new(limit/2) };
        let mut p = 3;
        while p * p <= limit {
            if !sieve.composite.get(p/2 - 1)  {
                let inc = p * 2;
                let mut q = p * p;
                while q <= limit {
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

// return number of decimal digits
fn count_digits(mut n: u32) -> u32 {
    let mut digits = 0;
    while n > 0 {
        n /= 10;
        digits += 1;
    }
    digits
}

// return the number with one digit replaced
fn change_digit(mut n: u32, mut index: u32, new_digit: u32) -> u32 {
    let mut p = 1;
    let mut changed = 0;
    while index > 0 {
        changed += p * (n % 10);
        p *= 10;
        n /= 10;
        index -= 1;
    }
    changed += (10 * (n/10) + new_digit) * p;
    changed
}

fn unprimeable(sieve: &PrimeSieve, n: u32) -> bool {
    if sieve.is_prime(n as usize) {
        return false;
    }
    let d = count_digits(n);
    for i in 0..d {
        for j in 0..10 {
            let m = change_digit(n, i, j);
            if m != n && sieve.is_prime(m as usize) {
                return false;
            }
        }
    }
    true
}

fn main() {
    let mut count = 0;
    let mut n = 100;
    let mut lowest = vec![0; 10];
    let mut found = 0;
    let sieve = PrimeSieve::new(10000000);
    println!("First 35 unprimeable numbers:");
    while count < 600 || found < 10 {
        if unprimeable(&sieve, n) {
            if count < 35 {
                if count > 0 {
                    print!(", ");
                }
                print!("{}", n);
            }
            count += 1;
            if count == 600 {
                println!("\n600th unprimeable number: {}", n);
            }
            let last_digit = n as usize % 10;
            if lowest[last_digit] == 0 {
                lowest[last_digit] = n;
                found += 1;
            }
        }
        n += 1;
    }
    for i in 0..10 {
        println!("Least unprimeable number ending in {}: {}" , i, lowest[i]);
    }
}
