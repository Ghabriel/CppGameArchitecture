#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include <iostream>
#include <memory>
#include <typeindex>
#include "Scanner.hpp"

class JsonValue;

namespace __detail {
    using Array = std::vector<JsonValue>;
    using Map = std::unordered_map<std::string, JsonValue>;
}

class JsonValue {
    using Array = __detail::Array;
    using Map = __detail::Map;
 public:
    template<typename T>
    JsonValue(const T&);

    template<typename T>
    bool is() const;

    template<typename T>
    T& get() const;

    // Array access
    JsonValue& operator[](int index);
    const JsonValue& operator[](int index) const;
    JsonValue& at(int index);
    const JsonValue& at(int index) const;

    // Map access
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    JsonValue& at(const std::string& key);
    const JsonValue& at(const std::string& key) const;

 private:
    std::type_index type;
    std::shared_ptr<void> value;
};

template <typename T>
JsonValue::JsonValue(const T& value) : type(typeid(T)), value(new T(value)) { }

template<typename T>
bool JsonValue::is() const {
    return type == typeid(T);
}

template<typename T>
T& JsonValue::get() const {
    return *static_cast<T*>(value.get());
}

JsonValue& JsonValue::operator[](const std::string& key) {
    return get<Map>().at(key);
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    return get<Map>().at(key);
}

JsonValue& JsonValue::at(const std::string& key) {
    return get<Map>().at(key);
}

const JsonValue& JsonValue::at(const std::string& key) const {
    return get<Map>().at(key);
}

JsonValue& JsonValue::operator[](int index) {
    return get<Array>().at(index);
}

const JsonValue& JsonValue::operator[](int index) const {
    return get<Array>().at(index);
}

JsonValue& JsonValue::at(int index) {
    return get<Array>().at(index);
}

const JsonValue& JsonValue::at(int index) const {
    return get<Array>().at(index);
}

#endif
