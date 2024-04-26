#pragma once
#include <algorithm>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <optional>
#include <stdexcept>
#include <unordered_set>

namespace utils
{

template <typename T>
bool IsEqual(T lhs, T rhs, T eps = 0.0001f)
{
    return std::fabs(lhs - rhs) < eps;
}

inline bool RandomTrue(float probabilityOfTrue = 0.5f)
{
    float randomValue = glm::linearRand(0.0f, 1.0f);
    return randomValue <= probabilityOfTrue;
}

template <typename T>
T Random(T min, T max)
{
    return static_cast<T>(glm::linearRand((float)min, (float)max));
}

// Get random number from T with method size(). Return optional<size_t> if size() == 0.
template <typename T>
std::optional<size_t> RandomIndexOpt(const T& container)
{
    if (container.empty())
        return std::nullopt;

    return Random<size_t>(0, container.size() - 1);
}

// Returns 1 if val > 0, -1 if val < 0, and 0 if val == 0.
template <typename T>
T Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline glm::vec2 GetRandomCoordinateAround(const glm::vec2& center, float radius)
{
    float angle = Random<float>(0, 2 * glm::pi<float>());
    float distance = Random<float>(0, radius);

    glm::vec2 randomPoint;
    randomPoint.x = center.x + distance * cos(angle);
    randomPoint.y = center.y + distance * sin(angle);

    return randomPoint;
}

#ifdef SFML_AVAILABLE

template <typename T>
T GetArea(const sf::Vector2<T>& size)
{
    return size.x * size.y;
}

template <typename T>
sf::Vector2<T> Normalize(const sf::Vector2<T>& v)
{
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0)
        return sf::Vector2<T>(v.x / length, v.y / length);
    return v;
}

#endif // SFML_AVAILABLE

// value = 85, step = 10 => 80
template <typename T>
T RoundStep(T value, T step)
{
    return step * glm::round(value / step);
}

template <typename T>
class NoRepeatableRandoms
{
public:
    NoRepeatableRandoms() = default;

    explicit NoRepeatableRandoms(size_t capacity, T min, T max) : capacity_(capacity), min_(min), max_(max)
    {
        InitValues();
    }

    bool Empty() const { return values_.empty(); }

    T Pull()
    {
        if (values_.empty())
            InitValues();

        auto it = values_.begin();
        auto number = *it;
        values_.erase(it);
        return number;
    }

    T PullOrThrow()
    {
        if (values_.empty())
            throw std::runtime_error("NoRepeatableRandoms::pullOrThrow() - no more numbers");

        return Pull();
    }

    void Sort()
    {
        std::vector<T> sortedValues(values_.begin(), values_.end());
        std::sort(sortedValues.begin(), sortedValues.end());
        values_ = std::unordered_set<T>(sortedValues.begin(), sortedValues.end());
    }
private:
    void InitValues()
    {
        while (values_.size() != capacity_)
            values_.insert(glm::linearRand<T>(min_, max_));
    }

    size_t capacity_;
    std::unordered_set<T> values_;
    T min_;
    T max_;
};

} // namespace utils