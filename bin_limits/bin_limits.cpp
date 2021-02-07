#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

std::vector<int> bins(const std::vector<int>& limits,
                      const std::vector<int>& data) {
    std::vector<int> result(limits.size() + 1, 0);
    for (int n : data) {
        auto i = std::upper_bound(limits.begin(), limits.end(), n);
        ++result[i - limits.begin()];
    }
    return result;
}

void print_bins(const std::vector<int>& limits, const std::vector<int>& bins) {
    size_t n = limits.size();
    if (n == 0)
        return;
    assert(n + 1 == bins.size());
    std::cout << "           < " << std::setw(3) << limits[0] << ": "
              << std::setw(2) << bins[0] << '\n';
    for (size_t i = 1; i < n; ++i)
        std::cout << ">= " << std::setw(3) << limits[i - 1] << " and < "
                  << std::setw(3) << limits[i] << ": " << std::setw(2)
                  << bins[i] << '\n';
    std::cout << ">= " << std::setw(3) << limits[n - 1] << "          : "
              << std::setw(2) << bins[n] << '\n';
}

int main() {
    const std::vector<int> limits1{23, 37, 43, 53, 67, 83};
    const std::vector<int> data1{
        95, 21, 94, 12, 99, 4,  70, 75, 83, 93, 52, 80, 57, 5,  53, 86, 65,
        17, 92, 83, 71, 61, 54, 58, 47, 16, 8,  9,  32, 84, 7,  87, 46, 19,
        30, 37, 96, 6,  98, 40, 79, 97, 45, 64, 60, 29, 49, 36, 43, 55};

    std::cout << "Example 1:\n";
    print_bins(limits1, bins(limits1, data1));

    const std::vector<int> limits2{14,  18,  249, 312, 389,
                                   392, 513, 591, 634, 720};
    const std::vector<int> data2{
        445, 814, 519, 697, 700, 130, 255, 889, 481, 122, 932, 77,  323, 525,
        570, 219, 367, 523, 442, 933, 416, 589, 930, 373, 202, 253, 775, 47,
        731, 685, 293, 126, 133, 450, 545, 100, 741, 583, 763, 306, 655, 267,
        248, 477, 549, 238, 62,  678, 98,  534, 622, 907, 406, 714, 184, 391,
        913, 42,  560, 247, 346, 860, 56,  138, 546, 38,  985, 948, 58,  213,
        799, 319, 390, 634, 458, 945, 733, 507, 916, 123, 345, 110, 720, 917,
        313, 845, 426, 9,   457, 628, 410, 723, 354, 895, 881, 953, 677, 137,
        397, 97,  854, 740, 83,  216, 421, 94,  517, 479, 292, 963, 376, 981,
        480, 39,  257, 272, 157, 5,   316, 395, 787, 942, 456, 242, 759, 898,
        576, 67,  298, 425, 894, 435, 831, 241, 989, 614, 987, 770, 384, 692,
        698, 765, 331, 487, 251, 600, 879, 342, 982, 527, 736, 795, 585, 40,
        54,  901, 408, 359, 577, 237, 605, 847, 353, 968, 832, 205, 838, 427,
        876, 959, 686, 646, 835, 127, 621, 892, 443, 198, 988, 791, 466, 23,
        707, 467, 33,  670, 921, 180, 991, 396, 160, 436, 717, 918, 8,   374,
        101, 684, 727, 749};

    std::cout << "\nExample 2:\n";
    print_bins(limits2, bins(limits2, data2));
}
