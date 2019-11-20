// DBFactory.h

#ifndef _DBBENCHMARK_DBFACTORY_H_
#define _DBBENCHMARK_DBFACTORY_H_

#include <memory>

#include "DB.h"
#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"

namespace dbbenchmark {
/**
 *   \brief DB Factory
 *   \details  This class will create a DB class object that specified with a 
 *      XML Property file.  
 *   \author Ozgun AY
 *   \version 1.0
 *   \date 21/11/2018
 *   \bug None so far
 */
class DBFactory {
public:
    std::shared_ptr<DB> CreateDB();

private:
    utility::programconfigurations::LayeredConfiguration* m_localConf;
};

} // namespace dbbenchmark

#endif // _DBBENCHMARK_DBFACTORY_H_

