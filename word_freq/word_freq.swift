import Foundation

func printTopWords(path: String, count: Int) throws {
    // load file contents into a string
    let text = try String(contentsOfFile: path, encoding: String.Encoding.utf8)
    var dict = Dictionary<String, Int>()
    // split text into words, convert to lowercase and store word counts in dict
    let regex = try NSRegularExpression(pattern: "\\w+")
    for match in regex.matches(in: text, range: NSRange(text.startIndex..., in: text)) {
        let word = String(text[Range(match.range, in: text)!]).lowercased()
        if let n = dict[word] {
            dict[word] = n + 1
        } else {
            dict[word] = 1
        }
    }
    // sort words by number of occurrences
    let wordCounts = dict.sorted(by: {$0.1 > $1.1})
    // print the top count words
    print("Rank\tWord\tCount")
    for (i, (word, n)) in wordCounts.prefix(count).enumerated() {
        print("\(i + 1)\t\(word)\t\(n)")
    }
}

do {
    try printTopWords(path: "135-0.txt", count: 10)
} catch {
    print(error.localizedDescription)
}
