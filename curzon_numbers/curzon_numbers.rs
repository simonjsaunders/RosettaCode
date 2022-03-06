// [dependencies]
// rug = "1.15.0"

fn is_curzon(n: u32, k: u32) -> bool {
    use rug::{Complete, Integer};
    (Integer::u_pow_u(k, n).complete() + 1) % (k * n + 1) == 0
}

fn main() {
    for k in (2..=10).step_by(2) {
        println!("Curzon numbers with base {k}:");
        let mut count = 0;
        let mut n = 1;
        while count < 50 {
            if is_curzon(n, k) {
                count += 1;
                print!("{:4}{}", n, if count % 10 == 0 { "\n" } else { " " });
            }
            n += 1;
        }
        loop {
            if is_curzon(n, k) {
                count += 1;
                if count == 1000 {
                    break;
                }
            }
            n += 1;
        }
        println!("1000th Curzon number with base {k}: {n}\n");
    }
}
