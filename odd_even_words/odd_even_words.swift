import Foundation

func isVowel(_ char: Character) -> Bool {
    switch (char) {
    case "a", "A", "e", "E", "i", "I", "o", "O", "u", "U":
        return true
    default:
        return false
    }
}

func alternatingVowelsAndConsonants(word: String) -> Bool {
    return zip(word, word.dropFirst()).allSatisfy{isVowel($0.0) != isVowel($0.1)}
}

do {
    let words = try String(contentsOfFile: "unixdict.txt", encoding: String.Encoding.ascii)
    let lines = words.components(separatedBy: "\n")
    for (n, word) in lines.filter(
        {$0.count > 9 && alternatingVowelsAndConsonants(word: $0)}).enumerated() {
        print("\(String(format: "%2d", n + 1)): \(word)")
    }
} catch {
    print(error.localizedDescription)
}
