package main

import "fmt"

func main() {
    var N uint = 12345
    fmt.Printf("f(%d)=%d\n", N, f(N))
    fmt.Printf("The biggest N to let f(N)=N is %d\n", getMaxN())
}

func f(N uint) uint {
    if N < 10 { if N == 0 { return 0 } else { return 1} }
    var firstVal uint = 0     // 最高位的值
    var size uint = 0         // 十进制位数
    n := N
    for n > 0 {
        firstVal = n
        n /= 10
        size++
    }
    base := pow(10, size-1)   // 最小的size位数
    if firstVal == 1 {
        return f(N % base) + (N % base + 1) + (size-1) * pow(10, size-2)
    }
    return pow(10, size-1) + firstVal*(size-1) * pow(10, size-2) + f(N % base)
}

func pow(x uint, y uint) uint {
    var res uint = 1
    for y > 0 {
        res *= x
        y--
    }
    return res
}

func getMaxN() uint {
    max := pow(10, 10) - 1
    min := pow(10, 9) - 1
    for true {
        mid := (max + min) / 2
        val := f(mid)
        if (val < mid) {
            min = mid
        } else if (val > mid) {
            max = mid
        } else {
            return mid;
        }
    }
    return 0
}
