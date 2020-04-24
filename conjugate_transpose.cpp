#include <cassert>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <vector>

template <typename scalar_type> class complex_matrix {
public:
    using element_type = std::complex<scalar_type>;

    complex_matrix(size_t rows, size_t columns)
        : rows_(rows), columns_(columns) {
        elements_.resize(rows * columns);
    }
    complex_matrix(size_t rows, size_t columns, element_type value)
        : rows_(rows), columns_(columns), elements_(rows * columns, value) {}

    size_t rows() const { return rows_; }
    size_t columns() const { return columns_; }
    element_type* data() { return &elements_[0]; }
    const element_type* data() const { return &elements_[0]; }
    element_type* row_data(size_t row) {
        assert(row < rows_);
        return &elements_[row * columns_];
    }
    const element_type* row_data(size_t row) const {
        assert(row < rows_);
        return &elements_[row * columns_];
    }

    const element_type& at(size_t row, size_t column) const {
        assert(row < rows_);
        assert(column < columns_);
        return elements_[index(row, column)];
    }
    element_type& at(size_t row, size_t column) {
        assert(row < rows_);
        assert(column < columns_);
        return elements_[index(row, column)];
    }

    friend bool operator==(const complex_matrix& a, const complex_matrix& b) {
        return a.rows_ == b.rows_ && a.columns_ == b.columns_ &&
               a.elements_ == b.elements_;
    }
private:
    size_t index(size_t row, size_t column) const {
        return row * columns_ + column;
    }
    size_t rows_;
    size_t columns_;
    std::vector<element_type> elements_;
};

template <typename scalar_type>
complex_matrix<scalar_type> product(const complex_matrix<scalar_type>& a,
                                    const complex_matrix<scalar_type>& b) {
    assert(a.columns() == b.rows());
    size_t arows = a.rows();
    size_t bcolumns = b.columns();
    size_t n = a.columns();
    complex_matrix<scalar_type> c(arows, bcolumns);
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
complex_matrix<scalar_type>
conjugate_transpose(const complex_matrix<scalar_type>& a) {
    size_t rows = a.rows(), columns = a.columns();
    complex_matrix<scalar_type> b(columns, rows);
    for (size_t i = 0; i < columns; i++) {
        auto* bptr = b.row_data(i);
        for (size_t j = 0; j < rows; j++) {
            *bptr++ = std::conj(a.at(j, i));
        }
    }
    return b;
}

template <typename scalar_type>
void print(std::ostream& out, const complex_matrix<scalar_type>& a) {
    out << '[';
    size_t rows = a.rows(), columns = a.columns();
    for (size_t row = 0; row < rows; ++row) {
        if (row > 0)
            out << ", ";
        out << '[';
        for (size_t column = 0; column < columns; ++column) {
            if (column > 0)
                out << ", ";
            out << a.at(row, column);
        }
        out << ']';
    }
    out << "]\n";
}

template <typename scalar_type>
bool is_hermitian_matrix(const complex_matrix<scalar_type>& matrix) {
    if (matrix.rows() != matrix.columns())
        return false;
    return matrix == conjugate_transpose(matrix);
}

template <typename scalar_type>
bool is_normal_matrix(const complex_matrix<scalar_type>& matrix) {
    if (matrix.rows() != matrix.columns())
        return false;
    auto c = conjugate_transpose(matrix);
    return product(c, matrix) == product(matrix, c);
}

bool is_equal(const std::complex<double>& a, double b) {
    constexpr double e = 1e-15;
    return std::abs(a.imag()) < e && std::abs(a.real() - b) < e;
}

template <typename scalar_type>
bool is_identity_matrix(const complex_matrix<scalar_type>& matrix) {
    if (matrix.rows() != matrix.columns())
        return false;
    size_t rows = matrix.rows();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            if (!is_equal(matrix.at(i, j), scalar_type(i == j ? 1 : 0)))
                return false;
        }
    }
    return true;
}

template <typename scalar_type>
bool is_unitary_matrix(const complex_matrix<scalar_type>& matrix) {
    if (matrix.rows() != matrix.columns())
        return false;
    auto c = conjugate_transpose(matrix);
    auto p = product(c, matrix);
    return is_identity_matrix(p) && p == product(matrix, c);
}

template <typename scalar_type>
void test(const complex_matrix<scalar_type>& matrix) {
    std::cout << "Matrix: ";
    print(std::cout, matrix);
    std::cout << "Conjugate transpose: ";
    print(std::cout, conjugate_transpose(matrix));
    std::cout << std::boolalpha;
    std::cout << "Hermitian: " << is_hermitian_matrix(matrix) << '\n';
    std::cout << "Normal: " << is_normal_matrix(matrix) << '\n';
    std::cout << "Unitary: " << is_unitary_matrix(matrix) << '\n';
}

int main() {
    complex_matrix<double> matrix1(3, 3);
    complex_matrix<double> matrix2(3, 3);
    complex_matrix<double> matrix3(3, 3);

    matrix1.at(0, 0) = {2, 0};
    matrix1.at(0, 1) = {2, 1};
    matrix1.at(0, 2) = {4, 0};
    matrix1.at(1, 0) = {2, -1};
    matrix1.at(1, 1) = {3, 0};
    matrix1.at(1, 2) = {0, 1};
    matrix1.at(2, 0) = {4, 0};
    matrix1.at(2, 1) = {0, -1};
    matrix1.at(2, 2) = {1, 0};

    double n = std::sqrt(0.5);
    matrix2.at(0, 0) = {n, 0};
    matrix2.at(0, 1) = {n, 0};
    matrix2.at(0, 2) = {0, 0};
    matrix2.at(1, 0) = {0, -n};
    matrix2.at(1, 1) = {0, n};
    matrix2.at(1, 2) = {0, 0};
    matrix2.at(2, 0) = {0, 0};
    matrix2.at(2, 1) = {0, 0};
    matrix2.at(2, 2) = {0, 1};

    matrix3.at(0, 0) = {2, 2};
    matrix3.at(0, 1) = {3, 1};
    matrix3.at(0, 2) = {-3, 5};
    matrix3.at(1, 0) = {2, -1};
    matrix3.at(1, 1) = {4, 1};
    matrix3.at(1, 2) = {0, 0};
    matrix3.at(2, 0) = {7, -5};
    matrix3.at(2, 1) = {1, -4};
    matrix3.at(2, 2) = {1, 0};

    test(matrix1);
    std::cout << '\n';
    test(matrix2);
    std::cout << '\n';
    test(matrix3);
    return 0;
}
