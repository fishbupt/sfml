// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <Drawable.hpp>
#include <ScopedPtr.hpp>
#include <OrbitCamera.hpp>
#include <vector>
#include <utility>

#using "WindowsBase.dll"
#using "PresentationFramework.dll"
#using "PresentationCore.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Controls;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {

        ref class ScatterChart;

        // clang-format off
        public ref class Annotation : public Drawable
        // clang-format on
        {
        public:
            Annotation(ScatterChart ^ chart);

            /// <summary>
            /// Family of Font
            /// </summary>
            property FontFamily ^ FontFamily;
            /// <summary>
            /// FontSize in pixels
            /// The default value is 16
            /// </summary>
            property double FontSize;

            /// <summary>
            /// Style of Font
            /// The default style is Normal
            /// </summary>
            property FontStyle FontStyle;

            /// <summary>
            /// Weight of Font
            /// The default value is Normal
            /// </summary>
            property FontWeight FontWeight;

            /// <summary>
            /// Color of the annotation text
            /// </summary>
            property Color Color;

            /// <summary>
            /// Unit text for X-Axis
            /// </summary>
            property System::String ^ XAxisUnit;

            /// <summary>
            /// Unit Text for Y-Axis
            /// </summary>
            property System::String ^ YAxisUnit;

            /// <summary>
            /// Unit Text for Z-Axis
            /// </summary>
            property System::String ^ ZAxisUnit;

            /// <summary>
            /// Set this property to "EngineeringFormatter" if you want more readable format value
            /// </summary>
            property IFormatProvider ^ FormatProvider;

            /// <summary>
            /// Disable drawing annotations
            /// </summary>
            void Disable();

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

        private:
            void PlotXAnnotation(const sf::Transform& transform);
            void PlotYAnnotation(const sf::Transform& transform);
            void PlotZAnnotation(const sf::Transform& transform);
            void PlotUnit(TextBlock ^ text, const sf::Vector2f& pos, String ^ unit, float angle);
            void PlotAxisMin(TextBlock ^ text, const sf::Vector2f& pos, double axis, float angle);
            void PlotAxisMax(TextBlock ^ text, const sf::Vector2f& pos, double axis, float angle);

            System::String ^ FormatAxisValue(double axis);

        private:
            OrbitCamera ^ _camera;
            array<TextBlock ^> ^ _textBlocks;
            ScatterChart ^ _chart;

            static const float kMinAxisLenghtInPixelToDisplay = 50.0f;
        };
    }
}
}
