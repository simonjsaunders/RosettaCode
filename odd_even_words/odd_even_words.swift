import Foundation

func isVowel(_ char: Character) -> Bool {
    switch (char) {
    case "a", "A", "e", "E", "i", "I", "o", "O", "u", "U":
        return true
    default:
        return false
    }
}

do {
    let words = try String(contentsOfFile: "unixdict.txt", encoding: String.Encoding.ascii)
    var n = 1
    for word in words.components(separatedBy: "\n") {
        if word.count <= 9 {
            continue
        }
        var isOddEvenWord = true
        var prev: Character = "\n"
        for ch in word {
            if prev != "\n" && isVowel(ch) == isVowel(prev) {
                isOddEvenWord = false
                break
            }
            prev = ch
        }
        if isOddEvenWord {
            print("\(String(format: "%2d", n)): \(word)")
            n += 1
        }
    }
} catch {
    print(error.localizedDescription)
}
