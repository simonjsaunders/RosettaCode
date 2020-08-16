#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

template <typename scalar_type> class matrix {
public:
    matrix(size_t rows, size_t columns)
        : rows_(rows), columns_(columns), elements_(rows * columns) {}

    matrix(size_t rows, size_t columns, scalar_type value)
        : rows_(rows), columns_(columns), elements_(rows * columns, value) {}

    matrix(size_t rows, size_t columns,
        const std::initializer_list<std::initializer_list<scalar_type>>& values)
        : rows_(rows), columns_(columns), elements_(rows * columns) {
        assert(values.size() <= rows_);
        size_t i = 0;
        for (const auto& row : values) {
            assert(row.size() <= columns_);
            std::copy(begin(row), end(row), &elements_[i]);
            i += columns_;
        }
    }

    size_t rows() const { return rows_; }
    size_t columns() const { return columns_; }

    const scalar_type& operator()(size_t row, size_t column) const {
        assert(row < rows_);
        assert(column < columns_);
        return elements_[row * columns_ + column];
    }
    scalar_type& operator()(size_t row, size_t column) {
        assert(row < rows_);
        assert(column < columns_);
        return elements_[row * columns_ + column];
    }
private:
    size_t rows_;
    size_t columns_;
    std::vector<scalar_type> elements_;
};

template <typename scalar_type>
void print(std::ostream& out, const matrix<scalar_type>& a) {
    size_t rows = a.rows(), columns = a.columns();
    out << std::fixed << std::setprecision(5);
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            if (column > 0)
                out << ' ';
            out << std::setw(8) << a(row, column);
        }
        out << '\n';
    }
}

template <typename scalar_type>
void lu_decompose(const matrix<scalar_type>& input) {
    assert(input.rows() == input.columns());
    size_t n = input.rows();
    std::vector<size_t> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    matrix<scalar_type> lower(n, n);
    matrix<scalar_type> upper(n, n);
    matrix<scalar_type> input1(input);
    for (size_t j = 0; j < n; ++j) {
        size_t max_index = j;
        scalar_type max_value = 0;
        for (size_t i = j; i < n; ++i) {
            scalar_type value = std::abs(input1(i, j));
            if (value > max_value) {
                max_index = i;
                max_value = value;
            }
        }
        if (j != max_index)
            std::swap(perm[j], perm[max_index]);
        size_t jj = perm[j];
        for (size_t i = j + 1; i < n; ++i) {
            size_t ii = perm[i];
            input1(ii, j) /= input1(jj, j);
            for (size_t k = j + 1; k < n; ++k)
                input1(ii, k) -= input1(ii, j) * input1(jj, k);
        }
    }
    
    for (size_t j = 0; j < n; ++j) {
        lower(j, j) = 1;
        for (size_t i = j + 1; i < n; ++i)
            lower(i, j) = input1(perm[i], j);
        for (size_t i = 0; i <= j; ++i)
            upper(i, j) = input1(perm[i], j);
    }
    
    matrix<scalar_type> pivot(n, n);
    for (size_t i = 0; i < n; ++i)
        pivot(i, perm[i]) = 1;

    std::cout << "A\n";
    print(std::cout, input);
    std::cout << "\nL\n";
    print(std::cout, lower);
    std::cout << "\nU\n";
    print(std::cout, upper);
    std::cout << "\nP\n";
    print(std::cout, pivot);
}

int main() {
    std::cout << "Example 1:\n";
    matrix<double> matrix1(3, 3,
       {{1, 3, 5},
        {2, 4, 7},
        {1, 1, 0}});
    lu_decompose(matrix1);
    std::cout << '\n';

    std::cout << "Example 2:\n";
    matrix<double> matrix2(4, 4,
      {{11, 9, 24, 2},
        {1, 5, 2, 6},
        {3, 17, 18, 1},
        {2, 5, 7, 1}});
    lu_decompose(matrix2);
    std::cout << '\n';
    
    std::cout << "Example 3:\n";
    matrix<double> matrix3(3, 3,
      {{-5, -6, -3},
       {-1,  0, -2},
       {-3, -4, -7}});
    lu_decompose(matrix3);

    return 0;
}
