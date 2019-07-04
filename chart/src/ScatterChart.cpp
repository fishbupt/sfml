// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
// note: put opengl headers before any other header files
#include <ScatterChart.hpp>
#include <Utils.hpp>
#include <sstream>
#include <iomanip>
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
            _canvas = gcnew Canvas();
            _image = gcnew Image();
            _camera = gcnew OrbitCamera(45.0f, 0.1f, 100.0f);
            Content = _canvas;
            _canvas->Children->Add(_image);

            _annotation = gcnew Graph::Annotation(this);
            _annotation->XAxisUnit = "sym";
            _annotation->YAxisUnit = "dBm";
            _annotation->ZAxisUnit = "carr";
            _annotation->FontSize = 13;
            _annotation->FontFamily = gcnew System::Windows::Media::FontFamily("Arial");
            _annotation->Color = System::Windows::Media::Colors::Red;

            IsPolorCoordinate = true;

            // flip image vertically, as image copied from renderTexture is flipped vertically, so we need unflip it
            _image->RenderTransformOrigin = Point(0.5, 0.5);
            ScaleTransform^ flipTrans = gcnew ScaleTransform();
            flipTrans->ScaleY = Is3DEnabled ? -1 : 1;
            _image->RenderTransform = flipTrans;

            Grid->GridRectangleChanged += gcnew EventHandler(this, &ScatterChart::OnGridRectangleChanged);
            SizeChanged += gcnew SizeChangedEventHandler(this, &ScatterChart::OnSizeChanged);
        }

        ScatterChart::~ScatterChart()
        {
            delete _canvas;
            delete _image;
            delete _grid;
            // clang-format off
            for each(PointsShape ^ shape in DataShapes)
            {
                delete shape;
            }
            // clang-format on
            delete _dataShapes;
            delete _annotation;
            delete _camera;
            this->!ScatterChart();
        }

        ScatterChart::!ScatterChart()
        {
        }

        void ScatterChart::Draw()
        {
            if (!_renderTextureIsReady || _glMajorVersion < 2)
                return;

            if (_glMajorVersion >= 3)
            {
                glEnable(GL_POINT_SMOOTH);
                if (Is3DEnabled)
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

            Draw(_renderTexture.get(), sf::RenderStates::Default);
            _renderTexture->display();
            const sf::Texture& texture = _renderTexture->getTexture();

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
            _image->Source = _drawnImage;
        }

        void ScatterChart::Draw(sf::RenderTarget* target, sf::RenderStates states)
        {
            if (Is3DEnabled)
            {
                if (_camera->IsOrthographicProjection)
                {
                    sf::Transform transform = _camera->getCamera()->getTransform();
                    sf::Transform viewTransform = _camera->getCamera()->getViewTransform();
                    transform.combine(viewTransform);

                    sf::FloatRect tRect = transform.transformBoxToRect(Grid->PlotBoxBounds);
                    float minSide = std::min(ActualWidth, ActualHeight);
                    if (minSide > 0)
                    {
                        float scaleRatio = (minSide - 5 * _annotation->FontSize) / minSide * 2;
                        float scaleFactor = scaleRatio / std::max(tRect.width, tRect.height);
                        _camera->ChangeScale(scaleFactor, scaleFactor, 1.0f);
                    }
                }
                target->enableDepthTest(true);
                target->setView(*_camera->getCamera());

                Grid->ShowTopPlane(!(_camera->Elevation > 0));
                Grid->ShowFrontPlane(!MathUtil::IsInRange(_camera->Azimuth, -90.0f, 90.0f));
                Grid->ShowRightPlane(!MathUtil::IsInRange(_camera->Azimuth, -180.0f, 0.0f));
            }
            else
                target->enableDepthTest(false);

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
            if(Is3DEnabled)
                DrawAnnotations(target, sf::RenderStates::Default);
        }

        void ScatterChart::DrawAnnotations(sf::RenderTarget* target, sf::RenderStates states)
        {
            _annotation->Draw(target, states);
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
            if (Is3DEnabled)
                _renderTextureIsReady = _renderTexture->create(width, height, sf::ContextSettings{ 24, 0, 8 });
            else
                _renderTextureIsReady = _renderTexture->create(width, height);

            _renderTexture->setActive(true);
            _renderTexture->setSmooth(true);
            _camera->Position = sf::Vector3f(1.0f, 1.0f, 1.0f);
            _drawnImage = gcnew WriteableBitmap(width, height, 96, 96, PixelFormats::Pbgra32, BitmapPalettes::WebPalette);

            Grid->WindowRectangle = Rect(0, 0, width, height);

            int majorVersion = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
            _glMajorVersion = majorVersion;
        }

        void ScatterChart::UpdateTransform()
        {
            float xRange = (float)Grid->GridRectangle.Width;
            float yRange = (float)Grid->GridRectangle.Height;
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
            if (Is3DEnabled)
            {
                sf::Vector3f origin((float)(XAxisMin + XAxisMax) / 2.0f,
                    (float)(YAxisMin + YAxisMax) / 2.0f,
                    (float)(ZAxisMin + ZAxisMax) / 2.0f);
                _transform->setPosition({ 0,0,0 });
                _transform->setOrigin(origin);
            }
            else
            {
                sf::Vector3f position(-(float)XAxisMin * xRange / (float)(XAxisMax - XAxisMin),
                                      -(float)YAxisMin * yRange / (float)(YAxisMax - YAxisMin));
                _transform->setOrigin({ 0, 0, 0 });
                _transform->setPosition(position);
            }
            sf::Vector3f scale(xRange / (float)(XAxisMax - XAxisMin),
                               yRange / (float)(YAxisMax - YAxisMin),
                               zRange / (float)(ZAxisMax - ZAxisMin));

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
