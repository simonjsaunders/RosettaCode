// Implementation of Heap's algorithm.
// See https://en.wikipedia.org/wiki/Heap%27s_algorithm#Details_of_the_algorithm
fn generate<T, F>(a: &mut [T], mut output: F)
where
    F: FnMut(&[T]),
{
    let n = a.len();
    let mut c = vec![0; n];
    let mut i = 1;
    output(a);
    while i < n {
        if c[i] < i {
            if (i & 1) == 0 {
                a.swap(0, i);
            } else {
                a.swap(c[i], i);
            }
            output(a);
            c[i] += 1;
            i = 1;
        } else {
            c[i] = 0;
            i += 1;
        }
    }
}

fn print_permutations<T: std::fmt::Debug>(a: &mut [T]) {
    let mut even = true;
    let p = move |x: &[T]| {
        println!("{:?} {}", x, if even { "+1" } else { "-1" });
        even = !even;
    };
    generate(a, p);
}

fn main() {
    println!("Permutations and signs for three items:");
    let mut a = vec![0, 1, 2];
    print_permutations(&mut a);

    println!("\nPermutations and signs for four items:");
    let mut b = vec![0, 1, 2, 3];
    print_permutations(&mut b);
}
