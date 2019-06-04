// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include "GridShape.hpp"
#include "Utils.hpp"

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {

        GridShape::GridShape()
            : _numberOfXAxisDivisions(0)
            , _numberOfYAxisDivisions(0)
            , _numberOfZAxisDivisions(0)
            , _xyPlaneGrid(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _xyPlaneBorder(new sf::VertexArray(sf::PrimitiveType::Lines, 8))
            , _xyPlaneGridUpdated(false)
            , _xzPlaneGrid(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _xzPlaneBorder(new sf::VertexArray(sf::PrimitiveType::Lines, 8))
            , _xzPlaneGridUpdated(false)
            , _yzPlaneGrid(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _yzPlaneBorder(new sf::VertexArray(sf::PrimitiveType::Lines, 8))
            , _yzPlaneGridUpdated(false)
            , _showTopPlane(true)
            , _showFrontPlane(true)
            , _showRightPlane(true)
            , _plotBox(new sf::Cuboid())
        {
            IsVisible = true;
            BackgroundColor = Colors::Black;
            GridColor = System::Windows::Media::Color::FromRgb(128, 128, 128);
            NumberOfXAxisDivisions = 10;
            NumberOfYAxisDivisions = 10;
            NumberOfZAxisDivisions = 0;
            _plotBox->setSize({2.0f, 2.0f, 2.0f});
            _plotBox->setColor(sf::Color::Transparent);
            _plotBox->setBorderColor(sf::Color::White);
        }

        void GridShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            UpdateXYPlaneGrid();
            if (IsVisible) // draw grid lines
            {
                target->draw(*_xyPlaneGrid, states);
            }
            else // draw border only
            {
                target->draw(*_xyPlaneBorder, states);
            }
            if (Is3DEnabled)
            {
                UpdateXZPlaneGrid();
                UpdateYZPlaneGrid();
                if (IsVisible)
                {
                    target->draw(*_xzPlaneGrid, states);
                    target->draw(*_yzPlaneGrid, states);
                }
                else
                {
                    target->draw(*_xzPlaneBorder, states);
                    target->draw(*_yzPlaneBorder, states);
                }
                //target->draw(*_plotBox, states);
            }
        }

        void GridShape::WindowRectangle::set(Rect rect)
        {
            if (rect.Size.IsEmpty || rect == _windowRectangle)
                return;

            _windowRectangle = rect;
            GridRectangle = Rect(_windowRectangle.X + kBorderPixelOffset, _windowRectangle.Y + kBorderPixelOffset,
                                 std::max(0.0, _windowRectangle.Width - 2 * kBorderPixelOffset),
                                 std::max(0.0, _windowRectangle.Height - 2 * kBorderPixelOffset));
            ClientRectangle = GridRectangle;
            GridRectangleChanged(this, nullptr);

            _xyPlaneGridUpdated = false;
            _xzPlaneGridUpdated = false;
            _yzPlaneGridUpdated = false;
        }

        void GridShape::NumberOfXAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfXAxisDivisions)
            {
                _numberOfXAxisDivisions = nofDivisions;
                _xyPlaneGridUpdated = false;
                _xzPlaneGridUpdated = false;
            }
        }

        void GridShape::NumberOfYAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfYAxisDivisions)
            {
                _numberOfYAxisDivisions = nofDivisions;
                _xyPlaneGridUpdated = false;
                _yzPlaneGridUpdated = false;
            }
        }

        void GridShape::NumberOfZAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfZAxisDivisions)
            {
                _numberOfZAxisDivisions = nofDivisions;
                _xzPlaneGridUpdated = false;
                _yzPlaneGridUpdated = false;
            }
        }

        void GridShape::ShowTopPlane(bool showTop)
        {
            if (showTop != _showTopPlane)
            {
                _showTopPlane = showTop;
                _xzPlaneGridUpdated = false;
            }

        }

        void GridShape::ShowFrontPlane(bool showFront)
        {
            if (showFront != _showFrontPlane)
            {
                _showFrontPlane = showFront;
                _xyPlaneGridUpdated = false;
            }
        }

        void GridShape::ShowRightPlane(bool showRight)
        {
            if (showRight != _showRightPlane)
            {
                _showRightPlane = showRight;
                _yzPlaneGridUpdated = false;
            }
        }

        void GridShape::UpdateXYPlaneGrid()
        {
            if (!_xyPlaneGridUpdated)
            {
                _xyPlaneGrid->resize((_numberOfXAxisDivisions + 1) * 2 + (_numberOfYAxisDivisions + 1) * 2);
                float xTick = (float)GridRectangle.Width / _numberOfXAxisDivisions;
                float yTick = (float)GridRectangle.Height / _numberOfYAxisDivisions;
                float xStart = (float)GridRectangle.X;
                float xStop = (float)GridRectangle.Width + xStart;
                float yStart = (float)GridRectangle.Y;
                float yStop = (float)GridRectangle.Height + yStart;
                float zValue = _showFrontPlane ? kMaxZValue : kMinZValue;
                zValue = Is3DEnabled ? zValue : 0;

                sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

                sf::Vertex *xGrid = _xyPlaneGrid->data();
                for (int i = 0; i <= _numberOfXAxisDivisions; i++)
                {
                    xGrid[2 * i].position.x = xStart + i * xTick;
                    xGrid[2 * i].position.y = yStart;
                    xGrid[2 * i].position.z = zValue;
                    xGrid[2 * i].color = gridColor;

                    xGrid[2 * i + 1].position.x = xStart + i * xTick;
                    xGrid[2 * i + 1].position.y = yStop;
                    xGrid[2 * i + 1].position.z = zValue;
                    xGrid[2 * i + 1].color = gridColor;
                }

                sf::Vertex *yGrid = _xyPlaneGrid->data() + (_numberOfXAxisDivisions + 1) * 2;
                for (int i = 0; i <= _numberOfYAxisDivisions; i++)
                {
                    yGrid[2 * i].position.x = xStart;
                    yGrid[2 * i].position.y = yStart + i * yTick;
                    yGrid[2 * i].position.z = zValue;
                    yGrid[2 * i].color = gridColor;

                    yGrid[2 * i + 1].position.x = xStop;
                    yGrid[2 * i + 1].position.y = yStart + i * yTick;
                    yGrid[2 * i + 1].position.z = zValue;
                    yGrid[2 * i + 1].color = gridColor;
                }
                sf::Vertex *border = _xyPlaneBorder->data();
                border[0] = xGrid[0];
                border[1] = xGrid[1];
                border[2] = xGrid[_numberOfXAxisDivisions * 2];
                border[3] = xGrid[_numberOfXAxisDivisions * 2 + 1];
                border[4] = yGrid[0];
                border[5] = yGrid[1];
                border[6] = yGrid[_numberOfYAxisDivisions * 2];
                border[7] = yGrid[_numberOfYAxisDivisions * 2 + 1];

                _xyPlaneGridUpdated = true;
            }
        }

        void GridShape::UpdateXZPlaneGrid()
        {
            if (!_xzPlaneGridUpdated)
            {
                _xzPlaneGrid->resize((_numberOfXAxisDivisions + 1) * 2 + (_numberOfZAxisDivisions + 1) * 2);
                float xTick = (float)GridRectangle.Width / _numberOfXAxisDivisions;
                float zTick = (kMaxZValue - kMinZValue) / _numberOfZAxisDivisions;
                float xStart = (float)GridRectangle.X;
                float xStop = (float)GridRectangle.Width + xStart;
                float zStart = kMinZValue;
                float zStop = kMaxZValue;
                float yValue = (float)GridRectangle.Y;
                if (_showTopPlane) yValue += (float)GridRectangle.Height;

                sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

                sf::Vertex *xGrid = _xzPlaneGrid->data();
                for (int i = 0; i <= _numberOfXAxisDivisions; i++)
                {
                    xGrid[2 * i].position.x = xStart + i * xTick;
                    xGrid[2 * i].position.y = yValue;
                    xGrid[2 * i].position.z = zStart;
                    xGrid[2 * i].color = gridColor;

                    xGrid[2 * i + 1].position.x = xStart + i * xTick;
                    xGrid[2 * i + 1].position.y = yValue;
                    xGrid[2 * i + 1].position.z = zStop;
                    xGrid[2 * i + 1].color = gridColor;
                }

                sf::Vertex *zGrid = _xzPlaneGrid->data() + (_numberOfXAxisDivisions + 1) * 2;
                for (int i = 0; i <= _numberOfZAxisDivisions; i++)
                {
                    zGrid[2 * i].position.x = xStart;
                    zGrid[2 * i].position.y = yValue;
                    zGrid[2 * i].position.z = zStart + i * zTick;
                    zGrid[2 * i].color = gridColor;

                    zGrid[2 * i + 1].position.x = xStop;
                    zGrid[2 * i + 1].position.y = yValue;
                    zGrid[2 * i + 1].position.z = zStart + i * zTick;
                    zGrid[2 * i + 1].color = gridColor;
                }
                sf::Vertex *border = _xzPlaneBorder->data();
                border[0] = xGrid[0];
                border[1] = xGrid[1];
                border[2] = xGrid[_numberOfXAxisDivisions * 2];
                border[3] = xGrid[_numberOfXAxisDivisions * 2 + 1];
                border[4] = zGrid[0];
                border[5] = zGrid[1];
                border[6] = zGrid[_numberOfZAxisDivisions * 2];
                border[7] = zGrid[_numberOfZAxisDivisions * 2 + 1];

                _xzPlaneGridUpdated = true;
            }
        }

        void GridShape::UpdateYZPlaneGrid()
        {
            if (!_yzPlaneGridUpdated)
            {
                _yzPlaneGrid->resize((_numberOfYAxisDivisions + 1) * 2 + (_numberOfZAxisDivisions + 1) * 2);
                float yTick = (float)GridRectangle.Height / _numberOfYAxisDivisions;
                float zTick = (kMaxZValue - kMinZValue) / _numberOfZAxisDivisions;
                float yStart = (float)GridRectangle.Y;
                float yStop = (float)GridRectangle.Height + yStart;
                float zStart = kMinZValue;
                float zStop = kMaxZValue;
                float xValue = (float)GridRectangle.X;
                if(_showRightPlane) xValue += GridRectangle.Width;

                sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

                sf::Vertex *yGrid = _yzPlaneGrid->data();
                for (int i = 0; i <= _numberOfYAxisDivisions; i++)
                {
                    yGrid[2 * i].position.x = xValue;
                    yGrid[2 * i].position.y = yStart + i * yTick;
                    yGrid[2 * i].position.z = zStart;
                    yGrid[2 * i].color = gridColor;

                    yGrid[2 * i + 1].position.x = xValue;
                    yGrid[2 * i + 1].position.y = yStart + i * yTick;
                    yGrid[2 * i + 1].position.z = zStop;
                    yGrid[2 * i + 1].color = gridColor;
                }

                sf::Vertex *zGrid = _yzPlaneGrid->data() + (_numberOfYAxisDivisions + 1) * 2;
                for (int i = 0; i <= _numberOfZAxisDivisions; i++)
                {
                    zGrid[2 * i].position.x = xValue;
                    zGrid[2 * i].position.y = yStart;
                    zGrid[2 * i].position.z = zStart + i * zTick;
                    zGrid[2 * i].color = gridColor;

                    zGrid[2 * i + 1].position.x = xValue;
                    zGrid[2 * i + 1].position.y = yStop;
                    zGrid[2 * i + 1].position.z = zStart + i * zTick;
                    zGrid[2 * i + 1].color = gridColor;
                }
                sf::Vertex *border = _yzPlaneBorder->data();
                border[0] = yGrid[0];
                border[1] = yGrid[1];
                border[2] = yGrid[_numberOfYAxisDivisions * 2];
                border[3] = yGrid[_numberOfYAxisDivisions * 2 + 1];
                border[4] = zGrid[0];
                border[5] = zGrid[1];
                border[6] = zGrid[_numberOfZAxisDivisions * 2];
                border[7] = zGrid[_numberOfZAxisDivisions * 2 + 1];
                _yzPlaneGridUpdated = true;
            }
        }

        bool GridShape::Is3DEnabled::get()
        {
            return _numberOfZAxisDivisions > 0;
        }
}
}
}
