// MapConfiguration.h

#ifndef _DBBENCHMARK_MAPCONFIGURATION_H_
#define _DBBENCHMARK_MAPCONFIGURATION_H_

#include <map>

#include "AbstractConfiguration.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
* @brief An implementation of AbstractConfiguration that stores configuration
* data in a map.
* @details
* @author Poco / Modified by Ozgun AY
* @version 1.0
* @date 09/03/2019
* @bug None so far
*/
class MapConfiguration: public AbstractConfiguration
{
public:
	/** Constructor
	*  Creates an empty MapConfiguration.
	*/
	MapConfiguration();
	/** Copies all configuration properties to the given configuration.
	* @param config Target configuration. */
	void copyTo(AbstractConfiguration& config);
	/** Clears the configuration.
	* @param config Key to be set.
	* @param value Property value to be set. */
	void clear();
	/** Destructor */
	~MapConfiguration();

protected:
	typedef std::map<std::string, std::string> StringMap;
	typedef StringMap::const_iterator iterator;

	bool getRaw(const std::string& key, std::string& value) const override;
	void setRaw(const std::string& key, const std::string& value) override;
	void enumerate(const std::string& key, Keys& range) const override;
	void removeRaw(const std::string& key) override;

	iterator begin() const;
	iterator end() const;

private:
	StringMap _map;
	mutable std::mutex _mutex;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_MAPCONFIGURATION_H_
