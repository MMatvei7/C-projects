#ifndef CTISPRIME__CT_PRIME_H_
#define CTISPRIME__CT_PRIME_H_
#include <type_traits>
#include <cstdint>

template <int64_t N, int64_t L, int64_t R, bool Part>
struct Sqrt;

template <int64_t N>
inline constexpr int64_t kSqrtV = Sqrt<N, 0, 1000000000, true>::kValue;

template <int64_t N, int64_t L, int64_t R, bool Part>
struct Sqrt {
  static constexpr int64_t kValue = Sqrt<N, L, ((R + L) / 2), N < ((R + L) / 2) * ((R + L) / 2)>::kValue + Sqrt<N, ((R + L) / 2), R, N >= ((R + L) / 2) * ((R + L) / 2)>::kValue;
};

template <int64_t N, int64_t L, int64_t R>
struct Sqrt<N, L, R, false> {
  static constexpr int64_t kValue = 0;
};

template <int64_t N, int64_t L>
struct Sqrt<N, L, L + 1, true> {
  static constexpr int64_t kValue = L;
};

template <int64_t N, int64_t L>
struct Sqrt<N, L, L, true> {
  static constexpr int64_t kValue = L;
};

template <int64_t N, int64_t L, int64_t R>
struct HasDivisor;

template <int64_t N, int64_t L, int64_t R>
inline constexpr bool kHasDivisorOnV = HasDivisor<N, L, R>::kValue;

template <int64_t N, int64_t L, int64_t R>
struct HasDivisor {
  static constexpr bool kValue = kHasDivisorOnV<N, L, ((R + L) / 2)> || kHasDivisorOnV<N, ((R + L + 1) / 2), R>;
};

template <int64_t N, int64_t L>
struct HasDivisor<N, L, L> {
  static constexpr bool kValue = (N % L == 0);
};

template <int64_t N, int64_t L>
struct HasDivisor<N, L, L + 1> {
  static constexpr bool kValue = (N % L == 0) || (N % (L + 1) == 0);
};

template <int64_t N>
struct IsPrime;

template <int64_t N>
inline constexpr int64_t kIsPrimeV = IsPrime<N>::kValue;

template <int64_t N>
struct IsPrime {
  static constexpr bool kValue = !kHasDivisorOnV<N, 2, kSqrtV<N>>;
};

template <>
struct IsPrime<0> : std::integral_constant<bool, false> {
};

template <>
struct IsPrime<1> : std::integral_constant<bool, false> {
};

template <>
struct IsPrime<2> : std::integral_constant<bool, true> {
};

template <>
struct IsPrime<3> : std::integral_constant<bool, true> {
};

#endif
