#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <my_common_cpp_utils/config.h>
#include <my_common_cpp_utils/logger.h>

namespace utils
{

TEST(ConfigViaJson_BasicTypes, StringLiteral)
{
    auto stringView = details_::toStringView<"literal string">();
    ASSERT_EQ(stringView, "literal string");
}

TEST(ConfigViaJson_BasicTypes, ReadIntValue)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "intValue": 123 } )");

    auto intValue = getConfig<int, "intValue">();
    ASSERT_EQ(intValue, 123);
}

TEST(ConfigViaJson_BasicTypes, ReadDefaultValues)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "hello": 123 } )");

    auto noExistingValue = getConfig<int, "noExistingValue", 456>();
    ASSERT_EQ(noExistingValue, 456);
}

TEST(ConfigViaJson_BasicTypes, ReadFloatValue)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "floatValue": 123.5 } )");

    auto floatValue = getConfig<float, "floatValue", 600>();
    ASSERT_EQ(floatValue, 123.5);
}

TEST(ConfigViaJson_GlmTypes, ReadVec2)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "point": { "x": 100, "y" : 200 } } )");

    auto readPoint = getConfig<glm::vec2, "point">();
    glm::vec2 expextedPoint{100, 200};
    EXPECT_EQ(readPoint, expextedPoint);
}

struct Address
{
    std::string street;
    int housenumber{};
    int postcode{};

    auto operator<=>(const Address&) const = default;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Address, street, housenumber, postcode)
};

TEST(ConfigViaJson_CustomStructs, ReadAdress)
{
    Config::getInstance(Config::Mode::ForceReload, R"(
        {
            "address": {
                "street": "Georgia",
                "housenumber": 123,
                "postcode": 456
            }
        }
        )");

    Address expectedAddress{"Georgia", 123, 456};

    const auto& addressFromConfig = getConfig<Address, "address">();
    EXPECT_EQ(expectedAddress, addressFromConfig);
}

#ifdef SFML_AVAILABLE

TEST(ConfigViaJson_MagicEnum, SfmlMagicEnumCast)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "controls": { "left": "X" } } )");
    const auto& left = getConfig<std::string, "controls.left">();
    auto leftKey = magic_enum::enum_cast<sf::Keyboard::Key>(left).value();
    EXPECT_EQ(leftKey, sf::Keyboard::Key::X);
}

TEST(ConfigViaJson_MagicEnum, SfmlAdlSerializerCast)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "controls": { "left": "X" } } )");

    auto sfmlKey = sf::Keyboard::Key::W;
    json jsonKey;
    adl_serializer::to_json(jsonKey, sfmlKey);
    EXPECT_EQ(jsonKey, "W");

    sf::Keyboard::Key sfmlKey2;
    adl_serializer::from_json(jsonKey, sfmlKey2);
    EXPECT_EQ(sfmlKey2, sf::Keyboard::Key::W);
    EXPECT_EQ(sfmlKey2, sfmlKey);
}

TEST(ConfigViaJson_MagicEnum, SfmlGetConfigCast)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "controls": { "left": "X" } } )");

    const auto& leftKey = getConfig<sf::Keyboard::Key, "controls.left">();
    EXPECT_EQ(leftKey, sf::Keyboard::Key::X);
}

TEST(ConfigViaJson_adl_serializer, ReadSfVector2)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "point": { "x": 100, "y" : 200 } } )");

    auto readPoint = getConfig<sf::Vector2i, "point">();
    sf::Vector2i expextedPoint{100, 200};
    EXPECT_EQ(readPoint, expextedPoint);
}

#endif // SFML_AVAILABLE

TEST(ConfigViaJson_MagicEnum, CustomEnum)
{
    Config::getInstance(Config::Mode::ForceReload, R"( { "controls": { "left": "X" } } )");

    enum class MyEnum
    {
        X,
        Y,
        Z,
    };

    const auto& leftKey = getConfig<MyEnum, "controls.left">();
    EXPECT_EQ(leftKey, MyEnum::X);
}
} // namespace utils