#pragma once
#include <magic_enum.hpp>
#include <my_common_cpp_utils/json_formatter.h>
#include <my_common_cpp_utils/json_utils.h>
#include <nlohmann/json.hpp>

namespace utils
{

// Singleton class to store and access JSON data.
class Config
{
public:
    static void initInstanceFromString(std::string& jsonSourceAsString);
    static void initInstanceFromFile(std::filesystem::path& jsonFilePath);
    static nlohmann::json& getInstance();
    // Save changes to the same file from which the data was loaded.
    // Throws exception if data was not loaded from file.
    static void save();
};

namespace details_
{
// Literal class type that wraps a constant expression string.
// Uses implicit conversion to allow templates to *seemingly* accept constant strings.
template <size_t N>
struct StringLiteral
{
    constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }
    char value[N];
};

template <StringLiteral Lit>
std::string_view toStringView()
{
    return {Lit.value};
}
} // namespace details_

// Read any built-it type from Json. Return optional. Key = "a.b.c"
// Do not throw exception if key not found.
template <typename T, details_::StringLiteral Key>
std::optional<T>& getConfigOpt()
{
    static std::optional<T> result = getElementByPath(Config::getInstance(), details_::toStringView<Key>());
    return result;
}

// Read any built-it type from Json. Throw exception if key not found. Key = "a.b.c"
template <typename T, details_::StringLiteral Key>
const T& getConfig()
{
    static T& result = getConfigOpt<T, Key>().value();
    return result;
}

namespace details_
{
template <typename T, StringLiteral Key>
    requires requires(T key) { adl_serializer::from_json(std::declval<nlohmann::basic_json<>&>(), key); }
T getComplexDataViaJsonSerialization()
{
    nlohmann::json elementJson = getConfigOpt<nlohmann::json, Key>().value();
    T result;
    adl_serializer::from_json(elementJson, result);
    return result;
}
} // namespace details_

// Read json serializable type from Json. Throw exception if key not found. Key = "a.b.c"
template <typename T, details_::StringLiteral Key>
    requires requires(T key) { adl_serializer::from_json(std::declval<nlohmann::json&>(), key); }
const T& getConfig()
{
    static T value = details_::getComplexDataViaJsonSerialization<T, Key>();
    return value;
}

// Read any type from Json with default value. Key = "a.b.c".
template <typename T, details_::StringLiteral Key, auto DefaultValue>
const T& getConfig()
{
    static T value = getConfigOpt<T, Key>().value_or(DefaultValue);
    return value;
}

} // namespace utils