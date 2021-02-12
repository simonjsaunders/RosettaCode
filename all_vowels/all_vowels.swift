import Foundation

func containsAllVowelsOnce(_ word: String) -> Bool {
    var seen = Array(repeating: false, count: 5)
    for ch in word {
        var index = -1
        switch (ch) {
        case "a", "A":
            index = 0
        case "e", "E":
            index = 1
        case "i", "I":
            index = 2
        case "o", "O":
            index = 3
        case "u", "U":
            index = 4
        default:
            break
        }
        if index < 0 {
            continue
        }
        if seen[index] {
            return false
        }
        seen[index] = true
    }
    return seen.allSatisfy{$0}
}

do {
    try String(contentsOfFile: "unixdict.txt", encoding: String.Encoding.ascii)
        .components(separatedBy: "\n")
        .filter{$0.count > 10 && containsAllVowelsOnce($0)}
        .enumerated()
        .forEach{print(String(format: "%2d. %@", $0.0 + 1, $0.1))}
} catch {
    print(error.localizedDescription)
}
