// Copyright (c) 2019 Keysight Technologies. All rights reserved.
#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Camera.hpp>
#include <SFML/Window/Context.hpp>
#include <ScopedPtr.hpp>
#include <PointsShape.hpp>
#include <GridShape.hpp>
#include <Drawable.hpp>

#using "WindowsBase.dll"
#using "PresentationFramework.dll"
#using "PresentationCore.dll"
#using "System.Windows.Forms.dll"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows;
using namespace System::Windows::Controls;
using namespace System::Windows::Media::Imaging;

namespace Xsa
{
namespace Presentation
{
    namespace Graph
    {
        // clang-format off
        public ref class ScatterChart : public UserControl, public Drawable
        // clang-format on
        {
        public:
            ScatterChart();

            /// <summary>
            /// This Method do library level initialization
            /// SFML use a shared OpenGL context to create other OpenGL context after the first context was created
            /// call this method to create a shared OpenGL context
            /// </summary>
            static void Initialize()
            {
                sf::Context ctx;
            }

            /// <summary>
            /// Draw object to the control
            /// </summary>
            void Draw();

            virtual void Draw(sf::RenderTarget* target, sf::RenderStates states);

            /// <summary>
            /// Get the X-pixel position corresponding the passed X-axis value.
            /// </summary>
            /// <param name="value">The X-pixel value.</param>
            /// <param name="gridLimit">Whether to limit the pixel position to fit within the grid</param>
            /// <returns>The X-pixel position.</returns>
            int PixelAtX(double value, bool gridLimit);

            /// <summary>
            /// Get the Y-pixel position corresponding the passed Y-axis value.
            /// </summary>
            /// <param name="value">The Y-pixel value</param>
            /// <param name="gridLimit">Whether to limit the pixel position to fit within the grid.</param>
            /// <returns>The Y-pixel position</returns>
            int PixelAtY(double value, bool gridLimit);

            /// <summary>
            /// Get the X-axis value corresponding the passed X-pixel position.
            /// </summary>
            /// <param name="value">The X-pixel position.</param>
            /// <returns>The X-axis value</returns>
            float XAtPixel(int value);

            /// <summary>
            /// Get the Y-axis value corresponding the passed Y-pixel position.
            /// </summary>
            /// <param name="value">The Y-pixel position.</param>
            /// <returns>The Y-axis value.</returns>
            float YAtPixel(int value);

            void SetCameraPitch(float pitch)
            {
                //_camera->setPitch(pitch);
            }

            void SetCameraYaw(float yaw)
            {
                //_camera->setYaw(yaw);
            }

            void SetCameraZ(float z)
            {
                //_camera->moveBackward(z / 10.0f);
            }
#pragma region Properties
        private:
            GridShape ^ _grid = gcnew GridShape();
            List<PointsShape ^> ^ _dataShapes = gcnew List<PointsShape ^>();

            double _xAxisMin = 0;
            double _xAxisMax = 1.0;
            double _yAxisMin = 0;
            double _yAxisMax = 1.0;

            array<Color> ^ _segColors = gcnew array<Color>(0);
            array<Color> ^ _traceColors = gcnew array<Color>(1);

        public:
            // clang-format off
            property GridShape ^ Grid
            {
                GridShape ^ get()
                {
                    return _grid;
                }
            }

            property List<PointsShape ^> ^ DataShapes
            {
                List<PointsShape ^> ^ get()
                {
                    return _dataShapes;
                }
            }

            /// <summary>
            /// For Polor Coordinate, the XAxisMin and XAxisMax are determinated by YAxisMin and YAxisMax
            /// </summary>
            property bool IsPolorCoordinate;
            // clang-format on

            property double XAxisMin
            {
                double get()
                {
                    return _xAxisMin;
                }
                void set(double value)
                {
                    if (IsPolorCoordinate || _xAxisMin == value || value >= _xAxisMax)
                        return;
                    _xAxisMin = value;
                    UpdateTransform();
                }
            }

            property double XAxisMax
            {
                double get()
                {
                    return _xAxisMax;
                }
                void set(double value)
                {
                    if (IsPolorCoordinate || _xAxisMax == value || value <= _xAxisMin)
                        return;
                    _xAxisMax = value;
                    UpdateTransform();
                }
            }

            property double YAxisMin
            {
                double get()
                {
                    return _yAxisMin;
                }
                void set(double value)
                {
                    if (_yAxisMin == value || value >= _yAxisMax)
                        return;
                    _yAxisMin = value;
                    UpdateTransform();
                }
            }

            property double YAxisMax
            {
                double get()
                {
                    return _yAxisMax;
                }
                void set(double value)
                {
                    if (_yAxisMax == value || value <= _yAxisMin)
                        return;
                    _yAxisMax = value;
                    UpdateTransform();
                }
            }

            property int NumberOfTraces
            {
                int get()
                {
                    return DataShapes->Count;
                }
                void set(int value)
                {
                    if (value != DataShapes->Count)
                    {
                        DataShapes->Clear();
                        for (int i = 0; i < value; i++)
                        {
                            PointsShape ^ shape = CreateShape();
                            int colorIndex = i < _traceColors->Length ? i : 0;
                            shape->TraceColor = _traceColors[colorIndex];
                            shape->SegColors = _segColors;
                            DataShapes->Add(shape);
                        }
                    }
                }
            }
            // clang-format off
            property array<Color> ^ TraceColors
            {
                array<Color> ^ get()
                {
                    return _traceColors;
                }
                void set(array<Color> ^ value)
                {
                    if (_traceColors != value)
                    {
                        _traceColors = value;
                        for (int i = 0; i < DataShapes->Count; i++)
                        {
                            int colorIndex = i < _traceColors->Length ? i : 0;
                            DataShapes[i]->TraceColor = _traceColors[colorIndex];
                        }
                    }
                }
            }

            property array<Color> ^ SegColors
            {
                array<Color> ^ get()
                {
                    return _segColors;
                }
                void set(array<Color> ^ value)
                {
                    if (_segColors != value)
                    {
                        _segColors = value;
                        for (int i = 0; i < DataShapes->Count; i++)
                        {
                            DataShapes[i]->SegColors = _segColors;
                        }
                    }
                }
            }

            property bool Enable3D
            {
                bool get()
                {
                    return _enableCamera;
                }
                void set(bool value)
                {
                    if (_enableCamera != value)
                        _enableCamera = value;
                    if (!_enableCamera)
                        _renderTexture->setView(_renderTexture->getDefaultView());
                }
            }

            #pragma endregion Properties

            #pragma region Events
        private:
            System::Windows::Forms::PaintEventHandler ^ _drawCustomMarkers;
        public:
            event System::Windows::Forms::PaintEventHandler ^ DrawCustomMarkers
            {
                void add(System::Windows::Forms::PaintEventHandler^ p){ _drawCustomMarkers += p; }
                void remove(System::Windows::Forms::PaintEventHandler ^p){ _drawCustomMarkers -= p; }
            }
        #pragma endregion Events
        protected :
            /// <summary>
            /// Factory method to create different shapes
            /// </summary>
            /// <returns></returns>
            virtual PointsShape ^ CreateShape();
            // clang-format on

        private:
            void CreateRenderTexture();

            void OnSizeChanged(Object ^ sender, SizeChangedEventArgs ^ e);

            void OnGridRectangleChanged(Object ^ sender, EventArgs ^ e);

            void UpdateTransform();

        private:
            scoped_ptr<sf::Transformable> _transform;
            scoped_ptr<sf::RenderTexture> _renderTexture;
            bool _renderTextureIsReady = false;
            Image ^ _imageItem = gcnew Image();
            WriteableBitmap ^ _drawnImage; // displaying bitmap
            int _glMajorVersion;

            scoped_ptr<sf::Camera> _camera;
            bool _enableCamera = false; // true to apply 3d camera
        };
    }
}
}
