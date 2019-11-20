// SkewedLatestGenerator.h

#ifndef _DBBENCHMARK_SKEWEDLATESTGENERATOR_H_
#define _DBBENCHMARK_SKEWEDLATESTGENERATOR_H_

#include "Generator.h"

#include <atomic>
#include <cstdint>

#include "CounterGenerator.h"
#include "ZipfianGenerator.h"

namespace dbbenchmark {
namespace generators {
/**
*   \brief Generate a popularity distribution of items, skewed to favor recent items significantly more than older items.
*   \details 
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class SkewedLatestGenerator : public Generator<uint64_t> {
public:
  explicit SkewedLatestGenerator(CounterGenerator &counter) :
      basis(counter), zipfian(this->basis.Last()) {
    Next();
  }
  
  uint64_t Next() override;
  uint64_t Last() override { return this->lastValue; }
private:
  CounterGenerator &basis;
  ZipfianGenerator zipfian;
  std::atomic<uint64_t> lastValue;
};
/**
* Generate the next string in the distribution, skewed Zipfian favoring the items most recently returned by
* the basis generator.
*/
inline uint64_t SkewedLatestGenerator::Next() {
  uint64_t max = this->basis.Last();
  return this->lastValue = max - this->zipfian.Next(max);
}

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_SKEWEDLATESTGENERATOR_H_
