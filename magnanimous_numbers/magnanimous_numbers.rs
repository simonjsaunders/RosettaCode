fn is_prime(n : u32) -> bool {
    if n < 2 {
        return false;
    }
    if n % 2 == 0 {
        return n == 2;
    }
    if n % 3 == 0 {
        return n == 3;
    }
    let mut p = 5;
    while p * p <= n {
        if n % p == 0 {
            return false;
        }
        p += 2;
        if n % p == 0 {
            return false;
        }
        p += 4;
    }
    true
}

fn is_magnanimous(n : u32) -> bool {
    let mut p : u32 = 10;
    while n >= p {
        if !is_prime(n % p + n / p) {
            return false;
        }
        p *= 10;
    }
    true
}

struct Magnanimous {
    next: u32
}

impl Iterator for Magnanimous {
    type Item = u32;
    fn next(&mut self) -> Option<u32> {
        let mut n = self.next;
        while !is_magnanimous(n) {
            n += 1;
        }
        self.next = n + 1;
        Some(n)
    }
}

fn magnanimous() -> Magnanimous {
    Magnanimous { next: 0 }
}

fn main() {
    let mut m = magnanimous().take(400);
    println!("First 45 magnanimous numbers:");
    for (i, n) in m.by_ref().take(45).enumerate() {
        if i > 0 && i % 15 == 0 {
            println!();
        }
        print!("{:3} ", n);
    }
    println!("\n\n241st through 250th magnanimous numbers:");
    for n in m.by_ref().skip(195).take(10) {
        print!("{} ", n);
    }
    println!("\n\n391st through 400th magnanimous numbers:");
    for n in m.by_ref().skip(140) {
        print!("{} ", n);
    }
    println!();
}