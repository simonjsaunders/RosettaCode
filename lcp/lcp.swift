func commonPrefix(string1: String, string2: String) -> String {
    return String(zip(string1, string2).prefix(while: {$0 == $1}).map{$0.0})
}

func longestCommonPrefix(_ strings: [String]) -> String {
    return strings.count == 0 ? ""
        : strings[1..<strings.count].reduce(strings[0],
            { commonPrefix(string1: $0, string2: $1) })
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
