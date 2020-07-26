// [dependencies]
// primal = "0.3"
// num-format = "0.4"

fn main() {
    use num_format::{Locale, ToFormattedString};
    let mut count = 0;
    let mut previous = 0;
    let mut power = 1;
    let mut limit = 10;
    for prime in primal::Primes::all() {
        if prime > limit {
            println!(
                "Number of twin prime pairs less than {} is {}",
                limit.to_formatted_string(&Locale::en),
                count.to_formatted_string(&Locale::en)
            );
            limit *= 10;
            power += 1;
            if power > 10 {
                break;
            }
        }
        if previous > 0 && prime == previous + 2 {
            count += 1;
        }
        previous = prime;
    }
}
