#pragma once
#include "JsonFormatter.h"
#include "JsonLoader.h"
#include <magic_enum.hpp>
#include <memory>
#include <nlohmann/json.hpp>

namespace utils
{

// TODO: check incorrect usage of config parameters on application start
class Config
{
public:
    enum class Mode
    {
        NoReload,
        ForceReload
    };

    // jsonSourceAsString - optional json string to load instead of file. Mostly for tests.
    static std::shared_ptr<JsonLoader>& getInstance(Mode mode, std::string_view jsonSourceAsString = {});
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
    static std::optional<T> result =
        getElementByPath(Config::getInstance(Config::Mode::NoReload)->root(), details_::toStringView<Key>());
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
    json elementJson = getConfigOpt<json, Key>().value();
    T result;
    adl_serializer::from_json(elementJson, result);
    return result;
}
} // namespace details_

// Read json serializable type from Json. Throw exception if key not found. Key = "a.b.c"
template <typename T, details_::StringLiteral Key>
    requires requires(T key) { adl_serializer::from_json(std::declval<json&>(), key); }
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