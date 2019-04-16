// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <ScatterChart.hpp>
#include <Utils.hpp>
//#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        ScatterChart::ScatterChart()
            : _transform(new sf::Transformable())
            , _renderTexture(new sf::RenderTexture())
        {
            Content = _imageItem;
            Grid->GridRectangleChanged += gcnew EventHandler(this, &ScatterChart::OnGridRectangleChanged);
            SizeChanged += gcnew SizeChangedEventHandler(this, &ScatterChart::OnSizeChanged);
        }

        void ScatterChart::Draw()
        {
            if (!_renderTextureIsReady)
                return;

            //glCheck(glEnable(GL_POINT_SMOOTH));
            //glCheck(glPointSize(3.0f));

            //Draw(_renderTexture.get(), sf::RenderStates::Default);
            //_renderTexture->display();
            //const sf::Texture& texture = _renderTexture->getTexture();
            //sf::Vector2u size = texture.getSize();
            //int width = (int)size.x;
            //int height = (int)size.y;

            //// use WriteableBitmap as intermediate object to store image pixels
            //// WriteableBitmap should be locked as short as possible
            //_drawnImage->Lock();
            //sf::Texture::bind(&texture, sf::Texture::CoordinateType::Normalized);
            //glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)_drawnImage->BackBuffer));

            ////Bitmap ^ renderingBitmap = gcnew Bitmap(_drawnImage->PixelWidth, _drawnImage->PixelHeight, _drawnImage->BackBufferStride,
            ////                                        System::Drawing::Imaging::PixelFormat::Format32bppRgb, _drawnImage->BackBuffer);
            ////using(Graphics ^ renderingGraphic = Graphics.FromImage(renderingBitmap))
            ////{
            ////    DrawCustomMarker ?.Invoke(this, new PaintEventArgs(renderingGraphic, System.Drawing.Rectangle.Empty));
            ////    // DrawCustomMarkers(renderingGraphic); rase event
            ////}

            //_drawnImage->AddDirtyRect(Int32Rect(0, 0, (int)_drawnImage->Width, (int)_drawnImage->Height));
            //_drawnImage->Unlock();
            //_imageItem->Source = _drawnImage;
        }

        void ScatterChart::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            sf::Color backColor = ColorUtil::ColorFrom(Grid->BackgroundColor);
            target->clear(backColor);
            Grid->Draw(target, states);
            states.transform *= _transform->getTransform();
            // clang-format off
            for each(PointsShape ^ shape in DataShapes)
            {
                shape->Draw(target, states);
            }
            // clang-format on
        }

        int ScatterChart::PixelAtX(double value, bool gridLimit)
        {
            sf::Vector2f pixelPoint = _transform->getTransform().transformPoint((float)value, 0.0f);
            float pixelAtX = pixelPoint.x;
            if (gridLimit)
            {
                Rect rect = Grid->ClientRectangle;
                if (pixelAtX < rect.Left)
                    pixelAtX = (float)rect.Left;
                if (pixelAtX > rect.Right)
                    pixelAtX = (float)rect.Right;
            }
            return (int)std::round(pixelAtX);
        }

        int ScatterChart::PixelAtY(double value, bool gridLimit)
        {
            sf::Vector2f pixelPoint = _transform->getTransform().transformPoint(0.0f, (float)value);
            float pixelAtY = pixelPoint.y;
            Rect rect = Grid->ClientRectangle;
            if (gridLimit)
            {
                if (pixelAtY < rect.Bottom)
                    pixelAtY = (float)rect.Bottom;
                if (pixelAtY > rect.Top)
                    pixelAtY = (float)rect.Top;
            }
            return (int)std::round(rect.Height - pixelAtY);
        }

        float ScatterChart::XAtPixel(int value)
        {
            sf::Vector2f point = _transform->getInverseTransform().transformPoint((float)value, 0.0f);
            float xVal = point.x;
            if (xVal < XAxisMin)
                xVal = (float)XAxisMin;
            if (xVal > XAxisMax)
                xVal = (float)XAxisMax;
            return xVal;
        }

        float ScatterChart::YAtPixel(int value)
        {
            sf::Vector2f point = _transform->getInverseTransform().transformPoint(0.0f, (float)value);
            float yVal = point.y;
            if (yVal < YAxisMin)
                yVal = (float)YAxisMin;
            if (yVal > YAxisMax)
                yVal = (float)YAxisMax;
            return yVal;
        }

        PointsShape ^ ScatterChart::CreateShape()
        {
            return gcnew PointsShape();
        }

        void ScatterChart::CreateRenderTexture()
        {
            _renderTexture->setActive(false);
            int width = (int)std::max(10.0, ActualWidth);
            int height = (int)std::max(10.0, ActualHeight);
            _renderTextureIsReady = _renderTexture->create(width, height);
            _renderTexture->setActive(true);
            _renderTexture->setSmooth(true);

            _drawnImage = gcnew WriteableBitmap(width, height, 96, 96, PixelFormats::Pbgra32, BitmapPalettes::WebPalette);

            Grid->WindowRectangle = Rect(0, 0, width, height);
        }

        void ScatterChart::UpdateTransform()
        {
            float clientWidth = (float)Grid->WindowRectangle.Width;
            float clientHeight = (float)Grid->WindowRectangle.Height;
            if (IsPolorCoordinate)
            {
                double aspectRatio = clientWidth / clientHeight;
                double yWidth = YAxisMax - YAxisMin;
                double xWidth = yWidth * aspectRatio;
                double xHalf = xWidth / 2;
                _xAxisMin = -xHalf;
                _xAxisMax = xHalf;
            }
            sf::Vector2f position(-(float)XAxisMin * clientWidth / (float)(XAxisMax - XAxisMin),
                                  -(float)YAxisMin * clientHeight / (float)(YAxisMax - YAxisMin));
            sf::Vector2f scale(clientWidth / (float)(XAxisMax - XAxisMin), clientHeight / (float)(YAxisMax - YAxisMin));
            _transform->setPosition(position);
            _transform->setScale(scale);
        }

        void ScatterChart::OnSizeChanged(Object ^ sender, SizeChangedEventArgs ^ e)
        {
            CreateRenderTexture();
            Draw();
        }

        void ScatterChart::OnGridRectangleChanged(Object ^ sender, EventArgs ^ e)
        {
            UpdateTransform();
        }
    }
}
}
