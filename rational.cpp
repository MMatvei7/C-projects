#include "rational.h"
#include <numeric>
#include <iostream>

void Rational::ReduceFraction() {
  auto nod = std::gcd(numerator_, denominator_);
  numerator_ /= nod;
  denominator_ /= nod;
  if (denominator_ < 0) {
    denominator_ *= -1;
    numerator_ *= -1;
  }
}

Rational::Rational() : numerator_(0), denominator_(1) {
}

Rational::Rational(int32_t x) : numerator_(x), denominator_(1) {
}

Rational::Rational(int32_t num, int32_t denum) : numerator_(num), denominator_(denum) {
  if (denominator_ == 0) {
    throw RationalDivisionByZero{};
  }
  ReduceFraction();
}

int32_t Rational::GetNumerator() const {
  return numerator_;
}

int32_t Rational::GetDenominator() const {
  return denominator_;
}

void Rational::SetNumerator(int32_t num) {
  numerator_ = num;
  ReduceFraction();
}

void Rational::SetDenominator(int32_t denum) {
  if (denum == 0) {
    throw RationalDivisionByZero{};
  }
  denominator_ = denum;
  ReduceFraction();
}

Rational& Rational::operator+=(Rational frac) {
  SetNumerator(numerator_ * frac.GetDenominator() + denominator_ * frac.GetNumerator());
  SetDenominator(denominator_ * frac.GetDenominator());
  ReduceFraction();
  return *this;
}

Rational& Rational::operator-=(Rational frac) {
  SetNumerator(numerator_ * frac.GetDenominator() - denominator_ * frac.GetNumerator());
  SetDenominator(denominator_ * frac.GetDenominator());
  ReduceFraction();
  return *this;
}

Rational& Rational::operator/=(Rational frac) {
  SetNumerator(numerator_ * frac.GetDenominator());
  SetDenominator(denominator_ * frac.GetNumerator());
  ReduceFraction();
  return *this;
}

Rational& Rational::operator*=(Rational frac) {
  SetNumerator(numerator_ * frac.GetNumerator());
  SetDenominator(denominator_ * frac.GetDenominator());
  ReduceFraction();
  return *this;
}

Rational operator+(Rational frac_first, Rational frac_second) {
  return frac_first += frac_second;
}

Rational operator-(Rational frac_first, Rational frac_second) {
  return frac_first -= frac_second;
}

Rational operator/(Rational frac_first, Rational frac_second) {
  return frac_first /= frac_second;
}

Rational operator*(Rational frac_first, Rational frac_second) {
  return frac_first *= frac_second;
}

Rational& Rational::operator+=(int32_t number) {
  SetNumerator(numerator_ + denominator_ * number);
  ReduceFraction();
  return *this;
}

Rational& Rational::operator-=(int32_t number) {
  SetNumerator(numerator_ - denominator_ * number);
  ReduceFraction();
  return *this;
}

Rational& Rational::operator/=(int32_t number) {
  if (number == 0) {
    throw RationalDivisionByZero{};
  }
  SetDenominator(denominator_ * number);
  ReduceFraction();
  return *this;
}

Rational& Rational::operator*=(int32_t number) {
  SetNumerator(numerator_ * number);
  ReduceFraction();
  return *this;
}

Rational operator+(Rational frac, int32_t number) {
  return frac += number;
}

Rational operator-(Rational frac, int32_t number) {
  return frac -= number;
}

Rational operator*(Rational frac, int32_t number) {
  return frac *= number;
}

Rational operator/(Rational frac, int32_t number) {
  return frac /= number;
}

Rational operator+(int32_t number, Rational frac) {
  return frac += number;
}

Rational operator-(int32_t number, Rational frac) {
  return frac -= number;
}

Rational operator*(int32_t number, Rational frac) {
  return frac *= number;
}

Rational operator/(int32_t number, Rational frac) {
  return frac /= number;
}

Rational operator+(Rational frac) {
  return frac;
}

Rational operator-(Rational frac) {
  frac.SetNumerator(-frac.GetNumerator());
  return frac;
}

Rational Rational::operator++(int) {
  auto res = *this;
  *this += 1;
  return res;
}

Rational& Rational::operator++() {
  *this += 1;
  return *this;
}

Rational Rational::operator--(int) {
  auto res = *this;
  *this -= 1;
  return res;
}

Rational& Rational::operator--() {
  *this -= 1;
  return *this;
}

bool Rational::operator<(Rational frac) const {
  return (numerator_ * frac.GetDenominator() - denominator_ * frac.GetNumerator() < 0);
}

bool operator==(Rational frac1, Rational frac2) {
  return (frac1.GetNumerator() == frac2.GetNumerator() && frac1.GetDenominator() == frac2.GetDenominator());
}

bool operator>(Rational frac_first, Rational frac_second) {
  return (!(frac_first < frac_second) && !(frac_first == frac_second));
}

bool operator<=(Rational frac_first, Rational frac_second) {
  return !(frac_first > frac_second);
}

bool operator>=(Rational frac_first, Rational frac_second) {
  return !(frac_first < frac_second);
}

bool operator!=(Rational frac_first, Rational frac_second) {
  return !(frac_first == frac_second);
}

std::istream& operator>>(std::istream& in, Rational& x) {
  int32_t numerator = 0;
  int32_t denuminator = 1;
  while (std::isspace(in.peek())) {
    in.get();
  }
  int16_t sign = 1;
  if (in.peek() == '-') {
    in.get();
    sign *= -1;
  } else if (in.peek() == '+') {
    in.get();
  }
  while (std::isdigit(in.peek())) {
    char c = in.get();
    numerator *= 10;
    numerator += c - '0';
  }
  if (in.peek() == '/') {
    in.get();
    denuminator = 0;
    if (in.peek() == '-') {
      in.get();
      sign *= -1;
    } else if (in.peek() == '+') {
      in.get();
    }
    while (std::isdigit(in.peek())) {
      char c = in.get();
      denuminator *= 10;
      denuminator += c - '0';
    }
  }
  numerator *= sign;
  x = Rational(numerator, denuminator);
  x.ReduceFraction();
  return in;
}

std::ostream& operator<<(std::ostream& out, const Rational& x) {
  if (x.GetDenominator() == 1) {
    out << x.GetNumerator();
    return out;
  }
  out << x.GetNumerator() << '/' << x.GetDenominator();
  return out;
}
