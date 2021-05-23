// Implementation of Heap's algorithm.
// See https://en.wikipedia.org/wiki/Heap%27s_algorithm#Details_of_the_algorithm
func generate<T>(array: inout [T], output: (_: [T]) -> Void) {
    let n = array.count
    var c = Array(repeating: 0, count: n)
    var i = 1
    output(array)
    while i < n {
        if c[i] < i {
            if (i & 1) == 0 {
                array.swapAt(0, i)
            } else {
                array.swapAt(c[i], i)
            }
            output(array)
            c[i] += 1
            i = 1
        } else {
            c[i] = 0
            i += 1
        }
    }
}

func printPermutations<T>(array: inout [T]) {
    var even = true
    func p(array: [T]) {
        print("\(array) \(even ? "+1" : "-1")")
        even = !even
    }
    generate(array: &array, output: p)
}

print("Permutations and signs for three items:")
var a = [0, 1, 2]
printPermutations(array: &a)

print("\nPermutations and signs for four items:")
var b = [0, 1, 2, 3]
printPermutations(array: &b)
