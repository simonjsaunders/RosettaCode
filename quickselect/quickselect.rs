// See https://en.wikipedia.org/wiki/Quickselect

fn partition<T: Ord>(a: &mut [T], left : usize, right : usize, pivot : usize) -> usize {
    a.swap(pivot, right);
    let mut store_index = left;
    for i in left..right {
        if a[i] < a[right] {
            a.swap(store_index, i);
            store_index += 1;
        }
    }
    a.swap(right, store_index);
    return store_index;
}

fn pivot_index(left : usize, right : usize) -> usize {
    return left + (right - left)/2;
}

fn select<T: Ord>(a: &mut [T], left : usize, right : usize, n : usize) {
    let mut low = left;
    let mut high = right;
    loop {
        if low == high {
            break;
        }
        let mut pivot = pivot_index(low, high);
        pivot = partition(a, low, high, pivot);
        if n == pivot {
            break;
        }
        else if n < pivot {
            high = pivot - 1;
        } else {
            low = pivot + 1;
        }
    }
}

// Rearranges the elements of 'a' such that the element at index 'n' is
// the same as it would be if the array were sorted, smaller elements are
// to the left of it and larger elements are to its right.
fn nth_element<T: Ord>(a: &mut [T], n : usize) {
    let len = a.len();
    select(a, 0, len - 1, n);
}

fn main() {
    let a = vec![9, 8, 7, 6, 5, 0, 1, 2, 3, 4];
    for n in 0..a.len() {
        let mut b = a.clone();
        nth_element(&mut b, n);
        println!("n = {}, nth element = {}", n + 1, b[n]);
    }
}
