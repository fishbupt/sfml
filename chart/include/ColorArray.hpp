// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace sf
{
/// <summary>
/// Define a set of sf::Color
/// This class is only useful when used by CLI class which could not hold native c++ class
/// we has ScopedPtr to hold pointer of native class but only scalar value
/// </summary>
class ColorArray
{
public:
    /// <summary>
    /// Default constructor
    /// Creates an empty color array
    /// </summary>
    ColorArray()
    {
    }

    /// <summary>
    /// Contruct the count of the color array
    /// </summary>
    /// <param name="count"></param>
    explicit ColorArray(std::size_t count)
        : _colors{count}
    {
    }

    /// <summary>
    /// Returns the number of elements in the container
    /// </summary>
    /// <returns></returns>
    std::size_t size() const
    {
        return _colors.size();
    }

    /// <summary>
    /// Checks if the container has no elements
    /// </summary>
    /// <returns></returns>
    bool empty() const
    {
        return _colors.empty();
    }

    /// <summary>
    /// Returns a reference to the element at specified location pos
    /// No bounds checking is performed
    /// </summary>
    /// <param name="pos">position of the element to return</param>
    /// <returns></returns>
    sf::Color& operator[](std::size_t pos)
    {
        return _colors[pos];
    }

    /// <summary>
    /// Returns a reference to the element at specified location pos
    /// No bounds checking is performed
    /// </summary>
    /// <param name="pos">position of the element to return</param>
    /// <returns></returns>
    const sf::Color& operator[](std::size_t pos) const
    {
        return _colors[pos];
    }

    /// <summary>
    /// Erases all elements from the container.
    /// </summary>
    void clear()
    {
        _colors.clear();
    }

    /// <summary>
    /// Resizes the container to contain count elements
    /// </summary>
    void resize(std::size_t count)
    {
        _colors.resize(count);
    }

private:
    std::vector<sf::Color> _colors; /// sf::colors contained in the array
};
}
