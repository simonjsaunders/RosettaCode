// [dependencies]
// primal = "0.3"

fn is_prime(n: u32) -> bool {
    primal::is_prime(n as u64)
}

fn next_prime_digit_number(n: u32) -> u32 {
    if n == 0 {
        return 2;
    }
    match n % 10 {
        2 => n + 1,
        3 | 5 => n + 2,
        _ => 2 + next_prime_digit_number(n / 10) * 10,
    }
}

fn digit_sum(mut n: u32) -> u32 {
    let mut sum = 0;
    while n > 0 {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

fn main() {
    let limit1 = 10000;
    let limit2 = 1000000000;
    let last = 10;
    let mut p = 0;
    let mut n = 0;
    let mut extra_primes = vec![0; last];
    println!("Extra primes under {}:", limit1);
    while p < limit2 {
        p = next_prime_digit_number(p);
        if is_prime(p) && is_prime(digit_sum(p)) {
            n += 1;
            if p < limit1 {
                println!("{:2}: {}", n, p);
            }
            extra_primes[n % last] = p;
        }
    }
    println!("\nLast {} extra primes under {}:", last, limit2);
    let mut i = last;
    while i > 0 {
        i -= 1;
        println!("{}: {}", n - i, extra_primes[(n - i) % last]);
    }
}
