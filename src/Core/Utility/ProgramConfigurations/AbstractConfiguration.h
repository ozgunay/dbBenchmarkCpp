// AbstractConfiguration.h

#ifndef _DBBENCHMARK_ABSTRACTCONFIGURATION_H_
#define _DBBENCHMARK_ABSTRACTCONFIGURATION_H_

#include <vector>
#include <utility>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/asio.hpp>

#include "Core/Utility/LoggerSink.hpp"

using boost::convert;
using boost::lexical_cast;

// https://stackoverflow.com/questions/4452136/how-do-i-use-boostlexical-cast-and-stdboolalpha-i-e-boostlexical-cast-b
namespace boost {
template<>
inline bool lexical_cast<bool, std::string>(const std::string& arg) {
	std::string temp(arg.length(), 'X');
	std::transform(arg.begin(), arg.end(), temp.begin(), ::tolower);
	std::istringstream ss(temp);
	bool b;
	ss >> std::boolalpha >> b;
	return b;
}
template<>
inline std::string lexical_cast<std::string, bool>(const bool& b) {
	std::ostringstream ss;
	ss << std::boolalpha << b;
	return ss.str();
}
struct cnv::by_default : cnv::lexical_cast {};
}

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
* @brief AbstractConfiguration is an abstract base class for different
* kinds of configurations.
* @details  AbstractConfiguration is an abstract base class for different
* kinds of configuration data, such as INI files, property files,
* XML configuration files or the Windows Registry.

* Configuration property keys have a hierarchical format, consisting
* of names separated by periods. The exact interpretation of key names
* is up to the actual subclass implementation of AbstractConfiguration.
* Keys are case sensitive.

* All public methods are synchronized, so the class is safe for multithreaded use.
* AbstractConfiguration implements reference counting based garbage collection.

* Subclasses must override the getRaw(), setRaw() and enumerate() methods.
* @author Poco / Modified by Ozgun AY
* @version 1.0
* @date 07/03/2019
* @bug None so far
*/
class AbstractConfiguration
{
public:
	typedef std::vector<std::string> Keys;
	/**
	* @brief A key-value pair, used as event argument.
	* @author Poco / Modified by Ozgun AY
	* @version 1.0
	* @date 07/03/2019
	* @bug None so far
	*/
	class KeyValue
	{
	public:
		/** Constructor
		* @param key
		* @param value */
		KeyValue(const std::string& key, std::string& value):
			_key(key),
			_value(value){
		}
		const std::string& key() const {
			return _key;
		}
		const std::string& value() const {
			return _value;
		}
		std::string& value() {
			return _value;
		}

