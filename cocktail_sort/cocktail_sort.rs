fn cocktail_sort<T: Ord>(a: &mut [T]) {
    let len = a.len();
    loop {
        let mut swapped = false;
        let mut i = 0;
        while i + 2 <= len {
            if a[i] > a[i + 1] {
                a.swap(i, i + 1);
                swapped = true;
            }
            i += 1;
        }
        if swapped {
            swapped = false;
            i = len;
            while i >= 2 {
                if a[i - 2] > a[i - 1] {
                    a.swap(i - 2, i - 1);
                    swapped = true;
                }
                i -= 1;
            }
        }
        if !swapped {
            break;
        }
    }
}

fn main() {
    let mut v = vec![10, 8, 4, 3, 1, 9, 0, 2, 7, 5, 6];
    println!("before: {:?}", v);
    cocktail_sort(&mut v);
    println!("after:  {:?}", v);
}
