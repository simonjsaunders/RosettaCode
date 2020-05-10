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
            std::copy(begin(row), end(row), row_data(i++));
        }
    }

    size_t rows() const { return rows_; }
    size_t columns() const { return columns_; }

    scalar_type* row_data(size_t row) {
        assert(row < rows_);
        return &elements_[row * columns_];
    }
    const scalar_type* row_data(size_t row) const {
        assert(row < rows_);
        return &elements_[row * columns_];
    }

    const scalar_type& at(size_t row, size_t column) const {
        assert(column < columns_);
        return row_data(row)[column];
    }
    scalar_type& at(size_t row, size_t column) {
        assert(column < columns_);
        return row_data(row)[column];
    }
private:
    size_t rows_;
    size_t columns_;
    std::vector<scalar_type> elements_;
};

template <typename scalar_type>
matrix<scalar_type> product(const matrix<scalar_type>& a,
                            const matrix<scalar_type>& b) {
    assert(a.columns() == b.rows());
    size_t arows = a.rows();
    size_t bcolumns = b.columns();
    size_t n = a.columns();
    matrix<scalar_type> c(arows, bcolumns);
    for (size_t i = 0; i < arows; ++i) {
        const auto* aptr = a.row_data(i);
        auto* cptr = c.row_data(i);
        for (size_t j = 0; j < n; ++j) {
            const auto* bptr = b.row_data(j);
            for (size_t k = 0; k < bcolumns; ++k)
                cptr[k] += aptr[j] * bptr[k];
        }
    }
    return c;
}

template <typename scalar_type>
void print(std::ostream& out, const matrix<scalar_type>& a) {
    size_t rows = a.rows(), columns = a.columns();
    out << std::fixed << std::setprecision(5);
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            if (column > 0)
                out << ' ';
            out << std::setw(8) << a.at(row, column);
        }
        out << '\n';
    }
}

template <typename scalar_type>
matrix<scalar_type> find_pivot(const matrix<scalar_type>& m) {
    assert(m.rows() == m.columns());
    size_t rows = m.rows();
    std::vector<size_t> perm(rows);
    std::iota(perm.begin(), perm.end(), 0);
    for (size_t column = 0; column < rows; ++column) {
        size_t max_index = column;
        for (size_t row = column; row < rows; ++row) {
            if (m.at(row, column) > m.at(max_index, column))
                max_index = row;
        }
        if (column != max_index)
            std::swap(perm[column], perm[max_index]);
    }
    matrix<scalar_type> pivot(rows, rows);
    for (size_t i = 0; i < rows; ++i)
        pivot.at(i, perm[i]) = 1;
    return pivot;
}

template <typename scalar_type>
void lu_decompose(const matrix<scalar_type>& input) {
    assert(input.rows() == input.columns());
    size_t n = input.rows();
    auto pivot = find_pivot(input);
    auto input1 = product(pivot, input);
    matrix<scalar_type> lower(n, n);
    matrix<scalar_type> upper(n, n);
    for (size_t j = 0; j < n; ++j) {
        lower.at(j, j) = 1;
        for (size_t i = 0; i <= j; ++i) {
            scalar_type value = input1.at(i, j);
            for (size_t k = 0; k < i; ++k)
                value -= upper.at(k, j) * lower.at(i, k);
            upper.at(i, j) = value;
        }
        for (size_t i = j; i < n; ++i) {
            scalar_type value = input1.at(i, j);
            for (size_t k = 0; k < j; ++k)
                value -= upper.at(k, j) * lower.at(i, k);
            value /= upper.at(j, j);
            lower.at(i, j) = value;
        }
    }
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
    matrix<double> matrix1(3, 3,
       {{1, 3, 5},
        {2, 4, 7},
        {1, 1, 0}});
    lu_decompose(matrix1);
    std::cout << '\n';

    matrix<double> matrix2(4, 4,
      {{11, 9, 24, 2},
        {1, 5, 2, 6},
        {3, 17, 18, 1},
        {2, 5, 7, 1}});
    lu_decompose(matrix2);

    return 0;
}
