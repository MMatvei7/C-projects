#ifndef ARRAY_TRAITS__ARRAY_TRAITS_H_
#define ARRAY_TRAITS__ARRAY_TRAITS_H_
#include <type_traits>
#include <cstdint>

template <typename T>
struct TypeDimension {
  static constexpr int64_t kValue = 0;
};

template <typename T, int64_t N>
struct TypeDimension<T[N]> {
  static constexpr int64_t kValue = TypeDimension<T>::kValue + 1;
};

template <typename T>
struct TypeDimension<T[]> {
  static constexpr int64_t kValue = TypeDimension<T>::kValue + 1;
};

template <typename T>
struct UpArraySize {
  static constexpr int64_t kValue = 1;
};

template <typename T, int64_t N>
struct UpArraySize<T[N]> {
  static constexpr int64_t kValue = N;
};

template <typename T>
struct UpArraySize<T[]> {
  static constexpr int64_t kValue = 0;
};

template <typename T>
struct ArraySize {
  static constexpr int64_t kValue = 1;
};

template <typename T, int64_t N>
struct ArraySize<T[N]> {
  static constexpr int64_t kValue = ArraySize<T>::kValue * N;
};

template <typename T>
struct ArraySize<T[]> {
  static constexpr int64_t kValue = 0;
};

template <typename T>
struct UpArrayType {
  using Type = T;
};

template <typename T, int64_t N>
struct UpArrayType<T[N]> {
  using Type = T;
};

template <typename T>
struct UpArrayType<T[]> {
  using Type = T;
};

template <typename T>
struct ArrayType {
  using Type = T;
};

template <typename T, int64_t N>
struct ArrayType<T[N]> {
  using Type = typename ArrayType<T>::Type;
};

template <typename T>
struct ArrayType<T[]> {
  using Type = typename ArrayType<T>::Type;
};

template <typename T>
inline constexpr bool kIsArrayV = (TypeDimension<T>::kValue > 0);

template <typename T>
inline constexpr int64_t kRankV = TypeDimension<T>::kValue;

template <typename T>
inline constexpr int64_t kSizeV = UpArraySize<T>::kValue;

template <typename T>
inline constexpr int64_t kTotalSizeV = ArraySize<T>::kValue;

template <typename T>
using RemoveArrayT = typename UpArrayType<T>::Type;

template <typename T>
using RemoveAllArraysT = typename ArrayType<T>::Type;

#endif
