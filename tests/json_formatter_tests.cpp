#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <my_common_cpp_utils/Config.h>
#include <my_common_cpp_utils/Logger.h>

namespace utils
{

TEST(UtilsFmtFormat, FmtJson)
{
    json jsonKey("A");
    auto result = fmt::format("jsonValue={}", jsonKey);
    EXPECT_EQ(result, "jsonValue=\"A\"");
}

#ifdef SFML_AVAILABLE

TEST(UtilsFmtFormat, FmtSfVector2)
{
    sf::Vector2i expextedPoint{100, 200};
    auto resultFmt = fmt::format("point={}", expextedPoint);
    EXPECT_EQ(resultFmt, R"(point={"x":100,"y":200})");
}

#endif // SFML_AVAILABLE

struct GameState
{
    bool quit{false};
    glm::vec2 windowSize{800, 600};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameState, quit);

    // TODO Doesn't work with custom defined `to_json / from_json` in adl_serializer
    // see definition of:
    // static void to_json( nlohmann::json& j, const glm::vec<2, T>& data )
    // NLOHMANN_DEFINE_TYPE_INTRUSIVE( GameState, quit, windowSize );
};

// TEST( ConfigViaJson_CustomStructs, ReadGameState )
//{
//     GameState gameState;
//     auto resultFmt = fmt::format( "gamestate {}", gameState ); // does not work in compile time
//     EXPECT_EQ( resultFmt, R"(gamestate {"quit":false})" );
// }

TEST(ConfigViaJson_CustomStructs, ManualCastToJson)
{
    GameState gameState;
    auto resultFmt = fmt::format("gamestate {}", nlohmann::json(gameState));
    EXPECT_EQ(resultFmt, R"(gamestate {"quit":false})");
}

template <typename T, typename = void>
struct has_to_json : std::false_type
{};

template <typename T>
struct has_to_json<T, std::void_t<decltype(to_json(std::declval<nlohmann::json&>(), std::declval<const T&>()))>>
  : std::true_type
{};

template <typename T, typename std::enable_if<has_to_json<T>::value, bool>::type = true>
std::string format_as_json(const T& value)
{
    nlohmann::json j = value;
    return fmt::format("{}", j.dump());
}

template <typename T, typename std::enable_if<!has_to_json<T>::value, bool>::type = true>
std::string format_as_json(const T& value)
{
    return fmt::format("{}", value);
}

TEST(ConfigViaJson_CustomStructs, Use_format_as_json_template)
{
    GameState gameState;
    auto resultFmt = format_as_json(gameState);
    EXPECT_EQ(resultFmt, R"({"quit":false})");
}

} // namespace utils