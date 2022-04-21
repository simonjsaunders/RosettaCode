// [dependencies]
// rug = "1.15.0"

use rug::Integer;
use rug::integer::IsPrime;

fn cullen_number(n: u32) -> Integer {
    let num = Integer::from(n);
    (num << n) + 1
}

fn woodall_number(n: u32) -> Integer {
    let num = Integer::from(n);
    (num << n) - 1
}

fn main() {
    println!("First 20 Cullen numbers:");
    for n in 1..21 {
        if n > 1 {
            print!(" ");
        }
        print!("{}", cullen_number(n));
    }

    println!("\n\nFirst 20 Woodall numbers:");
    for n in 1..21 {
        if n > 1 {
            print!(" ");
        }
        print!("{}", woodall_number(n));
    }

    println!("\n\nFirst 5 Cullen primes in terms of n:");
    let mut n = 1;
    let mut count = 0;
    while count != 5 {
        if cullen_number(n).is_probably_prime(25) != IsPrime::No {
            if count > 0 {
                print!(" ");
            }
            print!("{}", n);
            count += 1;
        }
        n += 1;
    }

    println!("\n\nFirst 5 Woodall primes in terms of n:");
    n = 1;
    count = 0;
    while count != 12 {
        if woodall_number(n).is_probably_prime(25) != IsPrime::No {
            if count > 0 {
                print!(" ");
            }
            print!("{}", n);
            count += 1;
        }
        n += 1;
    }
    println!();
}
