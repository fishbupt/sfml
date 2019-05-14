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
            , _gridXVertices(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _gridYVertices(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _gridZVertices(new sf::VertexArray(sf::PrimitiveType::Lines))
            , _borderXVertices(new sf::VertexArray(sf::PrimitiveType::Lines, 4))
            , _borderYVertices(new sf::VertexArray(sf::PrimitiveType::Lines, 4))
        {
            IsVisible = true;
            BackgroundColor = Colors::Black;
            GridColor = System::Windows::Media::Color::FromRgb(128, 128, 128);
            NumberOfXAxisDivisions = 10;
            NumberOfYAxisDivisions = 10;
            NumberOfZAxisDivisions = 0;
        }

        void GridShape::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            if (IsVisible) // draw grid lines
            {
                target->draw(*_gridXVertices, states);
                target->draw(*_gridYVertices, states);
                target->draw(*_gridZVertices, states);
            }
            else // draw border only
            {
                target->draw(*_borderXVertices, states);
                target->draw(*_borderYVertices, states);
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

            updateXAxis();
            updateYAxis();
            if (_numberOfZAxisDivisions > 0) // enable 3d grid
            {
                updateZAxis();
            }
        }

        void GridShape::NumberOfXAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfXAxisDivisions)
            {
                _numberOfXAxisDivisions = nofDivisions;
                updateXAxis();
                if (_numberOfZAxisDivisions > 0) // enable 3d grid
                {
                    updateZAxis();
                }
            }
        }

        void GridShape::NumberOfYAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfYAxisDivisions)
            {
                _numberOfYAxisDivisions = nofDivisions;
                updateYAxis();
                if (_numberOfZAxisDivisions > 0) // enable 3d grid
                {
                    updateZAxis();
                }
            }
        }

        void GridShape::NumberOfZAxisDivisions::set(int nofDivisions)
        {
            if (nofDivisions != _numberOfZAxisDivisions)
            {
                _numberOfZAxisDivisions = nofDivisions;
                if (_numberOfZAxisDivisions > 0) // enable 3d grid
                {
                    updateZAxis();
                }
            }
        }
        void GridShape::updateXAxis()
        {
            _gridXVertices->resize((_numberOfXAxisDivisions + 1) * 2);
            float xTick = (float)GridRectangle.Width / _numberOfXAxisDivisions;
            float xStart = (float)GridRectangle.X;
            float yStart = (float)GridRectangle.Y;
            float yStop = (float)GridRectangle.Height + yStart;
            sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

            for (int i = 0; i <= _numberOfXAxisDivisions; i++)
            {
                (*_gridXVertices)[2 * i].position.x = xStart + i * xTick;
                (*_gridXVertices)[2 * i].position.y = yStart;
                (*_gridXVertices)[2 * i].position.z = kMinZValue;
                (*_gridXVertices)[2 * i].color = gridColor;

                (*_gridXVertices)[2 * i + 1].position.x = xStart + i * xTick;
                (*_gridXVertices)[2 * i + 1].position.y = yStop;
                (*_gridXVertices)[2 * i + 1].position.z = kMinZValue;
                (*_gridXVertices)[2 * i + 1].color = gridColor;
            }
            (*_borderXVertices)[0] = (*_gridXVertices)[0];
            (*_borderXVertices)[1] = (*_gridXVertices)[1];
            (*_borderXVertices)[2] = (*_gridXVertices)[_gridXVertices->getVertexCount() - 2];
            (*_borderXVertices)[3] = (*_gridXVertices)[_gridXVertices->getVertexCount() - 1];
        }

        void GridShape::updateYAxis()
        {
            _gridYVertices->resize((_numberOfYAxisDivisions + 1) * 2);
            float yTick = (float)GridRectangle.Height / _numberOfYAxisDivisions;
            float yStart = (float)GridRectangle.Y;
            float xStart = (float)GridRectangle.X;
            float xStop = (float)GridRectangle.Width + xStart;
            sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

            for (int i = 0; i <= _numberOfYAxisDivisions; i++)
            {
                (*_gridYVertices)[2 * i].position.x = xStart;
                (*_gridYVertices)[2 * i].position.y = yStart + i * yTick;
                (*_gridYVertices)[2 * i].position.z = kMinZValue;
                (*_gridYVertices)[2 * i].color = gridColor;

                (*_gridYVertices)[2 * i + 1].position.x = xStop;
                (*_gridYVertices)[2 * i + 1].position.y = yStart + i * yTick;
                (*_gridYVertices)[2 * i + 1].position.z = kMinZValue;
                (*_gridYVertices)[2 * i + 1].color = gridColor;
            }
            (*_borderYVertices)[0] = (*_gridYVertices)[0];
            (*_borderYVertices)[1] = (*_gridYVertices)[1];
            (*_borderYVertices)[2] = (*_gridYVertices)[_gridYVertices->getVertexCount() - 2];
            (*_borderYVertices)[3] = (*_gridYVertices)[_gridYVertices->getVertexCount() - 1];
        }

        void GridShape::updateZAxis()
        {
            _gridZVertices->resize((_numberOfZAxisDivisions + 1) * 4 + (_numberOfXAxisDivisions + 1) * 2 +
                                   (_numberOfYAxisDivisions + 1) * 2);
            float zTick = (kMaxZValue - kMinZValue) / _numberOfZAxisDivisions;
            float zStart = kMinZValue;
            float zStop = kMaxZValue;
            float xStart = (float)GridRectangle.X;
            float xStop = (float)GridRectangle.Width + xStart;
            float xTick = (float)GridRectangle.Width / _numberOfXAxisDivisions;
            float yStart = (float)GridRectangle.Y;
            float yStop = (float)GridRectangle.Height + yStart;
            float yTick = (float)GridRectangle.Height / _numberOfYAxisDivisions;
            sf::Color gridColor = ColorUtil::ColorFrom(GridColor);

            for (int i = 0; i <= _numberOfZAxisDivisions; i++)
            {
                (*_gridZVertices)[4 * i].position.x = xStart;
                (*_gridZVertices)[4 * i].position.y = yStop;
                (*_gridZVertices)[4 * i].position.z = zStart + i * zTick;
                (*_gridZVertices)[4 * i].color = gridColor;

                (*_gridZVertices)[4 * i + 1].position.x = xStop;
                (*_gridZVertices)[4 * i + 1].position.y = yStop;
                (*_gridZVertices)[4 * i + 1].position.z = zStart + i * zTick;
                (*_gridZVertices)[4 * i + 1].color = gridColor;

                (*_gridZVertices)[4 * i + 2] = (*_gridZVertices)[4 * i];

                (*_gridZVertices)[4 * i + 3].position.x = xStart;
                (*_gridZVertices)[4 * i + 3].position.y = yStart;
                (*_gridZVertices)[4 * i + 3].position.z = zStart + i * zTick;
                (*_gridZVertices)[4 * i + 3].color = gridColor;
            }
            int offset = (_numberOfZAxisDivisions + 1) * 4;
            for (int i = 0; i <= _numberOfXAxisDivisions; i++)
            {
                (*_gridZVertices)[offset + 2 * i].position.x = xStart + i * xTick;
                (*_gridZVertices)[offset + 2 * i].position.y = yStop;
                (*_gridZVertices)[offset + 2 * i].position.z = zStart;
                (*_gridZVertices)[offset + 2 * i].color = gridColor;

                (*_gridZVertices)[offset + 2 * i + 1].position.x = xStart + i * xTick;
                (*_gridZVertices)[offset + 2 * i + 1].position.y = yStop;
                (*_gridZVertices)[offset + 2 * i + 1].position.z = zStop;
                (*_gridZVertices)[offset + 2 * i + 1].color = gridColor;
            }
            offset += (_numberOfXAxisDivisions + 1) * 2;
            for (int i = 0; i <= _numberOfYAxisDivisions; i++)
            {
                (*_gridZVertices)[offset + 2 * i].position.x = xStart;
                (*_gridZVertices)[offset + 2 * i].position.y = yStart + i * yTick;
                (*_gridZVertices)[offset + 2 * i].position.z = zStart;
                (*_gridZVertices)[offset + 2 * i].color = gridColor;

                (*_gridZVertices)[offset + 2 * i + 1].position.x = xStart;
                (*_gridZVertices)[offset + 2 * i + 1].position.y = yStart + i * yTick;
                (*_gridZVertices)[offset + 2 * i + 1].position.z = zStop;
                (*_gridZVertices)[offset + 2 * i + 1].color = gridColor;
            }
        }
    }
}
}
