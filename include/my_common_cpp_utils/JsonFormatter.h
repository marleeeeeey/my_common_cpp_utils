// ReSharper disable CppInconsistentNaming
#pragma once
#include "magic_enum.hpp"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/fmt/fmt.h>

struct adl_serializer
{
    // Convert any enum to json
    template <typename T>
        requires requires(T enumValue) { magic_enum::enum_name(enumValue); }
    static void to_json(nlohmann::json& j, const T& enumValue)
    {
        j = magic_enum::enum_name(enumValue);
    }

    // Convert any enum from json
    template <typename T>
        requires requires(T) { magic_enum::enum_cast<T>(std::declval<nlohmann::json&>().get<std::string>()).value(); }
    static void from_json(const nlohmann::json& j, T& enumValue)
    {
        enumValue = magic_enum::enum_cast<T>(j.get<std::string>()).value();
    }

    // ***************************************************************
    // ***************************************************************
    // ***************************************************************

    // Option 1: Add to_json/from_json to adl_serializer.
    // In this place define cutom rules to serialize/deserialize to json.
    // These rules will be also used for fmt::format.

    // Option 2: Use NLOHMANN_DEFINE_TYPE_INTRUSIVE inside your own class/struct.
    // Example:
    // struct Address
    // {
    //     std::string street;
    //     int housenumber{};
    //     int postcode{};
    //     auto operator<=>( const Address& ) const = default;
    //     NLOHMANN_DEFINE_TYPE_INTRUSIVE( Address, street, housenumber, postcode )
    // };

    // ***************************************************************

#ifdef SFML_AVAILABLE

    template <typename T>
    static void to_json(json& j, const sf::Vector2<T>& data)
    {
        j = json{{"x", data.x}, {"y", data.y}};
    }

    template <typename T>
    static void from_json(const json& j, sf::Vector2<T>& data)
    {
        data.x = j.at("x").get<T>();
        data.y = j.at("y").get<T>();
    }

    // ***************************************************************

    static auto& getPredefindColorMap()
    {
        static const std::unordered_map<std::string_view, sf::Color> stringToSfColorMatch = {
            {"Red", sf::Color::Red},
            {"Green", sf::Color::Green},
            {"Blue", sf::Color::Blue},
            {"Yellow", sf::Color::Yellow},
            {"Magenta", sf::Color::Magenta},
            {"Cyan", sf::Color::Cyan},
            {"Transparent", sf::Color::Transparent},
            {"White", sf::Color::White},
            {"Black", sf::Color::Black},
            {"Grey", sf::Color(50, 50, 50)},
        };

        return stringToSfColorMatch;
    }

    static void to_json(json& j, const sf::Color& data)
    {
        j = std::ranges::find_if(
                getPredefindColorMap(), [color = data](const auto& pair) { return pair.second == color; })
                ->first;
    }

    static void from_json(const json& j, sf::Color& data) { data = getPredefindColorMap().at(j.get<std::string>()); }

    // ***************************************************************
#endif

    template <typename T>
    static void to_json(nlohmann::json& j, const glm::vec<2, T>& data)
    {
        j = nlohmann::json{{"x", data.x}, {"y", data.y}};
    }

    template <typename T>
    static void from_json(const nlohmann::json& j, glm::vec<2, T>& data)
    {
        data.x = j.at("x").get<T>();
        data.y = j.at("y").get<T>();
    }
    // ***************************************************************
    // ***************************************************************
    // ***************************************************************
};

// Format any json
template <>
struct fmt::formatter<nlohmann::json> : fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const nlohmann::json& j, FormatContext& ctx)
    {
        std::string jsonStr = j.dump();
        return fmt::format_to(ctx.out(), "{}", jsonStr);
    }
};

// Format types that have json serialization
template <typename T>
    requires requires(T key) { adl_serializer::to_json(std::declval<nlohmann::json&>(), key); }
struct fmt::formatter<T> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const T& key, FormatContext& ctx)
    {
        nlohmann::json jsonKey;
        adl_serializer::to_json(jsonKey, key);
        return fmt::format_to(ctx.out(), "{}", jsonKey);
    }
};
