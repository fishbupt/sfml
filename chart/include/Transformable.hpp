// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <ScopedPtr.hpp>
#include <Vector3.hpp>
#include <Transform.hpp>    

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    /// <summary>
    /// The Wrapper class for sf::Transformable in managed space
    /// </summary>
    public ref class Transformable
    {
    public:
        Transformable() :_implPtr{ new sf::Transformable() }
        {
        }

        /// <summary>
        /// Position of the object
        /// </summary>
        property Vector3f Position
        {
            Vector3f get()
            {
                const auto& pos = _implPtr->getPosition();
                return Vector3f(pos.x, pos.y, pos.z);
            }

            void set(Vector3f value) { _implPtr->setPosition(value.X, value.Y, value.Z); }
        }

        /// <summary>
        /// Local origin of the object
        /// </summary>
        property Vector3f Origin
        {
            Vector3f get()
            {
                const auto& origin = _implPtr->getOrigin();
                return Vector3f(origin.x, origin.y, origin.z);
            }

            void set(Vector3f value) { _implPtr->setOrigin(value.X, value.Y, value.Z); }
        }

        /// <summary>
        /// Orientation of the object
        /// </summary>
        property float Rotation
        {
            float get() { return _implPtr->getRotation(); }
            void set(float value) { _implPtr->setRotation(value); }
        }

        /// <summary>
        /// Scale of the object
        /// </summary>
        property Vector3f Scale
        {
            Vector3f get()
            {
                const auto& scale = _implPtr->getScale();
                return Vector3f(scale.x, scale.y, scale.z);
            }
            void set(Vector3f value) { _implPtr->setScale(value.X, value.Y, value.Z); }
        }

        /// <summary>
        /// Get the combined transform of the object
        /// </summary>
        /// <returns></returns>
        Transform^ GetTransform()
        {
            return gcnew Transform(const_cast<sf::Transform*>(&_implPtr->getTransform()));
        }

        /// <summary>
        /// Get the inverse of the combined transform of the object
        /// </summary>
        /// <returns></returns>
        Transform^ GetInverseTransform()
        {
            return gcnew Transform(const_cast<sf::Transform*>(&_implPtr->getInverseTransform()));
        }

        property IntPtr NativePointer
        {
            IntPtr get()
            {
                return IntPtr(_implPtr.get());
            }
        }

    private:
        scoped_ptr<sf::Transformable> _implPtr;
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
