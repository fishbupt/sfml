// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include "Annotation.hpp"
#include "Utils.hpp"
#include "ScatterChart.hpp"
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


        Annotation::Annotation(ScatterChart ^ chart)
        {
            _chart = chart;
            _camera = _chart->Camera;
            FontSize = 13;
            _textBlocks = gcnew array<TextBlock ^>(TextIndexSize);
            for (int i = 0; i < TextIndexSize; i++)
            {
                _textBlocks[i] = gcnew TextBlock();
                _textBlocks[i]->RenderTransform = gcnew RotateTransform();
                _textBlocks[i]->Foreground = gcnew SolidColorBrush(Color);
                _textBlocks[i]->FontSize = FontSize;
                _textBlocks[i]->FontStyle = FontStyle;
                _textBlocks[i]->FontWeight = FontWeight;
                _chart->_canvas->Children->Add(_textBlocks[i]);
            }
        }

        void Annotation::Disable()
        {
            for each(TextBlock ^ text in _textBlocks)
            {
                text->Visibility = Visibility::Hidden;
            }

        }

        void Annotation::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {

            const sf::Transform& cameraTransform = _camera->getCamera()->getTransform();
            const sf::Transform& viewTransform = _camera->getCamera()->getViewTransform();
            sf::Transform screenTransform = target->getDefaultView().getInverseTransform();
            screenTransform.combine(cameraTransform).combine(viewTransform);

            for each(TextBlock ^ text in _textBlocks)
            {
                text->Foreground = gcnew SolidColorBrush(Color);
                text->FontSize = FontSize;
                text->FontFamily = FontFamily;
                text->FontStyle = FontStyle;
                text->FontWeight = FontWeight;
                text->Visibility = Visibility::Visible;
            }

            PlotXAnnotation(screenTransform);
            PlotYAnnotation(screenTransform);
            PlotZAnnotation(screenTransform);
        }

        void Annotation::PlotUnit(TextBlock ^ text, const sf::Vector2f& pos, String ^ unit, float angle)
        {
            text->Text = unit;
            float textWidth = text->DesiredSize.Width;
            text->RenderTransformOrigin = Point(0.5, 0.5);
            RotateTransform ^ textTransform = (RotateTransform ^) text->RenderTransform;
            if (MathUtil::IsInRange(angle, -90.0f, -70.0f) || MathUtil::IsInRange(angle, 90.0f, 110.f))
            {
                Canvas::SetLeft(text, pos.x - 2 * FontSize);
                Canvas::SetTop(text, pos.y);
                textTransform->Angle = -90.0;
            }
            else if (MathUtil::IsInRange(angle, -110.0f, -90.0f) || MathUtil::IsInRange(angle, 70.0f, 90.0f))
            {
                Canvas::SetLeft(text, pos.x + 2 * FontSize);
                Canvas::SetTop(text, pos.y);
                textTransform->Angle = -90.0;
            }
            else if (MathUtil::IsInRange(angle, 0, 70.0f) || MathUtil::IsInRange(angle, -180.0f, -110.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth / 2.0);
                Canvas::SetTop(text, pos.y + FontSize);
                textTransform->Angle = 0;
            }
            else
            {
                Canvas::SetLeft(text, pos.x - textWidth / 2.0);
                Canvas::SetTop(text, pos.y + FontSize);
                textTransform->Angle = 0;
            }
        }

        void Annotation::PlotAxisMin(TextBlock ^ text, const sf::Vector2f& pos, double axis, float angle)
        {
            text->Text = String::Format("{0}", axis);
            float textWidth = text->DesiredSize.Width;
            float textHeight = (float)FontSize;

            if (MathUtil::IsInRange(angle, -45.0f, 0))
            {
                Canvas::SetLeft(text, pos.x - textHeight);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, -90.0f, -45.0f))
            {
                Canvas::SetLeft(text, pos.x - textWidth - textHeight);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 90.0f, 135.0f))
            {
                Canvas::SetLeft(text, pos.x - textWidth - textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, 135.0f, 180.0f))
            {
                Canvas::SetLeft(text, pos.x - textWidth);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 0, 45.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 45.0f, 90.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth + textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, -135.0f, -90.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth + textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, -180.0f, -135.f))
            {
                Canvas::SetLeft(text, pos.x);
                Canvas::SetTop(text, pos.y + textHeight);
            }
        }

        void Annotation::PlotAxisMax(TextBlock ^ text, const sf::Vector2f& pos, double axis, float angle)
        {
            text->Text = String::Format("{0}", axis);
            float textWidth = text->DesiredSize.Width;
            float textHeight = (float)FontSize;

            if (MathUtil::IsInRange(angle, -45.0f, 0))
            {
                Canvas::SetLeft(text, pos.x - textWidth);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, -90.0f, -45.0f))
            {
                Canvas::SetLeft(text, pos.x - textWidth - textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, 90.0f, 135.0f))
            {
                Canvas::SetLeft(text, pos.x - textWidth - textHeight);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 135.0f, 180.0f))
            {
                Canvas::SetLeft(text, pos.x - textHeight);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 0, 45.0f))
            {
                Canvas::SetLeft(text, pos.x);
                Canvas::SetTop(text, pos.y + textHeight);
            }
            else if (MathUtil::IsInRange(angle, 45.0f, 90.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth + textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, -135.0f, -90.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth + textHeight);
                Canvas::SetTop(text, pos.y);
            }
            else if (MathUtil::IsInRange(angle, -180.0f, -135.0f))
            {
                Canvas::SetLeft(text, pos.x + textWidth);
                Canvas::SetTop(text, pos.y + textHeight);
            }
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
                _textBlocks[TextXAxisUnit]->Visibility = Visibility::Hidden;
                _textBlocks[TextXAxisMin]->Visibility = Visibility::Hidden;
                _textBlocks[TextXAxisMax]->Visibility = Visibility::Hidden;
                return;
            }
            float angle = getAngle(screenX);

            PlotUnit(_textBlocks[TextXAxisUnit], getMiddlePoint(screenX), XAxisUnit, angle);

            PlotAxisMin(_textBlocks[TextXAxisMin], screenX[0], _chart->XAxisMin, angle);

            PlotAxisMax(_textBlocks[TextXAxisMax], screenX[1], _chart->XAxisMax, angle);
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
                _textBlocks[TextYAxisUnit]->Visibility = Visibility::Hidden;
                _textBlocks[TextYAxisMin]->Visibility = Visibility::Hidden;
                _textBlocks[TextYAxisMax]->Visibility = Visibility::Hidden;
                return;
            }

            PlotUnit(_textBlocks[TextYAxisUnit], getMiddlePoint(screenY), YAxisUnit, -90.0f);

            TextBlock ^ minText = _textBlocks[TextYAxisMin];
            minText->Text = String::Format("{0}", _chart->YAxisMin);
            Canvas::SetLeft(minText, screenY[0].x - 2 * FontSize);
            Canvas::SetTop(minText, screenY[0].y - FontSize);

            TextBlock ^ maxText = _textBlocks[TextYAxisMax];
            maxText->Text = String::Format("{0}", _chart->YAxisMax);
            Canvas::SetLeft(maxText, screenY[1].x - 2 * FontSize);
            Canvas::SetTop(maxText, screenY[1].y);
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
                _textBlocks[TextZAxisUnit]->Visibility = Visibility::Hidden;
                _textBlocks[TextZAxisMin]->Visibility = Visibility::Hidden;
                _textBlocks[TextZAxisMax]->Visibility = Visibility::Hidden;
                return;
            }
            float angle = getAngle(screenZ);

            PlotUnit(_textBlocks[TextZAxisUnit], getMiddlePoint(screenZ), ZAxisUnit, angle);

            PlotAxisMin(_textBlocks[TextZAxisMin], screenZ[0], _chart->ZAxisMin, angle);

            PlotAxisMax(_textBlocks[TextZAxisMax], screenZ[1], _chart->ZAxisMax, angle);
        }
    }
}
}
