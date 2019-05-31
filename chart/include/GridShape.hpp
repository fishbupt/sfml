// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Cuboid.hpp>
#include "Drawable.hpp"
#include "ScopedPtr.hpp"

#using "WindowsBase.dll"
#using "PresentationFramework.dll"
#using "PresentationCore.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Media;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class GridShape : public Drawable
        // clang-format on
        {
        public:
            GridShape();

            static const float kMinZValue = -1.0f;

            static const float kMaxZValue = 1.0f;

            event EventHandler ^ GridRectangleChanged;

            property bool IsVisible;

            property Color GridColor;

            property Color BackgroundColor;

            property Rect ClientRectangle;

            property Rect GridRectangle;

            property Rect WindowRectangle
            {
                Rect get()
                {
                    return _windowRectangle;
                }
                void set(Rect value);
            }

            property int NumberOfXAxisDivisions
            {
                int get()
                {
                    return _numberOfXAxisDivisions;
                }
                void set(int value);
            }

            property int NumberOfYAxisDivisions
            {
                int get()
                {
                    return _numberOfYAxisDivisions;
                }
                void set(int value);
            }

            property int NumberOfZAxisDivisions
            {
                int get()
                {
                    return _numberOfZAxisDivisions;
                }
                void set(int value);
            }

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

            property sf::FloatBox PlotBoxBounds
            {
                sf::FloatBox get()
                {
                    return _plotBox->getLocalBounds();
                }
            }

            void ShowTopPlane(bool showTop);

            void ShowFrontPlane(bool showFront);

            void ShowRightPlane(bool showRight);

        private:
            constexpr static float kBorderPixelOffset{0.0f};

            Rect _windowRectangle;
            int _numberOfXAxisDivisions;
            int _numberOfYAxisDivisions;
            int _numberOfZAxisDivisions;

            scoped_ptr<sf::Cuboid> _plotBox;

            scoped_ptr<sf::VertexArray> _xyPlaneGrid;
            scoped_ptr<sf::VertexArray> _xyPlaneBorder;
            bool _xyPlaneGridUpdated;
            bool _showFrontPlane; // True -- display grid on Front plane. False -- display grid on back plane
            scoped_ptr<sf::VertexArray> _xzPlaneGrid;
            scoped_ptr<sf::VertexArray> _xzPlaneBorder;
            bool _xzPlaneGridUpdated;
            bool _showTopPlane; // True -- display grid on Top plane. False -- display grid on Bottom plane
            scoped_ptr<sf::VertexArray> _yzPlaneGrid;
            scoped_ptr<sf::VertexArray> _yzPlaneBorder;
            bool _yzPlaneGridUpdated;
            bool _showRightPlane; // True -- display grid on Right plane. False -- display grid on Left plane

            void UpdateXYPlaneGrid();
            void UpdateXZPlaneGrid();
            void UpdateYZPlaneGrid();

            property bool Is3DEnabled
            {
                bool get();
            }

        };
    }
}
}
