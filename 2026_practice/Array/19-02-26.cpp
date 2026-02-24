
#include <bits/stdc++.h>





uint32_t reverseBits(uint32_t n) {
    uint32_t result {0};
    for (int i = 0; i < 32; ++i) {
        result <<= 1;
        result |= (n & 1);
        n >>= 1;
    }
    return result;
}


int main(int argc, char** argv)
{
    uint32_t n {0011};
    std::cout << n << std::endl;
    std::cout << reverseBits(n) << std::endl;
    return (0);
}