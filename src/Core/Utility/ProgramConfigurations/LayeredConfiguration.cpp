// LayeredConfiguration.cpp

#include <set>

#include "LayeredConfiguration.h"
#include "Core/Utility/Exception.h"

using dbbenchmark::utility::RuntimeException;

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {

LayeredConfiguration& LayeredConfiguration::Instance() {
	static LayeredConfiguration singletonConfiguration;
    return singletonConfiguration;
}

void LayeredConfiguration::add(ConfigPtr pConfig) {
	add(pConfig, highest(), false);
}

void LayeredConfiguration::add(ConfigPtr pConfig, const std::string& label) {
	add(pConfig, label, highest(), false);
}

void LayeredConfiguration::add(ConfigPtr pConfig, int priority) {
	add(pConfig, priority, false);
}

void LayeredConfiguration::add(ConfigPtr pConfig, const std::string& label, int priority) {
	add(pConfig, label, priority, false);
}

void LayeredConfiguration::addFront(ConfigPtr pConfig) {
	add(pConfig, lowest(), false);
}

void LayeredConfiguration::addWriteable(ConfigPtr pConfig, int priority) {
	add(pConfig, priority, true);
}

void LayeredConfiguration::add(ConfigPtr pConfig, int priority, bool writeable) {
	add(pConfig, std::string(), priority, writeable);
}

void LayeredConfiguration::add(ConfigPtr pConfig, const std::string& label, int priority, bool writeable) {
	ConfigItem item;
	item.pConfig   = ConfigPtr(pConfig);
	item.priority  = priority;
	item.writeable = writeable;
	item.label     = label;

	ConfigList::iterator it = _configs.begin();
	while (it != _configs.end() && it->priority < priority)
		++it;

	_configs.insert(it, item);
}

void LayeredConfiguration::removeConfiguration(ConfigPtr pConfig) {
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it) {
		if (it->pConfig == pConfig) {
			_configs.erase(it);
			break;
		}
	}
}

LayeredConfiguration::ConfigPtr LayeredConfiguration::find(const std::string& label) const {
	for (ConfigList::const_iterator it = _configs.begin(); it != _configs.end(); ++it) {
		if (it->label == label) {
			return it->pConfig;
		}
	}
	return 0;
}

bool LayeredConfiguration::getRaw(const std::string& key, std::string& value) const {
	for (ConfigList::const_iterator it = _configs.begin(); it != _configs.end(); ++it) {
		if (it->pConfig->getRaw(key, value))
			return true;
	}
	return false;
}
void LayeredConfiguration::setRaw(const std::string& key, const std::string& value) {
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it) {
		if (it->writeable) {
			it->pConfig->setRaw(key, value);
			return;
		}
	}
	throw RuntimeException("No writeable configuration object to store the property", key);
}

void LayeredConfiguration::enumerate(const std::string& key, Keys& range) const {
	std::set<std::string> keys;
	for (ConfigList::const_iterator itc = _configs.begin(); itc != _configs.end(); ++itc) {
		Keys partRange;
		itc->pConfig->enumerate(key, partRange);
		for (Keys::const_iterator itr = partRange.begin(); itr != partRange.end(); ++itr) {
			if (keys.find(*itr) == keys.end()) {
				range.push_back(*itr);
				keys.insert(*itr);
			}
		}
	}
}

void LayeredConfiguration::removeRaw(const std::string& key) {
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it) {
		if (it->writeable) {
			it->pConfig->remove(key);
			return;
		}
	}
}

int LayeredConfiguration::lowest() const {
	if (_configs.empty())
		return 0;
	else
		return _configs.front().priority - 1;
}

int LayeredConfiguration::highest() const {
	if (_configs.empty())
		return 0;
	else
		return _configs.back().priority + 1;
}

const std::map<std::string, std::string> LayeredConfiguration::getAllConfigurationsAsMap() const {
	std::map<std::string, std::string> tempMap;
	for (ConfigList::const_reverse_iterator itMap = _configs.rbegin(); itMap != _configs.rend(); ++itMap) {
		std::function<void(const std::string&)> recursiveSubKeyFinder;
		recursiveSubKeyFinder = [&tempMap, &itMap, &recursiveSubKeyFinder](const std::string& base)->void {
			Keys keysTemp;
			itMap->pConfig->keys(base, keysTemp);
			if(keysTemp.empty()) {
				if(itMap->pConfig->hasProperty(base)) {
					tempMap[base] = itMap->pConfig->getString(base);
				}
			}
			else {
				for(auto const& it : keysTemp) {
					std::string fullKey = base;
					if (!fullKey.empty()) fullKey += '.';
					fullKey.append(it);
					recursiveSubKeyFinder(fullKey);
				}
			}
		};
		std::string emptyBase;
		recursiveSubKeyFinder(emptyBase);
	}
	return tempMap;
}

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark