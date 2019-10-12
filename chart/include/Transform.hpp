// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Transform.hpp>
#include <Vector3.hpp>

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// The Wrapper class for sf::Transform in managed space
    /// </summary>
    public ref class Transform
    {
    public:
        Transform(sf::Transform *ptr)
        {
            _implPtr = ptr;
        }

        Vector3f TransformPoint(Vector3f point)
        {
            if (_implPtr != nullptr)
            {
                sf::Vector3f pos = _implPtr->transformPoint(point.X, point.Y, point.Z);
                return Vector3f(pos.x, pos.y, pos.z);
            }
            return Vector3f(0, 0, 0);
        }

    private:
        sf::Transform* _implPtr;
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