	private:
		const std::string& _key;
		std::string& _value;
	};
	/** Constructor Creates the AbstractConfiguration. */
	AbstractConfiguration();
	/** Search configuration for if it has given key-value pair.
	* @param key Key to search for.
	* @return true if the property with the given key exists. */
	bool hasProperty(const std::string& key) const;
	/** Same as hasProperty().
	* @param key Key to search for.
	* @return true if the property with the given key exists. */
	bool hasOption(const std::string& key) const;
	/** Same as hasProperty().
	* @param key Key to search for.
	* @return true if the property with the given key exists. */
	bool has(const std::string& key) const;
	/** Get stored value of a given key as a string.
	* Throws a NotFoundException if the key does not exist.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* @param key Key to search for.
	* @return Returns the string value of the property with the given name. */
	std::string getString(const std::string& key) const;
	/** Get stored value of a given key as a string. If a property with the
	* given key exists, returns the property's string value, otherwise returns
	* the given default value.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	std::string getString(const std::string& key, const std::string& defaultValue) const;
	/** Get stored value of a given key as integer. If a property with the
	* given key exists, returns the int value of the property with the given name.
	* Throws a NotFoundException if the key does not exist.
	* Throws a SyntaxException if the property can not be converted
	* to an int.
	* Numbers starting with 0x are treated as hexadecimal.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* @param key Key to search for.
	* @return Returns the string value of the property with the given name. */
	int getInt(const std::string& key) const;
	/** Get stored value of a given key as unsigned integer. If a property with the
	* given key exists, returns the int value of the property with the given name.
	* Throws a NotFoundException if the key does not exist.
	* Throws a SyntaxException if the property can not be converted
	* to an unsigned int.
	* Numbers starting with 0x are treated as hexadecimal.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* @param key Key to search for.
	* @return Returns the string value of the property with the given name. */
	unsigned int getUInt(const std::string& key) const;
	/** Get stored value of a given key as integer. If a property with the
	* given key exists, returns the property's int value, otherwise returns
	* the given default value.
	* Throws a SyntaxException if the property can not be converted
	* to an int.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	int getInt(const std::string& key, int defaultValue) const;
	/** Get stored value of a given key as unsigned integer. If a property with the
	* given key exists, returns the property's int value, otherwise returns
	* the given default value.
	* Throws a SyntaxException if the property can not be converted
	* to an unsigned int.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	unsigned int getUInt(const std::string& key, unsigned int defaultValue) const;
	/** Get stored value of a given key as double. If a property with the
	* given key exists, returns the property's double value.
	* Throws a NotFoundException if the key does not exist.
	* Throws a SyntaxException if the property can not be converted
	* to an double.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	double getDouble(const std::string& key) const;
	/** Get stored value of a given key as double. If a property with the
	* given key exists, returns the property's double value, otherwise returns
	* the given default value.
	* Throws a SyntaxException if the property can not be converted
	* to an double.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	double getDouble(const std::string& key, double defaultValue) const;
	/** Get stored value of a given key as boolean. If a property with the
	* given key exists, returns the property's boolean value.
	* Throws a NotFoundException if the key does not exist.
	* Throws a SyntaxException if the property can not be converted
	* to an boolean.
	* The following string values can be converted into a boolean:
	*   - numerical values: non zero becomes true, zero becomes false
	*   - strings: true, yes, on become true, false, no, off become false
	* Case does not matter.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	bool getBool(const std::string& key) const;
	/** Get stored value of a given key as boolean. If a property with the
	* given key exists, returns the property's boolean value, otherwise returns
	* the given default value.
	* Throws a SyntaxException if the property can not be converted
	* to an boolean.
	* The following string values can be converted into a boolean:
	*   - numerical values: non zero becomes true, zero becomes false
	*   - strings: true, yes, on become true, false, no, off become false
	* Case does not matter.
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	bool getBool(const std::string& key, bool defaultValue) const;
	/** Get stored value of a given key as g3logger::LEVELS. If a property with the
	* given key exists, returns the property's g3logger::LEVELS.
	* Throws a NotFoundException if the key does not exist.
	* Throws a SyntaxException if the property can not be converted
	* to an g3logger::LEVELS.
	* The following string values can be converted into a g3logger::LEVELS:
	*   - ALARM_NORMAL, DEBUG, INFO, WARNING, ERROR, ALARM_CRITICAL, ALARM_NORMAL,
	*	FATAL
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	LEVELS getLogLevel(const std::string& key) const;
	/** Get stored value of a given key as g3logger::LEVELS. If a property with the
	* given key exists, returns the property's g3logger::LEVELS, otherwise returns
	* the given default value.
	* Throws a SyntaxException if the property can not be converted
	* to an g3logger::LEVELS.
	* The following string values can be converted into a g3logger::LEVELS:
	*   - ALARM_NORMAL, DEBUG, INFO, WARNING, ERROR, ALARM_CRITICAL, ALARM_NORMAL,
	*	FATAL
	* If the value contains references to other properties (${<property>}), these
	* are expanded.
	* Numbers starting with 0x are treated as hexadecimal.
	* @param key Key to search for.
	* @param defaultValue Default value to return if key not found
	* @return Returns the string value of the property with the given name. */
	LEVELS getLogLevel(const std::string& key, LEVELS defaultValue) const;
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setString(const std::string& key, const std::string& value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setInt(const std::string& key, int value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setUInt(const std::string& key, unsigned int value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setDouble(const std::string& key, double value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setBool(const std::string& key, bool value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setLogLevel(const std::string& key, LEVELS value);
		/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setLogLevel(const std::string& key, std::string value);
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Throws an exception if value can not be converted to a string.
	* @param key Key to be set.
	* @param value Property value to be set. */
	virtual void setIp(const std::string& key, std::string value);
	/** Returns in range the names of all keys at root level.
	* @param range Fills the vector with root level keys.*/
	void keys(Keys& range) const;
	/** Returns in range the names of all subkeys under the given key.
	*  If an empty key is passed, all root level keys are returned.
	* @param key Root level key.
	* @param range Fills the vector with root level keys.*/
	void keys(const std::string& key, Keys& range) const;
	/** Replaces all occurrences of ${<property>} in value with the
	* value of the <property>. If <property> does not exist,
	* nothing is changed.
	* If a circular property reference is detected, a
	* CircularReferenceException will be thrown.
	* @param value String that will be expand.*/
	std::string expand(const std::string& value) const;
	/** Removes the property with the given key.
	* Does nothing if the key does not exist.
	* @param key Key as a string to be deleted.*/
	void remove(const std::string& key);
	/** Destructor */
	virtual ~AbstractConfiguration();

protected:
	/** If the property with the given key exists, stores the property's value
	* in value and returns true. Otherwise, returns false.
	* Must be overridden by subclasses.
	* @param key Key as string.
	* @param value Saved Property's value */
	virtual bool getRaw(const std::string& key, std::string& value) const = 0;
	/** Sets the property with the given key to the given value.
	* An already existing value for the key is overwritten.
	* Must be overridden by subclasses.
	* @param key Key as string.
	* @param value Property's value */
	virtual void setRaw(const std::string& key, const std::string& value) = 0;
	/** Returns in range the names of all subkeys under the given key.
	* If an empty key is passed, all root level keys are returned.
	* @param key Key as string.
	* @param range Keys as vector of strings. */
	virtual void enumerate(const std::string& key, Keys& range) const = 0;
	/** Removes the property with the given key.
	* Does nothing if the key does not exist.
	* Should be overridden by subclasses; the default
	* implementation throws a Utility::NotImplementedException.
	* @param key Key as string. */
	virtual void removeRaw(const std::string& key);
	/** Removes the property with the given key.
	* Does nothing if the key does not exist.
	* Should be overridden by subclasses; the default
	* implementation throws a Utility::NotImplementedException.
	* @param key Key as string. */
	static int parseInt(const std::string& value);
	/** Function that converts std::string to int using boost::convert
	* Throws an exception if value can not be converted to a integer.
	* @param value Value to be convert */
	static unsigned parseUInt(const std::string& value);
	/** Function that converts std::string to boolean using boost::convert
	* Throws an exception if value can not be converted to a boolean.
	* @param value Value to be convert */
	static bool parseBool(const std::string& value);
	/** Function that converts std::string to g3Logger::LEVELS using boost::convert
	* Throws an exception if value can not be converted to a g3Logger::LEVELS.
	* @param value Value to be convert */
	static LEVELS parseLevel(const std::string& value);
	/** Function that store keyivalue pair. This function has interval lock mechanism.
	* @param key Key to be stored.
	* @param value Value to be stored. */
	void setRawWithEvent(const std::string& key, std::string value);

private:
	std::string internalExpand(const std::string& value) const;
	std::string uncheckedExpand(const std::string& value) const;

	AbstractConfiguration(const AbstractConfiguration&);
	AbstractConfiguration& operator = (const AbstractConfiguration&);

	mutable int m_depth;
	mutable std::mutex m_mutex;

	friend class MapConfiguration;
	friend class LayeredConfiguration;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_ABSTRACTCONFIGURATION_H_
