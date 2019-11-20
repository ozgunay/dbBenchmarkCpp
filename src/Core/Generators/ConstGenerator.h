#ifndef _DBBENCHMARK_CONSTGENERATOR_H_
#define _DBBENCHMARK_CONSTGENERATOR_H_

#include "Generator.h"

#include <cstdint>

namespace dbbenchmark {
namespace generators {

/**
*   \brief A trivial integer generator that always returns the same value.
*   \details 
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class ConstGenerator : public Generator<uint64_t> {
public:
  /** Constructor
  * @param constant The integer that this generator will always return.
  */
  explicit ConstGenerator(int constant) : constantValue(constant) { }
  uint64_t Next() override { return constantValue; }
  uint64_t Last() override { return constantValue; }

private:
  uint64_t constantValue;
};

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_CONSTGENERATOR_H_
