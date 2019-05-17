// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
// note: put opengl headers before any other header files
#include <ScatterChart.hpp>
#include <Utils.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#using "System.Drawing.dll"
using Bitmap = System::Drawing::Bitmap;
using Graphics = System::Drawing::Graphics;

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
        ScatterChart::ScatterChart()
            : _transform(new sf::Transformable())
            , _renderTexture(new sf::RenderTexture())
        {
            IsPolorCoordinate = true;
            Content = _imageItem;
            Grid->GridRectangleChanged += gcnew EventHandler(this, &ScatterChart::OnGridRectangleChanged);
            SizeChanged += gcnew SizeChangedEventHandler(this, &ScatterChart::OnSizeChanged);
        }

        void ScatterChart::Draw()
        {
            if (!_renderTextureIsReady || _glMajorVersion < 2)
                return;

            if (_glMajorVersion >= 3)
            {
                glEnable(GL_POINT_SMOOTH);
                if (_enableCamera)
                {
                    glEnable(GL_LINE_SMOOTH);
                }
            }
            glPointSize(3.0f);

            Draw(_renderTexture.get(), sf::RenderStates::Default);
            _renderTexture->display();
            const sf::Texture& texture = _renderTexture->getTexture();
            sf::Vector2u size = texture.getSize();
            int width = (int)size.x;
            int height = (int)size.y;

            // use WriteableBitmap as intermediate object to store image pixels
            // WriteableBitmap should be locked as short as possible
            _drawnImage->Lock();
            sf::Texture::bind(&texture, sf::Texture::CoordinateType::Normalized);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)_drawnImage->BackBuffer);

            Bitmap^ renderingBitmap = gcnew Bitmap(_drawnImage->PixelWidth, _drawnImage->PixelHeight, _drawnImage->BackBufferStride,
                                                    System::Drawing::Imaging::PixelFormat::Format32bppRgb, _drawnImage->BackBuffer);
            Graphics^ renderingGraphic = Graphics::FromImage(renderingBitmap);
            try
            {
                if (_drawCustomMarkers != nullptr)
                {
                    _drawCustomMarkers(this,
                                       gcnew System::Windows::Forms::PaintEventArgs(renderingGraphic, System::Drawing::Rectangle::Empty));
                }
            }
            finally
            {
                delete renderingGraphic;
                delete renderingBitmap;
            }

            _drawnImage->AddDirtyRect(Int32Rect(0, 0, (int)_drawnImage->Width, (int)_drawnImage->Height));
            _drawnImage->Unlock();
            _imageItem->Source = _drawnImage;
        }

        void ScatterChart::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            sf::FloatBox lBox{ -1.0f, -1.0f, -1.0f, 2.0f, 2.0f, 2.0f };
            sf::Transform transform = _camera->getCamera()->getTransform();
            sf::Transform viewTransform = _camera->getCamera()->getViewTransform();
            transform.combine(viewTransform);

            sf::FloatBox gBox = transform.transformBox(lBox);
            //_camera->getCamera()->scale({ 2.0f /gBox.width, 2.0f / gBox.height, 1.0f });
            if (_enableCamera)
            {
                target->enableDepthTest(true);
                target->setView(*_camera->getCamera());
            }

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
                Rect ^ rect = Grid->ClientRectangle;
                if (pixelAtX < rect->Left)
                    pixelAtX = (float)rect->Left;
                if (pixelAtX > rect->Right)
                    pixelAtX = (float)rect->Right;
            }
            return (int)std::round(pixelAtX);
        }

        int ScatterChart::PixelAtY(double value, bool gridLimit)
        {
            sf::Vector2f pixelPoint = _transform->getTransform().transformPoint(0.0f, (float)value);
            float pixelAtY = pixelPoint.y;
            Rect ^ rect = Grid->ClientRectangle;
            if (gridLimit)
            {
                if (pixelAtY < rect->Bottom)
                    pixelAtY = (float)rect->Bottom;
                if (pixelAtY > rect->Top)
                    pixelAtY = (float)rect->Top;
            }
            return (int)std::round(rect->Height - pixelAtY);
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

        PointsShape^ ScatterChart::CreateShape()
        {
            return gcnew PointsShape();
        }

        void ScatterChart::CreateRenderTexture()
        {
            _renderTexture->setActive(false);
            int width = (int)std::max(10.0, ActualWidth);
            int height = (int)std::max(10.0, ActualHeight);
            sf::ContextSettings ctxSettings{ 24, 8 };
            _renderTextureIsReady = _renderTexture->create(width, height);
            _renderTexture->setActive(true);
            _renderTexture->setSmooth(true);

            //_camera->Position = sf::Vector3f(width / 2.0f, height / 2.0f, 1.0f);
            _camera->Position = sf::Vector3f(1.0f, 1.0f, 1.0f);
            //_camera->Scale = sf::Vector3f(2.0f / width, 2.0f / height, 1.0f);
            //_camera->getCamera()->setWidth(width);
            //_camera->getCamera()->setHeight(height);
            _drawnImage = gcnew WriteableBitmap(width, height, 96, 96, PixelFormats::Pbgra32, BitmapPalettes::WebPalette);

            if (_enableCamera)
                Grid->WindowRectangle = Rect(-1.0f, -1.0f, 2.0f, 2.0f);
            else
                Grid->WindowRectangle = Rect(0, 0, width, height);


            int majorVersion = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
            _glMajorVersion = majorVersion;
        }

        void ScatterChart::UpdateTransform()
        {
            float xRange = (float)Grid->WindowRectangle.Width;
            float yRange = (float)Grid->WindowRectangle.Height;
            float zMinValue = Grid->kMinZValue;
            float zMaxValue = Grid->kMaxZValue;
            float zRange = zMaxValue - zMinValue;
            if (IsPolorCoordinate)
            {
                double aspectRatio = xRange / yRange;
                double yWidth = YAxisMax - YAxisMin;
                double xWidth = yWidth * aspectRatio;
                double xHalf = xWidth / 2;
                _xAxisMin = -xHalf;
                _xAxisMax = xHalf;
            }
            if (_enableCamera)
            {
                sf::Vector3f origin((float)(XAxisMin + XAxisMax),
                    (float)(YAxisMin + YAxisMax),
                    (float)(ZAxisMin + ZAxisMax));
                _transform->setOrigin(origin);
            }
            else
            {
                sf::Vector3f position(-(float)XAxisMin * xRange / (float)(XAxisMax - XAxisMin),
                                      -(float)YAxisMin * yRange / (float)(YAxisMax - YAxisMin));
                _transform->setPosition(position);
            }
            sf::Vector3f scale(xRange / (float)(XAxisMax - XAxisMin),
                               yRange / (float)(YAxisMax - YAxisMin),
                               zRange / (float)(ZAxisMax - ZAxisMin));

            //_transform->setPosition(position);
            _transform->setScale(scale);
        }

        void ScatterChart::OnSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
        {
            CreateRenderTexture();
            Draw();
        }

        void ScatterChart::OnGridRectangleChanged(Object^ sender, EventArgs^ e)
        {
            UpdateTransform();
        }
    }
}
}
