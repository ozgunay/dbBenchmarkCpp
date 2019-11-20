// Generator.h

#ifndef  _DBBENCHMARK_GENERATOR_H_
#define  _DBBENCHMARK_GENERATOR_H_

#include <cstdint>
#include <string>

namespace dbbenchmark {
namespace generators {
/**
*   \brief Interface for generators.
*   \details  This template generator is an interface for content generator 
*            classes for tests.
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
template <typename Value>
class Generator {
public:
  /** Get Next value
  * @return Return the next value.
  */
  virtual Value Next() = 0;
  /** Get Last value
  * @return Return the last value.
  */
  virtual Value Last() = 0;
  /** Destructor
  */
  virtual ~Generator() { }
};

} // namespace generators
} // namespace dbbenchmark

#endif // _DBBENCHMARK_GENERATOR_H_
