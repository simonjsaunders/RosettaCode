func commonPrefix(string1: String, string2: String) -> String {
    var result = String()
    for (ch, _) in zip(string1, string2).prefix(while: {$0 == $1}) {
        result.append(ch)
    }
    return result
}

func longestCommonPrefix(_ strings: [String]) -> String {
    if strings.count == 0 {
        return ""
    }
    var result = strings[0]
    for string in strings[1..<strings.count] {
        result = commonPrefix(string1: result, string2: string)
    }
    return result
}

func printLongestCommonPrefix(_ strings: [String]) {
    print("lcp(\(strings)) = \"\(longestCommonPrefix(strings))\"")
}

printLongestCommonPrefix(["interspecies", "interstellar", "interstate"])
printLongestCommonPrefix(["throne", "throne"])
printLongestCommonPrefix(["throne", "dungeon"])
printLongestCommonPrefix(["throne", "", "throne"])
printLongestCommonPrefix(["cheese"])
printLongestCommonPrefix([""])
printLongestCommonPrefix([])
printLongestCommonPrefix(["prefix", "suffix"])
printLongestCommonPrefix(["foo", "foobar"])
