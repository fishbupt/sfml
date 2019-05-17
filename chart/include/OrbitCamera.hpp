// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SFML/Graphics/OrbitCamera.hpp>
#include "ScopedPtr.hpp"

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // This class in managed space is just a wrapper of sf::OrbitCamera
        // clang-format off
        public ref class OrbitCamera
        // clang-format on
        {
        public:
            OrbitCamera(float fov, float nearPlane, float farPlane)
                : _camera(new sf::OrbitCamera(fov, nearPlane, farPlane))
            {
            }

        // clang-format off
            property float FieldOfView
            {
                float get() { return _camera->getFieldOfView(); }
                void set(float value) { _camera->setFieldOfView(value); }
            }

            property float NearClipplingPlane
            {
                float get() { return _camera->getNearClippingPlane(); }
                void set(float value) { _camera->setNearClippingPlane(value); }
            }

            property float FarClippingPlane
            {
                float get() { return _camera->getFarClippingPlane(); }
                void set(float value) { _camera->setFarClippingPlane(value); }
            }

            property float Distance
            {
                float get() { return _camera->getDistance(); }
                void set(float value) { _camera->setDistance(value); }
            }

            property float Azimuth
            {
                float get() { return _camera->getAzimuth(); }
                void set(float value) { _camera->setAzimuth(value); }
            }

            property float Elevation
            {
                float get() { return _camera->getElevation(); }
                void set(float value) { _camera->setElevation(value); }
            }

            property sf::Vector3f Position
            {
                sf::Vector3f get() { return _camera->getCenter(); }
                void set(sf::Vector3f value) { _camera->setPosition(value); }
            }

            property sf::Vector3f Scale
            {
                sf::Vector3f get() { return _camera->getScale(); }
                void set(sf::Vector3f value) { _camera->setScale(value); }
            }
        // clang-format on

            void UseOrthographicCamera()
            {
                _camera->useOrthographicProjection();
            }

            void UsePerspectiveCamera()
            {
                _camera->usePerspectiveProjection();
            }

            void ChangeAzimuth(float angle)
            {
                _camera->changeAzimuth(angle);
            }

            void ChangleElevation(float angle)
            {
                _camera->changeElevation(angle);
            }

            void ChangeScale(float factor)
            {
                _camera->scale(factor, factor, factor);
            }

            sf::OrbitCamera* getCamera()
            {
                return _camera.get();
            }
        private:
            scoped_ptr<sf::OrbitCamera> _camera;
        };
    }
}
}
