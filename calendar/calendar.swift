let monthDays = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
let monthNames = ["January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"]
let monthWidth = 20
let monthGap = 2
let dayNames = "Su Mo Tu We Th Fr Sa"

// Gauss's algorithm for calculating the day of the week for 1 Jan
func firstDayOfYear(year: Int) -> Int {
    let c = year / 100
    let y = year % 100 - 1
    return (1 + 5 * (y % 4) + 3 * y + 5 * (c % 4)) % 7
}

func isLeapYear(year: Int) -> Bool {
    return (year % 100 == 0) ? year % 400 == 0 : year % 4 == 0
}

func firstDayOfMonth(year: Int, month: Int) -> Int {
    var day = firstDayOfYear(year: year)
    for m in 1..<month {
        day += monthDays[m - 1]
        if m == 2 && isLeapYear(year: year) {
            day += 1
        }
    }
    return day % 7
}

func daysInMonth(year: Int, month: Int) -> Int {
    var days = monthDays[month - 1]
    if isLeapYear(year: year) && month == 2 {
        days += 1
    }
    return days
}

func rpad(string: String, width: Int) -> String {
    return string.count >= width ? string
        : String(repeating: " ", count: width - string.count) + string
}

func lpad(string: String, width: Int) -> String {
    return string.count >= width ? string
        : string + String(repeating: " ", count: width - string.count)
}

func centre(string: String, width: Int) -> String {
    if string.count >= width {
        return string
    }
    let c = (width - string.count)/2
    return String(repeating: " ", count: c) + string
        + String(repeating: " ", count: width - string.count - c)
}

func formatMonth(year: Int, month: Int) -> [String] {
    var lines: [String] = []
    let firstDay = firstDayOfMonth(year: year, month: month)
    lines.append(centre(string: monthNames[month - 1], width: monthWidth))
    lines.append(dayNames)
    var padWidth = 2
    var line = String(repeating: " ", count: 3 * firstDay)
    for day in 1...daysInMonth(year: year, month: month) {
        line += rpad(string: String(day), width: padWidth)
        padWidth = 3
        if (firstDay + day) % 7 == 0 {
            lines.append(line)
            line = ""
            padWidth = 2
        }
    }
    if line.count > 0 {
        lines.append(lpad(string: line, width: monthWidth))
    }
    return lines
}

func printCentred(string: String, width: Int) {
    print(rpad(string: string, width: (width + string.count)/2))
}

public func printCalendar(year: Int, width: Int) {
    let months = min(12, max(1, (width + monthGap)/(monthWidth + monthGap)))
    let lineWidth = monthWidth * months + monthGap * (months - 1)
    printCentred(string: "[Snoopy]", width: lineWidth)
    printCentred(string: String(year), width: lineWidth)
    var firstMonth = 1
    while firstMonth <= 12 {
        if firstMonth > 1 {
            print()
        }
        let lastMonth = min(12, firstMonth + months - 1)
        let monthCount = lastMonth - firstMonth + 1
        var lines: [[String]] = []
        var lineCount = 0
        for month in firstMonth...lastMonth {
            let monthLines = formatMonth(year: year, month: month)
            lineCount = max(lineCount, monthLines.count)
            lines.append(monthLines)
        }
        for i in 0..<lineCount {
            var line = ""
            for month in 0..<monthCount {
                if month > 0 {
                    line.append(String(repeating: " ", count: monthGap))
                }
                line.append(i < lines[month].count ? lines[month][i]
                            : String(repeating: " ", count: monthWidth))
            }
            print(line)
        }
        firstMonth = lastMonth + 1
    }
}

printCalendar(year: 1969, width: 80)
