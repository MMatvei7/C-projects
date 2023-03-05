#ifndef RATIONAL_H
#define RATIONAL_H
#include <stdexcept>

class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};

class Rational {
 private:
  int32_t numerator_;
  int32_t denominator_;

 public:
  Rational();
  Rational(int32_t);  // NOLINT
  Rational(int32_t, int32_t);
  int32_t GetNumerator() const;
  int32_t GetDenominator() const;
  void ReduceFraction();
  void SetNumerator(int32_t);
  void SetDenominator(int32_t);
  Rational& operator+=(Rational);
  Rational& operator-=(Rational);
  Rational& operator/=(Rational);
  Rational& operator*=(Rational);
  Rational& operator+=(int32_t);
  Rational& operator-=(int32_t);
  Rational& operator/=(int32_t);
  Rational& operator*=(int32_t);
  Rational operator++(int);
  Rational& operator++();
  Rational operator--(int);
  Rational& operator--();
  bool operator<(Rational) const;
};

Rational operator+(Rational);
Rational operator-(Rational);
Rational operator+(Rational, int32_t);
Rational operator-(Rational, int32_t);
Rational operator/(Rational, int32_t);
Rational operator*(Rational, int32_t);
Rational operator+(int32_t, Rational);
Rational operator-(int32_t, Rational);
Rational operator/(int32_t, Rational);
Rational operator*(int32_t, Rational);
Rational operator+(Rational, Rational);
Rational operator-(Rational, Rational);
Rational operator/(Rational, Rational);
Rational operator*(Rational, Rational);
bool operator==(Rational, Rational);
bool operator!=(Rational, Rational);
bool operator>(Rational, Rational);
bool operator>=(Rational, Rational);
bool operator<=(Rational, Rational);
std::istream& operator>>(std::istream& in, Rational& x);
std::ostream& operator<<(std::ostream& out, const Rational& x);

#endif
