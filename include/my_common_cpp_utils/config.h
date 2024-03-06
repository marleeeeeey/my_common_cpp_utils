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
    static void InitInstanceFromString(const std::string& jsonSourceAsString);
    static void InitInstanceFromFile(const std::filesystem::path& jsonFilePath);
    static nlohmann::json& GetInstance();
    // Save changes to the same file from which the data was loaded.
    // Throws exception if data was not loaded from file.
    static void Save();
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
std::string_view ToStringView()
{
    return {Lit.value};
}
} // namespace details_

// Read any built-it type from Json. Return optional. Key = "a.b.c"
// Do not throw exception if key not found.
template <typename T, details_::StringLiteral Key>
std::optional<T>& GetConfigOpt()
{
    static std::optional<T> result = GetElementByPath(Config::GetInstance(), details_::ToStringView<Key>());
    return result;
}

// Read any built-it type from Json. Throw exception if key not found. Key = "a.b.c"
template <typename T, details_::StringLiteral Key>
const T& GetConfig()
{
    static T& result = GetConfigOpt<T, Key>().value();
    return result;
}

namespace details_
{
template <typename T, StringLiteral Key>
    requires requires(T key) { adl_serializer::from_json(std::declval<nlohmann::basic_json<>&>(), key); }
T GetComplexDataViaJsonSerialization()
{
    nlohmann::json elementJson = GetConfigOpt<nlohmann::json, Key>().value();
    T result;
    adl_serializer::from_json(elementJson, result);
    return result;
}
} // namespace details_

// Read json serializable type from Json. Throw exception if key not found. Key = "a.b.c"
template <typename T, details_::StringLiteral Key>
    requires requires(T key) { adl_serializer::from_json(std::declval<nlohmann::json&>(), key); }
const T& GetConfig()
{
    static T value = details_::GetComplexDataViaJsonSerialization<T, Key>();
    return value;
}

// Read any type from Json with default value. Key = "a.b.c".
template <typename T, details_::StringLiteral Key, auto DefaultValue>
const T& GetConfig()
{
    static T value = GetConfigOpt<T, Key>().value_or(DefaultValue);
    return value;
}

} // namespace utils