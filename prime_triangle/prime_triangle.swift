import Foundation

func isPrime(_ n: Int) -> Bool {
    guard n > 0 && n < 64 else {
        return false
    }
    return ((UInt64(1) << n) & 0x28208a20a08a28ac) != 0
}

func primeTriangleRow(_ a: inout ArraySlice<Int>) -> Bool {
    let start = a.startIndex
    let end = a.endIndex
    if a.count == 2 {
        return isPrime(a[start] + a[start + 1])
    }
    for i in start + 1..<end - 1 {
        if isPrime(a[start] + a[i]) {
            a.swapAt(i, start + 1)
            if primeTriangleRow(&a[start + 1..<end]) {
                return true
            }
            a.swapAt(i, start + 1)
        }
    }
    return false
}

func primeTriangleCount(_ a: inout ArraySlice<Int>) -> Int {
    let start = a.startIndex
    let end = a.endIndex
    var count = 0
    if a.count == 2 {
        if isPrime(a[start] + a[start + 1]) {
            count += 1
        }
    } else {
        for i in start + 1..<end - 1 {
            if isPrime(a[start] + a[i]) {
                a.swapAt(i, start + 1)
                count += primeTriangleCount(&a[start + 1..<end])
                a.swapAt(i, start + 1)
            }
        }
    }
    return count
}

func printRow(_ a: [Int]) {
    if a.count == 0 {
        return
    }
    print(String(format: "%2d", a[0]), terminator: "")
    for x in a[1...] {
        print(String(format: " %2d", x), terminator: "")
    }
    print()
}

for n in 2...20 {
    var a = Array(1...n)
    if primeTriangleRow(&a[...]) {
        printRow(a)
    }
}
print()

for n in 2...20 {
    var a = Array(1...n)
    if n > 2 {
        print(" ", terminator: "")
    }
    print("\(primeTriangleCount(&a[...]))", terminator: "")
}
print()
