// CounterGenerator.h

#ifndef _DBBENCHMARK_COUNTERGENERATOR_H_
#define _DBBENCHMARK_COUNTERGENERATOR_H_

#include "Generator.h"

#include <cstdint>
#include <atomic>

namespace dbbenchmark {
namespace generators {
/**
* \brief Genere a incremental integer values. (0,1,2,...)
* \details 
* \author Ozgun AY
* \version 1.0
* \date 21/11/2018
* \bug None so far
*/
class CounterGenerator : public Generator<uint64_t> {
public:
  explicit CounterGenerator(uint64_t start) : counterValue(start) { }
  uint64_t Next() override { return this->counterValue.fetch_add(1); }
  uint64_t Last() override { return this->counterValue.load() - 1; }
  /** Set a start point.
  * @param start Start point for counter.
  */
  void Set(uint64_t start) { this->counterValue.store(start); }

private:
  std::atomic<uint64_t> counterValue;
};

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_COUNTERGENERATOR_H_
