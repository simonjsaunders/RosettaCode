import Foundation

class BitArray {
    var array: [UInt32]

    init(size: Int) {
        self.array = Array(repeating: 0, count: (size + 31)/32)
    }
    
    func get(index: Int) -> Bool {
        let bit = UInt32(1) << (index & 31)
        return (self.array[index >> 5] & bit) != 0
    }
    
    func set(index: Int, value: Bool) {
        let bit = UInt32(1) << (index & 31)
        if value {
            self.array[index >> 5] |= bit
        } else {
            self.array[index >> 5] &= ~bit
        }
    }
}

class PrimeSieve {
    var composite: BitArray
    
    init(size: Int) {
        self.composite = BitArray(size: size/2)
        var p = 3
        while p * p <= size {
            if !self.composite.get(index: p/2 - 1) {
                let inc = p * 2
                var q = p * p
                while q <= size {
                    self.composite.set(index: q/2 - 1, value: true)
                    q += inc
                }
            }
            p += 2
        }
    }
    
    func isPrime(number: Int) -> Bool {
        if number < 2 {
            return false
        }
        if (number & 1) == 0 {
            return number == 2
        }
        return !self.composite.get(index: number/2 - 1)
    }
}

func loadDictionary(_ path: String) throws -> [String] {
    let contents = try String(contentsOfFile: path, encoding: String.Encoding.ascii)
    return contents.components(separatedBy: "\n").filter{!$0.isEmpty}
}

do {
    let dictionary = try loadDictionary("unixdict.txt")
    let sieve = PrimeSieve(size: 255)
    for word in dictionary {
        if word.allSatisfy({$0.isASCII && sieve.isPrime(number: Int($0.asciiValue!))}) {
            print(word)
        }
    }
} catch {
    print(error.localizedDescription)
}
