func squareFree(number: UInt) -> Bool {
    var n = number
    if n % 4 == 0 {
        return false
    }
    var p: UInt = 3
    while p * p <= n {
        var count = 0
        while n % p == 0 {
            count += 1
            if count > 1 {
                return false
            }
            n /= p
        }
        p += 2
    }
    return true
}

func printSquareFreeNumbers(from: UInt, to: UInt) {
    print("Square-free numbers between \(from) and \(to):")
    var line = ""
    for i in from...to {
        if squareFree(number: i) {
            if line.count > 0 {
                line += " "
            }
            line += "\(i)"
            if line.count >= 80 {
                print(line)
                line = ""
            }
        }
    }
    if line.count > 0 {
        print(line)
    }
}

func printSquareFreeCount(from: UInt, to: UInt) {
    var count = 0
    for i in from...to {
        if squareFree(number: i) {
            count += 1
        }
    }
    print("Number of square-free numbers between \(from) and \(to): \(count)")
}

printSquareFreeNumbers(from: 1, to: 145)
printSquareFreeNumbers(from: 1000000000000, to: 1000000000145)

var n: UInt = 100
while n <= 1000000 {
    printSquareFreeCount(from: 1, to: n)
    n *= 10
}
