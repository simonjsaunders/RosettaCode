import Foundation

func oneAway(string1: String, string2: String) -> Bool {
    if string1.count != string2.count {
        return false
    }
    var result = false
    for (c1, c2) in zip(string1, string2) {
        if c1 != c2 {
            if result {
                return false
            }
            result = true
        }
    }
    return result
}

func wordLadder(words: [String], from: String, to: String) {
    var poss = words.filter{$0.count == from.count}
    var queue: [[String]] = [[from]]
    while !queue.isEmpty {
        var curr = queue[0]
        let last = curr[curr.count - 1]
        queue.removeFirst()
        let next = poss.filter{oneAway(string1: $0, string2: last)}
        if next.contains(to) {
            curr.append(to)
            print(curr.joined(separator: " -> "))
            return
        }
        poss.removeAll(where: {next.contains($0)})
        for str in next {
            var temp = curr
            temp.append(str)
            queue.append(temp)
        }
    }
    print("\(from) into \(to) cannot be done.")
}

do {
    let words = try String(contentsOfFile: "unixdict.txt", encoding: String.Encoding.ascii)
        .components(separatedBy: "\n")
        .filter{!$0.isEmpty}
    wordLadder(words: words, from: "man", to: "boy")
    wordLadder(words: words, from: "girl", to: "lady")
    wordLadder(words: words, from: "john", to: "jane")
    wordLadder(words: words, from: "child", to: "adult")
} catch {
    print(error.localizedDescription)
}
