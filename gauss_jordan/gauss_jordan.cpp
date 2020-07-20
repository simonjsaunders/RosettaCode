#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
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
void swap_rows(matrix<scalar_type>& m, size_t i, size_t j) {
    size_t columns = m.columns();
    for (size_t column = 0; column < columns; ++column)
        std::swap(m.at(i, column), m.at(j, column));
}

// Convert matrix to reduced row echelon form
template <typename scalar_type>
void rref(matrix<scalar_type>& m) {
    size_t rows = m.rows();
    size_t columns = m.columns();
    for (size_t row = 0, lead = 0; row < rows && lead < columns; ++row, ++lead) {
        size_t i = row;
        while (m.at(i, lead) == 0) {
            if (++i == rows) {
                i = row;
                if (++lead == columns)
                    return;
            }
        }
        swap_rows(m, i, row);
        if (m.at(row, lead) != 0) {
            scalar_type f = m.at(row, lead);
            for (size_t column = 0; column < columns; ++column)
                m.at(row, column) /= f;
        }
        for (size_t j = 0; j < rows; ++j) {
            if (j == row)
                continue;
            scalar_type f = m.at(j, lead);
            for (size_t column = 0; column < columns; ++column)
                m.at(j, column) -= f * m.at(row, column);
        }
    }
}

template <typename scalar_type>
matrix<scalar_type> inverse(const matrix<scalar_type>& m) {
    assert(m.rows() == m.columns());
    size_t rows = m.rows();
    matrix<scalar_type> tmp(rows, 2 * rows, 0);
    for (size_t row = 0; row < rows; ++row) {
        std::copy_n(m.row_data(row), rows, tmp.row_data(row));
        tmp.at(row, row + rows) = 1;
    }
    rref(tmp);
    matrix<scalar_type> inv(rows, rows);
    for (size_t row = 0; row < rows; ++row)
        std::copy_n(tmp.row_data(row) + rows, rows, inv.row_data(row));
    return inv;
}

template <typename scalar_type>
void print(std::ostream& out, const matrix<scalar_type>& a) {
    size_t rows = a.rows(), columns = a.columns();
    out << std::fixed << std::setprecision(4);
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            if (column > 0)
                out << ' ';
            out << std::setw(7) << a.at(row, column);
        }
        out << '\n';
    }
}

int main() {
    matrix<double> m(3, 3, {{ 2, -1,  0},
                            {-1,  2, -1},
                            { 0, -1,  2}});
    std::cout << "Matrix:\n";
    print(std::cout, m);
    auto inv(inverse(m));
    std::cout << "Inverse:\n";
    print(std::cout, inv);
    std::cout << "Product of matrix and inverse:\n";
    print(std::cout, product(m, inv));
    std::cout << "Inverse of inverse:\n";
    print(std::cout, inverse(inv));
    return 0;
}
