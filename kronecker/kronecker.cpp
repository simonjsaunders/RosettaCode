#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

template <typename scalar_type> class matrix {
public:
    matrix(size_t rows, size_t columns)
        : rows_(rows), columns_(columns), elements_(rows * columns) {}
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

// See https://en.wikipedia.org/wiki/Kronecker_product
template <typename scalar_type>
matrix<scalar_type> kronecker_product(const matrix<scalar_type>& a,
                                      const matrix<scalar_type>& b) {
    size_t arows = a.rows();
    size_t acolumns = a.columns();
    size_t brows = b.rows();
    size_t bcolumns = b.columns();
    size_t rows = arows * brows;
    size_t columns = acolumns * bcolumns;
    matrix<scalar_type> c(rows, columns);
    for (size_t i = 0; i < rows; ++i) {
        auto* crow = c.row_data(i);
        auto* arow = a.row_data(i/brows);
        auto* brow = b.row_data(i % brows);
        for (size_t j = 0; j < columns; ++j)
            crow[j] = arow[j/bcolumns] * brow[j % bcolumns];
    }
    return c;
}

template <typename scalar_type>
void print(std::ostream& out, const matrix<scalar_type>& a) {
    size_t rows = a.rows(), columns = a.columns();
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            if (column > 0)
                out << ' ';
            out << std::setw(3) << a.at(row, column);
        }
        out << '\n';
    }
}

void test1() {
    matrix<int> matrix1(2, 2, {{1,2}, {3,4}});
    matrix<int> matrix2(2, 2, {{0,5}, {6,7}});
    matrix<int> kp = kronecker_product(matrix1, matrix2);
    std::cout << "Test case 1:\n";
    print(std::cout, kp);
}

void test2() {
    matrix<int> matrix1(3, 3, {{0,1,0}, {1,1,1}, {0,1,0}});
    matrix<int> matrix2(3, 4, {{1,1,1,1}, {1,0,0,1}, {1,1,1,1}});
    matrix<int> kp = kronecker_product(matrix1, matrix2);
    std::cout << "Test case 2:\n";
    print(std::cout, kp);
}

int main() {
    test1();
    test2();
    return 0;
}
