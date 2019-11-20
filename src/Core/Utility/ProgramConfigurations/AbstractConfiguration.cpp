// AbstractConfiguration.cpp

#include "AbstractConfiguration.h"
#include "Core/Utility/Exception.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {

AbstractConfiguration::AbstractConfiguration():
	m_depth(0) {
}

AbstractConfiguration::~AbstractConfiguration() {
}

bool AbstractConfiguration::hasProperty(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	return getRaw(key, value);
}

bool AbstractConfiguration::hasOption(const std::string& key) const {
	return hasProperty(key);
}

bool AbstractConfiguration::has(const std::string& key) const {
	return hasProperty(key);
}

std::string AbstractConfiguration::getString(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		throw NotFoundException(key);
}

std::string AbstractConfiguration::getString(const std::string& key,
	const std::string& defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return internalExpand(value);
	else
		return defaultValue;
}

int AbstractConfiguration::getInt(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		throw NotFoundException(key);
}

int AbstractConfiguration::getInt(const std::string& key, int defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseInt(internalExpand(value));
	else
		return defaultValue;
}

unsigned AbstractConfiguration::getUInt(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		throw NotFoundException(key);
}

unsigned AbstractConfiguration::getUInt(const std::string& key,
	unsigned defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseUInt(internalExpand(value));
	else
		return defaultValue;
}

double AbstractConfiguration::getDouble(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return convert<double>(internalExpand(value)).value();
	else
		throw NotFoundException(key);
}

double AbstractConfiguration::getDouble(const std::string& key, double defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return convert<float>(internalExpand(value)).value();
	else
		return defaultValue;
}

bool AbstractConfiguration::getBool(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseBool(internalExpand(value));
	else
		throw NotFoundException(key);
}

bool AbstractConfiguration::getBool(const std::string& key, bool defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return convert<bool>(internalExpand(value)).value();
	else
		return defaultValue;
}

LEVELS AbstractConfiguration::getLogLevel(const std::string& key) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseLevel(value);
	else
		throw NotFoundException(key);
}

LEVELS AbstractConfiguration::getLogLevel(const std::string& key,
	LEVELS defaultValue) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string value;
	if (getRaw(key, value))
		return parseLevel(value);
	else
		return defaultValue;
}

void AbstractConfiguration::setString(const std::string& key, const std::string& value) {
	setRawWithEvent(key, value);
}

void AbstractConfiguration::setInt(const std::string& key, int value) {
	setRawWithEvent(key, convert<std::string>(value).value());
}

void AbstractConfiguration::setUInt(const std::string& key, unsigned int value) {
	setRawWithEvent(key, convert<std::string>(value).value());
}

void AbstractConfiguration::setDouble(const std::string& key, double value) {
	setRawWithEvent(key, convert<std::string>(value).value());
}

void AbstractConfiguration::setBool(const std::string& key, bool value) {
	setRawWithEvent(key, value ? "true" : "false");
}


void AbstractConfiguration::setLogLevel(const std::string& key, LEVELS value) {
	setRawWithEvent(key, value.text);
}

void AbstractConfiguration::setLogLevel(const std::string& key, std::string value) {
	LEVELS temp = parseLevel(value);
	setRawWithEvent(key, temp.text);
}

void AbstractConfiguration::setIp(const std::string& key, std::string value) {
	boost::system::error_code ec;
	boost::asio::ip::address::from_string(value, ec);
	if ( ec )
		throw utility::SyntaxException(key);

	setRawWithEvent(key, value);
}

void AbstractConfiguration::keys(Keys& range) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::string key;
	range.clear();
	enumerate(key, range);
}


void AbstractConfiguration::keys(const std::string& key, Keys& range) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	range.clear();
	enumerate(key, range);
}
namespace {
	class AutoCounter
	{
	public:
		AutoCounter(int& count): _count(count) {
			++_count;
		}
		~AutoCounter() {
			--_count;
		}
	private:
		int& _count;
	};
}

std::string AbstractConfiguration::expand(const std::string& value) const {
	std::lock_guard<std::mutex> lock(m_mutex);

	return internalExpand(value);
}

void AbstractConfiguration::remove(const std::string& key) {
	std::lock_guard<std::mutex> lock(m_mutex);
	removeRaw(key);
}

void AbstractConfiguration::removeRaw(const std::string& key) {
	throw NotImplementedException("removeRaw()");
}

std::string AbstractConfiguration::internalExpand(const std::string& value) const {
	AutoCounter counter(m_depth);
	if (m_depth > 10) throw CircularReferenceException("Too many property references encountered");
	return uncheckedExpand(value);
}

std::string AbstractConfiguration::uncheckedExpand(const std::string& value) const {
	std::string result;
	std::string::const_iterator it  = value.begin();
	std::string::const_iterator end = value.end();
	while (it != end) {
		if (*it == '$') {
			++it;
			if (it != end && *it == '{') {
				++it;
				std::string prop;
				while (it != end && *it != '}') prop += *it++;
				if (it != end) ++it;
				std::string value;
				if (getRaw(prop, value)) {
					result.append(internalExpand(value));
				} else {
					result.append("${");
					result.append(prop);
					result.append("}");
				}
			} else {
				result += '$';
			}
		} else {
			result += *it++;
		}
	}
	return result;
}

int AbstractConfiguration::parseInt(const std::string& value) {
	return convert<int>(value).value();
}


unsigned AbstractConfiguration::parseUInt(const std::string& value) {
	return convert<unsigned int>(value).value();
}

bool AbstractConfiguration::parseBool(const std::string& value) {
	return convert<bool>(value).value();
}

LEVELS AbstractConfiguration::parseLevel(const std::string& value) {
	if(value == INFO.text)
		return INFO;
	else if(value == DEBUG.text)
		return DEBUG;
	else if(value == WARNING.text)
		return WARNING;
	else if(value == FATAL.text)
		return FATAL;
	else
		throw utility::Exception("Invalid log level: " + value + ". ");
}

void AbstractConfiguration::setRawWithEvent(const std::string& key, std::string value) {
	KeyValue kv(key, value);
	std::lock_guard<std::mutex> lock(m_mutex);
	setRaw(key, value);
}

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark