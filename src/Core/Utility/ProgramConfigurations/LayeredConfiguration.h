// LayeredConfiguration.h

#ifndef _DBBENCHMARK_LAYEREDCONFIGURATION_H_
#define _DBBENCHMARK_LAYEREDCONFIGURATION_H_

#include <list>
#include <map>

#include "AbstractConfiguration.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
* @brief  A LayeredConfiguration consists of a number of
* AbstractConfigurations.
* @details  When reading a configuration property in a LayeredConfiguration,
* all added configurations are searched, in order of their priority.
* Configurations with lower priority values have precedence.
*
* When setting a property, the property is always written to the first writeable
* configuration (see addWriteable()).
* If no writeable configuration has been added to the LayeredConfiguration, and an
* attempt is made to set a property, a RuntimeException is thrown.
*
* Every configuration added to the LayeredConfiguration has a priority value.
* The priority determines the position where the configuration is inserted,
* with lower priority values coming before higher priority values.
*
* If no priority is specified, a priority of 0 is assumed.
* @author Poco / Modified by Ozgun AY
* @version 1.0
* @date 09/03/2019
* @bug None so far
*/
class LayeredConfiguration: public AbstractConfiguration
{
public:
	typedef std::shared_ptr<AbstractConfiguration> ConfigPtr;
	/** Adds a read-only configuration to the back of the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added. */
	void add(ConfigPtr pConfig);
	/** Adds a read-only configuration with the given label to the back of the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param label Description of the AbstractConfigution object. */
	void add(ConfigPtr pConfig, const std::string& label);
	/** Adds a read-only configuration to the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param priority Prority of the AbstractConfigution object. */
	void add(ConfigPtr pConfig, int priority);
	/** Adds a read-only configuration to the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param label Description of the AbstractConfigution object.
	* @param priority Prority of the AbstractConfigution object.*/
	void add(ConfigPtr pConfig, const std::string& label, int priority);
	/** Adds a configuration to the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param priority Prority of the AbstractConfigution object.
	* @param writeable Set true for writeable.*/
	void add(ConfigPtr pConfig, int priority, bool writeable);
	/** Adds a configuration with the given label to the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param label Description of the AbstractConfigution object.
	* @param priority Prority of the AbstractConfigution object.
	* @param writeable Set true for writeable.*/
	void add(ConfigPtr pConfig, const std::string& label, int priority, bool writeable);
	/** Adds a writeable configuration to the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added.
	* @param priority Prority of the AbstractConfigution object. */
	void addWriteable(ConfigPtr pConfig, int priority);
	/** Finds and returns the configuration with the given label.
	* Returns null if no such configuration can be found.
	* @param label Description of the AbstractConfigution object to be searched. */
	ConfigPtr find(const std::string& label) const;
	//@ deprecated
	/**  Adds a read-only configuration to the front of the LayeredConfiguration.
	* The LayeredConfiguration does not take ownership of the given
	* configuration. In other words, the configuration's reference
	* count is incremented.
	* @param pConfig shared_ptr of the AbstractConfigution object to be added. */
	void addFront(ConfigPtr pConfig);
	/**  Adds a read-only configuration to the front of the LayeredConfiguration.
	* Does nothing if the given configuration is not part of the LayeredConfiguration.
	* @param pConfig shared_ptr of the AbstractConfigution object to be removed. */
	void removeConfiguration(ConfigPtr pConfig);
	/**  Get all configurations stored in LayeredConfiguration object
	 * as a map<key,value>.*/
	const std::map<std::string, std::string> getAllConfigurationsAsMap() const;
	/**  LayeredConfiguration is Meyers' Singleton object. With instance method,
	* if no object created before, a new object created. If there is an object exists
	* returns referance of that object.
	* @return Referance to the singleton object*/
	static LayeredConfiguration& Instance();


protected:
	struct ConfigItem
	{
		ConfigPtr   pConfig;
		int         priority;
		bool        writeable;
		std::string label;
	};

	bool getRaw(const std::string& key, std::string& value) const;
	void setRaw(const std::string& key, const std::string& value);
	void enumerate(const std::string& key, Keys& range) const;
	void removeRaw(const std::string& key);

	int lowest() const;
	int highest() const;
	void insert(const ConfigItem& item);



private:
	LayeredConfiguration() = default;
	LayeredConfiguration(const LayeredConfiguration&);
	LayeredConfiguration operator&(const LayeredConfiguration&) = delete;
	~LayeredConfiguration() = default;

	typedef std::list<ConfigItem> ConfigList;

	ConfigList _configs;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_LAYEREDCONFIGURATION_H_
