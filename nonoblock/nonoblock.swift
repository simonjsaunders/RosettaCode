import Foundation

class Nonoblock {
    let cells: Int
    let blocks: [Int]
    var positions: [Int]
    var count: Int

    private init(cells: Int, blocks: [Int]) {
        self.cells = cells
        self.blocks = blocks
        positions = Array(repeating: 0, count: blocks.count)
        count = 0
    }

    class func printSolutions(cells: Int, blocks: [Int]) {
        print("\(cells) cells and blocks \(blocks):")
        var totalBlockSize = 0
        for blockSize in blocks {
            totalBlockSize += blockSize
        }
        if cells < totalBlockSize + blocks.count - 1 {
            print("no solution")
            return
        }
        let nb = Nonoblock(cells: cells, blocks: blocks)
        nb.solve(cells: cells, index: 0, totalBlockSize: totalBlockSize)
    }

    private func solve(cells: Int, index: Int, totalBlockSize: Int) {
        if index == blocks.count {
            count += 1
            printSolution()
            return
        }
        let blockSize = blocks[index]
        let maxPos = cells - (totalBlockSize + blocks.count - index - 1)
        let t = totalBlockSize - blockSize
        var c = cells - (blockSize + 1)
        for pos in 0...maxPos {
            positions[index] = pos
            solve(cells: c, index: index + 1, totalBlockSize: t)
            c -= 1
        }
    }

    private func printSolution() {
        var str = String(format: "%2d  ", count)
        var c = 0
        for i in 0..<blocks.count {
            var n = positions[i]
            if i > 0 {
                n += 1
            }
            if n > 0 {
                str.append(String(repeating: ".", count: n))
                c += n
            }
            str.append(String(repeating: "#", count: blocks[i]))
            c += blocks[i]
        }
        if c < cells {
            str.append(String(repeating: ".", count: cells - c))
        }
        print(str)
    }
}

Nonoblock.printSolutions(cells: 5, blocks: [2, 1])
print()

Nonoblock.printSolutions(cells: 5, blocks: [])
print()

Nonoblock.printSolutions(cells: 10, blocks: [8])
print()

Nonoblock.printSolutions(cells: 15, blocks: [2, 3, 2, 3])
print()

Nonoblock.printSolutions(cells: 5, blocks: [2, 3])
