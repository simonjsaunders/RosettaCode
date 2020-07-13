// [dependencies]
// chrono = "0.4"

fn is_palindrome(s: &str) -> bool {
    let r: String = s.chars().rev().collect();
    s == r
}

fn main() {
    let mut date = chrono::Utc::today();
    let mut count = 0;
    while count < 15 {
        if is_palindrome(&date.format("%Y%m%d").to_string()) {
            println!("{}", date.format("%F"));
            count += 1;
        }
        date = date.succ();
    }
}
