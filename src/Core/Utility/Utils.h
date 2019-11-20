// Utils.h

#ifndef _DBBENCHMARK_UTILS_H_
#define _DBBENCHMARK_UTILS_H_

#include <algorithm>
#include <cstdint>
#include <exception>
#include <random>

namespace dbbenchmark {
namespace utility {
/**
*   \brief General functions and const values that used in entire project.
*   \details 
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/

typedef std::pair<std::string, std::string> stringPair;

const uint64_t kFNVOffsetBasis64 = 0xCBF29CE484222325;
const uint64_t kFNVPrime64 = 1099511628211;

inline uint64_t FNVHash64(uint64_t val) {
  uint64_t hash = kFNVOffsetBasis64;

  for (int i = 0; i < 8; i++) {
    uint64_t octet = val & 0x00ff;
    val = val >> 8;

    hash = hash ^ octet;
    hash = hash * kFNVPrime64;
  }
  return hash;
}

inline uint64_t Hash(uint64_t val) { return FNVHash64(val); }

inline double RandomDouble(double min = 0.0, double max = 1.0) {
  static std::default_random_engine generator;
  static std::uniform_real_distribution<double> uniform(min, max);
  return uniform(generator);
}

///
/// Returns an ASCII code that can be printed to desplay
///
inline char RandomPrintChar() {
  return rand() % 94 + 33;
}

} // namespace utility
} // namespace dbtester

#endif // _DBBENCHMARK_UTILS_H_
