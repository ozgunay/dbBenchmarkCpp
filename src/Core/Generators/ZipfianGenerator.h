// ZipfianGenerator.h

#ifndef _DBBENCHMARK_ZIPFIANGENERATOR_H_
#define _DBBENCHMARK_ZIPFIANGENERATOR_H_

#include "Generator.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <mutex>

#include "Core/Utility/Utils.h"

namespace dbbenchmark {
namespace generators {
/**
*   \brief  A generator of a zipfian distribution.
*   \details It produces a sequence of items, such that some items are more popular than
*   others, according to a zipfian distribution. When you construct an instance of this class, you specify the number
*   of items in the set to draw from, either by specifying an itemcount (so that the sequence is of items from 0 to
*   itemcount-1) or by specifying a min and a max (so that the sequence is of items from min to max inclusive). After
*   you construct the instance, you can change the number of items by calling nextInt(itemcount) or nextLong(itemcount).
*
*   Note that the popular items will be clustered together, e.g. item 0 is the most popular, item 1 the second most
*   popular, and so on (or min is the most popular, min+1 the next most popular, etc.) If you don't want this clustering,
*   and instead want the popular items scattered throughout the item space, then use ScrambledZipfianGenerator instead.
*
*   Be aware: initializing this generator may take a long time if there are lots of items to choose from (e.g. over a
*   minute for 100 million objects). This is because certain mathematical values need to be computed to properly
*   generate a zipfian skew, and one of those values (zeta) is a sum sequence from 1 to n, where n is the itemcount.
*   Note that if you increase the number of items in the set, we can compute a new zeta incrementally, so it should be
*   fast unless you have added millions of items. However, if you decrease the number of items, we recompute zeta from
*   scratch, so this can take a long time.
*
*   The algorithm used here is from "Quickly Generating Billion-Record Synthetic Databases", Jim Gray et al, SIGMOD 1994.
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class ZipfianGenerator : public Generator<uint64_t> {
public:
  constexpr static const double kZipfianConst = 0.99;
  static const uint64_t kMaxNumItems = (UINT64_MAX >> 24);
  /**
 * Create a zipfian generator for items between min and max (inclusive) for the specified zipfian constant.
 * @param min The smallest integer to generate in the sequence.
 * @param max The largest integer to generate in the sequence.
 * @param zipfian_const The zipfian constant to use.
 */
  ZipfianGenerator(uint64_t min, uint64_t max,
                  double zipfian_const = kZipfianConst) :
      numItems(max - min + 1), base(min), theta(zipfian_const),
      zetaN(0), nforZeta(0) {
    assert(this->numItems >= 2 && this->numItems < kMaxNumItems);
    this->zeta2 = Zeta(2, this->theta);
    this->alpha = 1.0 / (1.0 - this->theta);
    RaiseZeta(this->numItems);
    this->eta = Eta();
    
    Next();
  }
  /**
 * Create a zipfian generator for the specified number of items.
 * @param items The number of items in the distribution.
 */
  explicit ZipfianGenerator(uint64_t num_items) :
      ZipfianGenerator(0, num_items - 1, kZipfianConst) { }
  
  uint64_t Next(uint64_t num_items);
  
  uint64_t Next() override { return Next(this->numItems); }

  uint64_t Last() override;
  
private:
  /**
  * Compute the zeta constant needed for the distribution.
  * Remember the number of items, so if it is changed, we can recompute zeta.
  * @param num The number of items to compute zeta over.
  */
  void RaiseZeta(uint64_t num) {
    assert(num >= this->nforZeta);
    this->zetaN = Zeta(this->nforZeta, num, this->theta, this->zetaN);
    this->nforZeta = num;
  }
  
  double Eta() {
    return (1 - std::pow(2.0 / this->numItems, 1 - this->theta)) /
        (1 - this->zeta2 / this->zetaN);
  }

  /**
  * Calculate the zeta constant needed for a distribution.
  * Do this incrementally from the last_num of items to the cur_num.
  * Use the zipfian constant as theta. Remember the new number of items
  * so that, if it is changed, we can recompute zeta.
  */
  static double Zeta(uint64_t last_num, uint64_t cur_num,
                    double theta, double last_zeta) {
    double zeta = last_zeta;
    for (uint64_t i = last_num + 1; i <= cur_num; ++i) {
      zeta += 1 / std::pow(i, theta);
    }
    return zeta;
  }
  
  static double Zeta(uint64_t num, double theta) {
    return Zeta(0, num, theta, 0);
  }
  
  uint64_t numItems; /// Number of items
  uint64_t base; /// Min number of items to generate
  
  // Computed parameters for generating the distribution
  double theta, zetaN, eta, alpha, zeta2;
  uint64_t nforZeta; /// Number of items used to compute zeta_n
  uint64_t lastValue;
  std::mutex mutex;
};

/**
* Generate the next item as a long.
*
* @param num The number of items in the distribution.
* @return The next item in the sequence.
*/
inline uint64_t ZipfianGenerator::Next(uint64_t num) {
  assert(num >= 2 && num < kMaxNumItems);
  std::lock_guard<std::mutex> lock(this->mutex);

  if (num > this->nforZeta) { // Recompute zeta_n and eta
    RaiseZeta(num);
    this->eta = Eta();
  }
  
  double u = utility::RandomDouble();
  double uz = u * this->zetaN;
  
  if (uz < 1.0) {
    return this->lastValue = 0;
  }
  
  if (uz < 1.0 + std::pow(0.5, this->theta)) {
    return lastValue = 1;
  }

  return this->lastValue = this->base + num * std::pow(this->eta * u - this->eta + 1, this->alpha);
}

inline uint64_t ZipfianGenerator::Last() {
  std::lock_guard<std::mutex> lock(this->mutex);
  return this->lastValue;
}

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_ZIPFIANGENERATOR_H_
