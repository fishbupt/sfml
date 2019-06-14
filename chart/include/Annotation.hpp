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

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
    public
        /// <summary>
        /// Managed version of SFML Text::Style
        /// </summary>
        enum class TextStyle
        {
            Regular = 0,           ///< Regular characters, no style
            Bold = 1 << 0,         ///< Bold characters
            Italic = 1 << 1,       ///< Italic characters
            Underlined = 1 << 2,   ///< Underlined characters
            StrikeThrough = 1 << 3 ///< Strike through characters
        };

        ref class ScatterChart;

        // clang-format off
        public ref class Annotation : public Drawable
        // clang-format on
        {
        public:
            Annotation(ScatterChart^ chart);

            /// <summary>
            /// Name of font
            /// SFML Font needs to load font from a ttf file.
            /// The following fonts are shipped "aria.ttf", "consola.ttf", "lucon.ttf", "roboto.ttf", "wqy-microhei.ttc"
            /// If the sepecified FontName is not found in the list, the default font "aria" will be used
            /// </summary>
            property System::String^ FontName
            {
                System::String^ get();
                void set(System::String ^ value);
            }

            /// <summary>
            /// FontSize in pixels
            /// The default value is 16
            /// </summary>
            property int FontSize;

            /// <summary>
            /// Style of Font
            /// The default style is TextStyle::Regular
            /// </summary>
            property TextStyle FontStyle;

            /// <summary>
            /// Color of the annotation text
            /// </summary>
            property Color Color;

            /// <summary>
            /// Unit text for X-Axis
            /// </summary>
            property System::String^ XAxisUnit;

            /// <summary>
            /// Unit Text for Y-Axis
            /// </summary>
            property System::String^ YAxisUnit;

            /// <summary>
            /// Unit Text for Z-Axis
            /// </summary>
            property System::String^ ZAxisUnit;


            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

        private:

            void PlotXAnnotation(const sf::Transform& transform);
            void PlotYAnnotation(const sf::Transform& transform);
            void PlotZAnnotation(const sf::Transform& transform);
            void PlotUnit(sf::Text& text, const sf::Vector2f& pos, const std::string& unit, float angle);
            void PlotAxisMin(sf::Text& text, const sf::Vector2f& pos, const std::string& axis, float angle);
            void PlotAxisMax(sf::Text& text, const sf::Vector2f& pos, const std::string& axis, float angle);
        private:
            using VectorOfPair = std::vector<std::pair<bool, sf::Text>>;

            OrbitCamera^ _camera;
            System::String^ _fontName = gcnew System::String("");
            scoped_ptr<sf::Font> _font;
            scoped_ptr<VectorOfPair> _texts;
            ScatterChart^ _chart;

            static const float kMinAxisLenghtInPixelToDisplay = 50.0f;

        };
    }
}
}
