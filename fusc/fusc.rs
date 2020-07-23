fn fusc_sequence() -> impl std::iter::Iterator<Item = u32> {
    let mut sequence = vec![0, 1];
    let mut n = 0;
    std::iter::from_fn(move || {
        if n > 1 {
            sequence.push(match n % 2 {
                0 => sequence[n / 2],
                _ => sequence[(n - 1) / 2] + sequence[(n + 1) / 2],
            });
        }
        let result = sequence[n];
        n += 1;
        Some(result)
    })
}

fn digit_count(mut n: u32) -> u32 {
    let mut count = 1;
    while n >= 10 {
        count += 1;
        n /= 10;
    }
    count
}

fn main() {
    println!("First 61 fusc numbers:");
    for n in fusc_sequence().take(61) {
        print!("{} ", n)
    }
    println!();

    let limit = 20000000;
    println!(
        "Fusc numbers up to {} that are longer than any previous one:",
        limit
    );
    let mut max_length = 0;
    let mut max_fusc = 0;
    for (index, n) in fusc_sequence().take(limit).enumerate() {
        if n < max_fusc {
            continue;
        }
        let length = digit_count(n);
        if length > max_length {
            max_length = length;
            println!("index = {}, fusc number = {}", index, n);
        }
        max_fusc = n;
    }
}
