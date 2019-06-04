// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <msclr\marshal_cppstd.h>
#include "Annotation.hpp"
#include "Utils.hpp"
#include "ScatterChart.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <cmath>

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        enum TextIndex
        {
            TextXAxisMin = 0,
            TextXAxisMax,
            TextXAxisUnit,
            TextYAxisMin,
            TextYAxisMax,
            TextYAxisUnit,
            TextZAxisMin,
            TextZAxisMax,
            TextZAxisUnit,
            TextIndexSize
        };

        using Line = std::array<sf::Vector3f, 2>;

        sf::Vector2f getMiddlePoint(const Line& line)
        {
            return sf::Vector2f{(line[0].x + line[1].x) / 2.0f, (line[0].y + line[1].y) / 2.0f};
        }

        float getAngle(const Line& line)
        {
            return glm::degrees(glm::atan(line[0].y - line[1].y, line[1].x - line[0].x));
        }

        float getLength(const Line& line)
        {
            return std::hypot(std::fabsf(line[0].x - line[1].x), std::fabsf(line[0].y - line[1].y));
        }

        std::string FromManagedString(System::String ^ managedStr)
        {
            return msclr::interop::marshal_as<std::string>(managedStr);
        }

        std::string FormatAxisValue(double value, int precision = 2)
        {
            std::ostringstream oss;
            oss << std::setprecision(precision) << value;
            return oss.str();
        }

        Annotation::Annotation(ScatterChart ^ chart)
            : _font(new sf::Font())
            , _texts(new std::vector<sf::Text>(TextIndexSize))
        {
            _chart = chart;
            _camera = _chart->Camera;
            FontName = "arial";
            FontSize = 16;
            FontStyle = TextStyle::Regular;
        }

        void Annotation::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            // draw text to 2D view
            target->setView(target->getDefaultView());

            for (auto& text : *_texts)
            {
                text.setCharacterSize(FontSize);
                text.setFillColor(ColorUtil::ColorFrom(Color));
                text.setStyle((int)FontStyle);
            }

            const sf::Transform& cameraTransform = _camera->getCamera()->getTransform();
            const sf::Transform& viewTransform = _camera->getCamera()->getViewTransform();
            sf::Transform screenTransform = target->getDefaultView().getInverseTransform();
            screenTransform.combine(cameraTransform).combine(viewTransform);

            PlotXAnnotation(screenTransform);
            PlotYAnnotation(screenTransform);
            PlotZAnnotation(screenTransform);

            for (auto& text : *_texts)
            {
                target->draw(text, states);
            }
        }

        void Annotation::PlotUnit(sf::Text& text, const sf::Vector2f& pos, const std::string& unit, float angle)
        {
            text.setString(unit);
            float textWidth = text.getLocalBounds().width;
            float textHeight = text.getLocalBounds().height;
            text.setOrigin(textWidth / 2.0f, textHeight / 2.0f);
            if (MathUtil::IsInRange(angle, -90.0f, -70.0f)||MathUtil::IsInRange(angle, 90.0f, 110.f))
            {
                text.setPosition(pos.x - 2 * FontSize, pos.y);
                text.setRotation(-90.f);
            }
            else if (MathUtil::IsInRange(angle, -110.0f, -90.0f)||MathUtil::IsInRange(angle, 70.0f, 90.0f))
            {
                text.setPosition(pos.x + 2 * FontSize, pos.y);
                text.setRotation(-90.f);
            } 
            else if (MathUtil::IsInRange(angle, 0, 70.0f)||MathUtil::IsInRange(angle, -180.0f, -110.0f))
            {
                text.setPosition(pos.x + textWidth / 2.0f, pos.y + FontSize);
                text.setRotation(0);
            }
            else
            {
                text.setPosition(pos.x - textWidth / 2.0f, pos.y + FontSize);
                text.setRotation(0);
            }
        }

        void Annotation::PlotAxisMin(sf::Text& text, const sf::Vector2f& pos, const std::string& axis, float angle)
        {
            text.setString(axis);
            text.setPosition(pos);
            if (MathUtil::IsInRange(angle, -90.0f, -45.0f))
                text.move(-text.getLocalBounds().width - FontSize, FontSize);
            if (MathUtil::IsInRange(angle, -45.0f, 0))
                text.move(0, FontSize);
            else if (MathUtil::IsInRange(angle, -180.0f, -90.0f))
                text.move(text.getLocalBounds().width, FontSize);
            else if (MathUtil::IsInRange(angle, 0, 90.f))
                text.move(text.getLocalBounds().width + FontSize, 0);
            else if (MathUtil::IsInRange(angle, 90.0f, 180.0f))
                text.move(-text.getLocalBounds().width - FontSize, FontSize);
        }
        void Annotation::PlotAxisMax(sf::Text& text, const sf::Vector2f& pos, const std::string& axis, float angle)
        {
            text.setString(axis);
            text.setPosition(pos);
            if (MathUtil::IsInRange(angle, -90.0f, 0))
                text.move(-text.getLocalBounds().width - FontSize, FontSize);
            else if (MathUtil::IsInRange(angle, -180.0f, -90.0f))
                text.move(text.getLocalBounds().width + FontSize, 0);
            else if (MathUtil::IsInRange(angle, 0, 90.f))
                text.move(text.getGlobalBounds().width, FontSize);
            else if (MathUtil::IsInRange(angle, 90.0f, 135.0f))
                text.move(-text.getLocalBounds().width - FontSize, FontSize);
            else if (MathUtil::IsInRange(angle, 135.0f, 180.0f))
                text.move(0, FontSize);
        }
        void Annotation::PlotXAnnotation(const sf::Transform& transform)
        {
            float zVal = -1.0f;
            if (MathUtil::IsInRange(_camera->Azimuth, -90.0f, 90.f))
                zVal = 1.0f;
            if (_camera->Elevation < 0.0f)
                zVal = -zVal;

            Line xAxis{sf::Vector3f{-1, -1, zVal}, sf::Vector3f{1, -1, zVal}};
            Line screenX{transform.transformPoint(xAxis[0]), transform.transformPoint(xAxis[1])};
            if (getLength(screenX) < kMinAxisLenghtInPixelToDisplay)
            {
                (*_texts)[TextXAxisUnit].setString("");
                (*_texts)[TextXAxisMin].setString("");
                (*_texts)[TextXAxisMax].setString("");
                return;
            }
            float angle = getAngle(screenX);

            sf::Vector2f unitPos = getMiddlePoint(screenX);
            auto& textUnit = (*_texts)[TextXAxisUnit];
            PlotUnit(textUnit, getMiddlePoint(screenX), FromManagedString(XAxisUnit), angle);

            auto& textMin = (*_texts)[TextXAxisMin];
            PlotAxisMin(textMin, screenX[0], FormatAxisValue(_chart->XAxisMin), angle);

            auto& textMax = (*_texts)[TextXAxisMax];
            PlotAxisMax(textMax, screenX[1], FormatAxisValue(_chart->XAxisMax), angle);
        }

        void Annotation::PlotYAnnotation(const sf::Transform& transform)
        {
            Line yAxis;
            if (MathUtil::IsInRange(_camera->Azimuth, -90.0f, 0))
                yAxis = Line{sf::Vector3f{-1, -1, 1}, sf::Vector3f{-1, 1, 1}};
            else if (MathUtil::IsInRange(_camera->Azimuth, -180.0f, -90.f))
                yAxis = Line{sf::Vector3f{1, -1, 1}, sf::Vector3f{1, 1, 1}};
            else if (MathUtil::IsInRange(_camera->Azimuth, 0, 90.f))
                yAxis = Line{sf::Vector3f{-1, -1, -1}, sf::Vector3f{-1, 1, -1}};
            else if (MathUtil::IsInRange(_camera->Azimuth, 90.f, 180.f))
                yAxis = Line{sf::Vector3f{1, -1, -1}, sf::Vector3f{1, 1, -1}};

            Line screenY{transform.transformPoint(yAxis[0]), transform.transformPoint(yAxis[1])};
            if (getLength(screenY) < kMinAxisLenghtInPixelToDisplay)
            {
                (*_texts)[TextYAxisUnit].setString("");
                (*_texts)[TextYAxisMin].setString("");
                (*_texts)[TextYAxisMax].setString("");
                return;
            }

            auto& textUnit = (*_texts)[TextYAxisUnit];
            PlotUnit(textUnit, getMiddlePoint(screenY), FromManagedString(YAxisUnit), -90.0f);

            auto& textMin = (*_texts)[TextYAxisMin];
            textMin.setString(FormatAxisValue(_chart->YAxisMin));
            textMin.setPosition(screenY[0].x - 2 * FontSize, screenY[0].y - FontSize);

            auto& textMax = (*_texts)[TextYAxisMax];
            textMax.setString(FormatAxisValue(_chart->YAxisMax));
            textMax.setPosition(screenY[1].x - 2 * FontSize, screenY[1].y);
        }

        void Annotation::PlotZAnnotation(const sf::Transform& transform)
        {
            float xVal = -1.0f;
            if (MathUtil::IsInRange(_camera->Azimuth, -180.f, 0.f))
                xVal = 1.0f;
            if (_camera->Elevation < 0.0f)
                xVal = -xVal;

            Line zAxis{sf::Vector3f{xVal, -1, -1}, sf::Vector3f{xVal, -1, 1}};
            Line screenZ{transform.transformPoint(zAxis[0]), transform.transformPoint(zAxis[1])};
            if (getLength(screenZ) < kMinAxisLenghtInPixelToDisplay)
            {
                (*_texts)[TextZAxisUnit].setString("");
                (*_texts)[TextZAxisMin].setString("");
                (*_texts)[TextZAxisMax].setString("");
                return;
            }
            float angle = getAngle(screenZ);

            sf::Vector2f unitPos = getMiddlePoint(screenZ);
            auto& textUnit = (*_texts)[TextZAxisUnit];
            PlotUnit(textUnit, getMiddlePoint(screenZ), FromManagedString(ZAxisUnit), angle);

            auto& textMin = (*_texts)[TextZAxisMin];
            PlotAxisMin(textMin, screenZ[0], FormatAxisValue(_chart->ZAxisMin), angle);

            auto& textMax = (*_texts)[TextZAxisMax];
            PlotAxisMax(textMax, screenZ[1], FormatAxisValue(_chart->ZAxisMax), angle);
        }

        System::String ^ Annotation::FontName::get()
        {
            return _fontName;
        }

        void Annotation::FontName::set(System::String ^ value)
        {
            array<System::String ^> ^ availableFontNames =
                gcnew array<System::String ^>{"arial", "consola", "lucon", "roboto", "wqy-microhei"};

            if (Array::IndexOf(availableFontNames, value) < 0)
                value = "arail";

            if (_fontName != value)
            {
                _fontName = value;

                System::String ^ fontPath = "fonts/" + _fontName + ".ttf";

                std::string filename = FromManagedString(fontPath);
                _font->loadFromFile(filename);
                for (auto& text : *_texts)
                {
                    text.setFont(*_font);
                }
            }
        }
    }
}
}
