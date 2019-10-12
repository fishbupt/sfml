// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once
#include <SFML/OpenGL.hpp>
// note: put opengl headers before any other header files
#include <SFML/Graphics/RenderTexture.hpp>

#include <Utils.hpp>
#include <ScopedPtr.hpp>
#include <Drawable.hpp>
#include <Annotation.hpp>
#include <Markers.hpp>
#include <GridShape.hpp>
#include <OrbitCamera.hpp>

#using "mscorlib.dll"
using namespace System;

#if !defined(GL_MAJOR_VERSION)
#    define GL_MAJOR_VERSION 0x821B
#endif
#if !defined(GL_MINOR_VERSION)
#    define GL_MINOR_VERSION 0x821C
#endif

namespace Xsa
{
namespace Presentation
{
namespace Graph
{
    public ref class RenderChart
    {
    public:

        property int GlMajorVersion;

        property GridShape^ Grid;

        property Annotation^ Annotation;

        property Markers^ Markers;

        property OrbitCamera^ Camera;

        property Color BackgroundColor;

        property IntPtr RenderTarget
        {
            IntPtr get() { return IntPtr(_renderTexture.get()); }
        }

        property IntPtr RenderState
        {
            IntPtr get() { return IntPtr(_renderState.get()); }
        }

        property IntPtr RenderBuffer
        {
            IntPtr get() { return IntPtr(_renderBuffer->data()); }
        }

        property IntPtr Transform
        {
            void set(IntPtr value)
            {
                if (value != IntPtr::Zero)
                {
                    _transform = (sf::Transformable*)value.ToPointer();
                }
            }
        }

        RenderChart()
            :_renderTexture(new sf::RenderTexture())
            , _renderState(new sf::RenderStates(sf::RenderStates::Default))
            ,_renderBuffer(new std::vector<int>())
        {
        }

        void Draw(bool is3DEnabled) {

            if (GlMajorVersion >= 3)
            {
                glEnable(GL_POINT_SMOOTH);
                if (is3DEnabled)
                {
                    glEnable(GL_LINE_SMOOTH);
                    glLineWidth(1.1f);
                }
                else
                {
                    glDisable(GL_LINE_SMOOTH);
                    glLineWidth(1.0f);
                }
            }
            glPointSize(3.0f);

            Draw(_renderTexture.get(), is3DEnabled);

        }

        void UpdateBuffer()
        {
            _renderTexture->display();
            const sf::Texture& texture = _renderTexture->getTexture();

            sf::Texture::bind(&texture, sf::Texture::CoordinateType::Normalized);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, _renderBuffer->data());
        }

        void Draw(sf::RenderTarget* target, bool is3DEnabled)
        {

            if (is3DEnabled)
            {
                if (Camera->IsOrthographicProjection)
                {
                    sf::Transform transform = Camera->getCamera()->getTransform();
                    sf::Transform viewTransform = Camera->getCamera()->getViewTransform();
                    transform.combine(viewTransform);

                    sf::FloatRect tRect = transform.transformBoxToRect(Grid->PlotBoxBounds);
                    float minSide = std::min((float)Grid->WindowRectangle.Width, (float)Grid->WindowRectangle.Height);
                    if (minSide > 0)
                    {
                        float scaleRatio = (minSide - 5 * (float)Annotation->FontSize) / minSide * 2;
                        float scaleFactor = scaleRatio / std::max(tRect.width, tRect.height);
                        Camera->ChangeScale(scaleFactor, scaleFactor, 1.0f);
                    }
                }
                target->enableDepthTest(true);
                target->setView(*Camera->getCamera());

                Grid->ShowTopPlane(!(Camera->Elevation > 0));
                Grid->ShowFrontPlane(!MathUtil::IsInRange(Camera->Azimuth, -90.0f, 90.0f));
                Grid->ShowRightPlane(!MathUtil::IsInRange(Camera->Azimuth, -180.0f, 0.0f));
            }
            else
            {
                target->setView(target->getDefaultView());
                target->enableDepthTest(false);
            }

            sf::Color backColor = ColorUtil::ColorFrom(BackgroundColor);
            target->clear(backColor);
            Grid->Draw(target, sf::RenderStates::Default);
            if (is3DEnabled)
            {
                DrawMarkers(target);
            }

            *_renderState = sf::RenderStates::Default;
            _renderState->transform *= _transform->getTransform();
        }


        void DrawMarkers(sf::RenderTarget* target)
        {
            sf::Transform viewTransform = Camera->getCamera()->getViewTransform();
            viewTransform.combine(_transform->getTransform());

            Markers->Draw(target, sf::RenderStates::Default, viewTransform);
        }


        bool CreateRenderTexture(int width, int height, bool is3DEnabled)
        {
            bool res = false;
            _renderTexture->setActive(false);
            if (is3DEnabled)
                res = _renderTexture->create(width, height, sf::ContextSettings{ 24,0,8 });
            else
                res = _renderTexture->create(width, height);

            _renderTexture->setActive(true);
            _renderTexture->setSmooth(true);

            int majorVersion = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
            GlMajorVersion = majorVersion;

            _renderBuffer->resize(width * height);
            return res;
        }

    private:
        scoped_ptr<sf::RenderTexture> _renderTexture;
        scoped_ptr<sf::RenderStates> _renderState;
        scoped_ptr<std::vector<int>> _renderBuffer;

        sf::Transformable* _transform;
    };
} // namespace Xsa
} // namespace Presentation
} // namespace Graph
