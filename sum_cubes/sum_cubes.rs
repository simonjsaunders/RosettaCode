fn main() {
    (0..50)
        .map(|x| x * x * x)
        .scan(0, |sum, x| {
            *sum += x;
            Some(*sum)
        })
        .enumerate()
        .for_each(|(i, n)| {
            print!("{:7}", n);
            if (i + 1) % 5 == 0 {
                println!();
            } else {
                print!(" ");
            }
        });
}
