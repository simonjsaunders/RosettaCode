struct Leonardo: Sequence, IteratorProtocol {
    private let add : Int
    private var n0: Int
    private var n1: Int
    
    init(n0: Int, n1: Int, add: Int) {
        self.n0 = n0
        self.n1 = n1
        self.add = add
    }
    
    mutating func next() -> Int? {
        let n = n0
        n0 = n1
        n1 += n + add
        return n
    }
}

print("First 25 Leonardo numbers:")
print(Leonardo(n0: 1, n1: 1, add: 1).prefix(25).map{String($0)}.joined(separator: " "))

print("First 25 Fibonacci numbers:")
print(Leonardo(n0: 0, n1: 1, add: 0).prefix(25).map{String($0)}.joined(separator: " "))
