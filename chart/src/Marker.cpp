// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <Marker.hpp>
namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    static sf::Vector3f InitialSize{ 0.03f, 0.06f, 0.03f };

    Marker::Marker()
        : _shape(new sf::Octahedron())
    {
        Visible = false;
        X = 0;
        Y = 0;
        Z = 0;
        Active = false;
        InactiveColor = Colors::Gray;
        ActiveColor = Colors::Lime;
        _shape->setSize(InitialSize);
        _shape->setColor(sf::Color::Green);
        _shape->setBorderColor(sf::Color::White);
        _shape->rotate(-30.0f, {0, 1, 0});
    }

    Marker::~Marker()
    {
        this->!Marker();
    }

    Marker::!Marker()
    {
    }

    void Marker::Scale(float scaleX, float scaleY, float scaleZ)
    {
        _shape->setSize({ InitialSize.x*scaleX, InitialSize.y*scaleY, InitialSize.z*scaleZ });
    }

    void Marker::Draw(sf::RenderTarget* target, sf::RenderStates states, sf::Transform transform)
    {
        if (Active)
            _shape->setColor(ColorUtil::ColorFrom(ActiveColor));
        else
            _shape->setColor(ColorUtil::ColorFrom(InactiveColor));
        _shape->setPosition(transform.transformPoint({X, Y, Z}));
        target->draw(*_shape, states);
    }
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
