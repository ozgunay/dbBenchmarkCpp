// ScrambledZipfianGenerator.h

#ifndef _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATOR_H_
#define _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATOR_H_

#include "Generator.h"

#include <atomic>
#include <cstdint>

#include "Core/Utility/Utils.h"
#include "ZipfianGenerator.h"

namespace dbbenchmark {
namespace generators {
/**
*   \brief A generator of a zipfian distribution.
*   \details It produces a sequence of items, such that some items are more popular than
*       others, according to a zipfian distribution. When you construct an instance of this class, you specify the number
*       of items in the set to draw from, either by specifying an itemcount (so that the sequence is of items from 0 to
*       itemcount-1) or by specifying a min and a max (so that the sequence is of items from min to max inclusive). After
*       you construct the instance, you can change the number of items by calling nextInt(itemcount) or nextLong(itemcount).
*       <p>
*       Unlike @ZipfianGenerator, this class scatters the "popular" items across the itemspace. Use this, instead of
*       @ZipfianGenerator, if you don't want the head of the distribution (the popular items) clustered together.
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class ScrambledZipfianGenerator : public Generator<uint64_t> {
public:
  /**
  * Create a zipfian generator for items between min and max.
  *
  * @param min The smallest integer to generate in the sequence.
  * @param max The largest integer to generate in the sequence.
  */
  ScrambledZipfianGenerator(uint64_t min, uint64_t max,
      double zipfian_const = ZipfianGenerator::kZipfianConst) :
      base(min), numItems(max - min + 1),
      generator(min, max, zipfian_const) { }
  /**
  * Create a zipfian generator for the specified number of items.
  * @param items The number of items in the distribution.
  */
  explicit ScrambledZipfianGenerator(uint64_t num_items) :
      ScrambledZipfianGenerator(0, num_items - 1) { }
  
  uint64_t Next() override;
  uint64_t Last() override;
  
private:
  const uint64_t base;
  const uint64_t numItems;
  ZipfianGenerator generator;

  uint64_t Scramble(uint64_t value) const;
};

inline uint64_t ScrambledZipfianGenerator::Scramble(uint64_t value) const {
  return this->base + utility::FNVHash64(value) % this->numItems;
}
/**
* Return the next long in the sequence.
*/
inline uint64_t ScrambledZipfianGenerator::Next() {
  return Scramble(this->generator.Next());
}

inline uint64_t ScrambledZipfianGenerator::Last() {
  return Scramble(this->generator.Last());
}

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATOR_H_
