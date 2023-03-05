/*#ifndef matrix_h
#define matrix_h
#include <stdexcept>
#include <iostream>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <class T, size_t Rows, size_t Columns>
class Matrix {
 public:
  T matrix[Rows][Columns];
  size_t RowsNumber() const;
  size_t ColumnsNumber() const;
  T& operator()(size_t i, size_t j);
  const T& operator()(size_t i, size_t j) const;
  T& At(size_t i, size_t j);
  const T& At(size_t i, size_t j) const;
  Matrix& operator+=(Matrix);
  Matrix& operator-=(Matrix);
  Matrix& operator*=(Matrix<T, Columns, Columns>);
  Matrix& operator*=(double);
  Matrix& operator/=(double);
};

template <class T, size_t Rows, size_t Columns>
T& Matrix<T, Rows, Columns>::operator()(size_t i, size_t j) {
  return matrix[i][j];
}

template <class T, size_t Rows, size_t Columns>
const T& Matrix<T, Rows, Columns>::operator()(size_t i, size_t j) const {
  return matrix[i][j];
}

template <class T, size_t Rows, size_t Columns>
T& Matrix<T, Rows, Columns>::At(size_t i, size_t j) {
  if (i > Rows || j > Columns) {
    throw MatrixOutOfRange{};
  }
  return matrix[i][j];
}

template <class T, size_t Rows, size_t Columns>
const T& Matrix<T, Rows, Columns>::At(size_t i, size_t j) const {
  if (i > Rows || j > Columns) {
    throw MatrixOutOfRange{};
  }
  return matrix[i][j];
}

template <class T, size_t Rows, size_t Columns>
size_t Matrix<T, Rows, Columns>::RowsNumber() const {
  return Rows;
}

template <class T, size_t Rows, size_t Columns>
size_t Matrix<T, Rows, Columns>::ColumnsNumber() const {
  return Columns;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Columns, Rows> GetTransposed(Matrix<T, Rows, Columns> matrix) {
  Matrix<T, Columns, Rows> transposed;
  for (size_t i = 0; i < Columns; ++i) {
    for (size_t j = 0; j < Rows; ++j) {
      transposed(i, j) = matrix(j, i);
    }
  }
  return transposed;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns> operator+(Matrix<T, Rows, Columns> first, Matrix<T, Rows, Columns> second) {
  Matrix res = first;
  return res += second;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns> operator-(Matrix<T, Rows, Columns> first, Matrix<T, Rows, Columns> second) {
  Matrix res = first;
  return res -= second;
}

template <class T, size_t Rows, size_t Columns, size_t Equals>
Matrix<T, Rows, Columns> operator*(Matrix<T, Rows, Equals> first, Matrix<T, Equals, Columns> second) {
  Matrix<T, Rows, Columns> res;
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      res(i, j) = 0;
      for (size_t k = 0; k < Equals; ++k) {
        res(i, j) += first(i, k) * second(k, j);
      }
    }
  }
  return res;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& Matrix<T, Rows, Columns>::operator+=(Matrix<T, Rows, Columns> new_matrix) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matrix[i][j] += new_matrix(i, j);
    }
  }
  return *this;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& Matrix<T, Rows, Columns>::operator-=(Matrix<T, Rows, Columns> new_matrix) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matrix[i][j] -= new_matrix(i, j);
    }
  }
  return *this;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& Matrix<T, Rows, Columns>::operator*=(Matrix<T, Columns, Columns> new_matrix) {
  Matrix<T, Rows, Columns> res;
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      res(i, j) = 0;
      for (size_t k = 0; k < Columns; ++k) {
        res(i, j) += matrix[i][k] * new_matrix(k, j);
      }
    }
  }
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matrix[i][j] = res(i, j);
    }
  }
  return *this;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& Matrix<T, Rows, Columns>::operator*=(double k) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matrix[i][j] *= k;
    }
  }
  return *this;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& Matrix<T, Rows, Columns>::operator/=(double k) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matrix[i][j] /= k;
    }
  }
  return *this;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns> operator*(double k, Matrix<T, Rows, Columns> matrix) {
  Matrix<T, Rows, Columns> res = matrix;
  return res *= k;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns> operator*(Matrix<T, Rows, Columns> matrix, double k) {
  Matrix<T, Rows, Columns> res = matrix;
  return res *= k;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns> operator/(Matrix<T, Rows, Columns> matrix, double k) {
  Matrix<T, Rows, Columns> res = matrix;
  return res /= k;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& operator*=(double k, Matrix<T, Rows, Columns> matr) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matr[i][j] *= k;
    }
  }
  return matr;
}

template <class T, size_t Rows, size_t Columns>
Matrix<T, Rows, Columns>& operator/=(double k, Matrix<T, Rows, Columns> matr) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      matr[i][j] /= k;
    }
  }
  return matr;
}

template <class T, size_t Rows, size_t Columns>
bool operator==(Matrix<T, Rows, Columns> first, Matrix<T, Rows, Columns> second) {
  bool flag = true;
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      if (first(i, j) != second(i, j)) {
        flag = false;
        break;
      }
    }
    if (!flag) {
      break;
    }
  }
  return flag;
}

template <class T, size_t Rows, size_t Columns>
bool operator!=(Matrix<T, Rows, Columns> first, Matrix<T, Rows, Columns> second) {
  return !(first == second);
}

template <class T, size_t Rows, size_t Columns>
std::istream& operator>>(std::istream& in, Matrix<T, Rows, Columns>& x) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      in >> x.matrix[i][j];
    }
  }
  return in;
}

template <class T, size_t Rows, size_t Columns>
std::ostream& operator<<(std::ostream& out, const Matrix<T, Rows, Columns>& x) {
  for (size_t i = 0; i < Rows; ++i) {
    for (size_t j = 0; j < Columns; ++j) {
      out << x(i, j);
      if (j < Columns - 1) {
        out << " ";
      }
    }
    out << '\n';
  }
  return out;
}

#endif
*/
