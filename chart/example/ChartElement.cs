// Copyright (c) 2019 Keysight Technologies. All rights reserved.
using System;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Bitmap = System.Drawing.Bitmap;
using Color = System.Windows.Media.Color;
using FontFamily = System.Windows.Media.FontFamily;
using Graphics = System.Drawing.Graphics;
using Image = System.Windows.Controls.Image;
using Point = System.Windows.Point;
using UserControl = System.Windows.Controls.UserControl;

namespace Xsa.Presentation.Graph
{
    public class DrawTracesEventArgs : EventArgs
    {
        public DrawTracesEventArgs(IntPtr target, IntPtr states)
        {
            Target = target;
            States = states;
        }

        public IntPtr Target;
        public IntPtr States;
    };

    public class ChartElement : UserControl, IDisposable
    {
        #region Fields

        private double _xAxisMin = -1.0;
        private double _xAxisMax = 1.0;
        private double _yAxisMin = -1.0;
        private double _yAxisMax = 1.0;
        private double _zAxisMin = -1.0;
        private double _zAxisMax = 1.0;

        private bool _enableCamera = false; // true to apply 3d camera

        private readonly Transformable _transformable = new Transformable();

        private readonly Image _image = new Image();
        private WriteableBitmap _drawnBitmap; // displaying bitmap
        private readonly RenderChart _renderChart = new RenderChart();
        private bool _renderTextureIsReady = false;

        private static SingleThreadTaskScheduler _glTakScheduler;
        #endregion

        #region  Properties

        public Canvas Canvas { get; private set; } = new Canvas();

        private GridShape _grid = new GridShape();
        public GridShape Grid { get => _grid; }

        public Annotation Annotation { get; private set; }

        private Markers _markers = new Markers(12);
        public Markers Markers { get => _markers; }

        private OrbitCamera _camera = new OrbitCamera(45.0f, 0.1f, 100.0f);
        public OrbitCamera Camera { get => _camera; }

        public Color BackgroundColor;

        public bool Is3DEnabled
        {
            get => _enableCamera;
            set
            {
                if (_enableCamera != value)
                {
                    _enableCamera = value;
                    if (!_enableCamera)
                    {
                        Annotation.Disable();
                        ((ScaleTransform)_image.RenderTransform).ScaleY = 1;
                    }
                    else
                    {

                        ((ScaleTransform)_image.RenderTransform).ScaleY = -1;
                    }
                    UpdateTransform();
                }
            }
        }

        /// <summary>
        /// For Polor Coordinate, the XAxisMin and XAxisMax are determinated by YAxisMin and YAxisMax
        /// </summary>
        public bool IsPolorCoordinate = false;

        /// <summary>
        /// Min value of X axis
        /// </summary>
        public double XAxisMin
        {
            get => _xAxisMin;

            set
            {
                if (IsPolorCoordinate || _xAxisMin == value) return;
                _xAxisMin = value;
                _xAxisMax = ClipMax(_xAxisMin, _xAxisMax);
                UpdateTransform();
            }
        }

        /// <summary>
        /// Max value of X axis
        /// </summary>
        public double XAxisMax
        {
            get => _xAxisMax;
            set
            {
                if (IsPolorCoordinate || _xAxisMax == value) return;
                _xAxisMax = value;
                _xAxisMin = ClipMin(_xAxisMin, _xAxisMax);
                UpdateTransform();
            }
        }

        /// <summary>
        /// Min value of Y axis
        /// </summary>
        public double YAxisMin
        {
            get => _yAxisMin;
            set
            {
                if (_yAxisMin == value) return;
                _yAxisMin = value;
                _yAxisMax = ClipMax(_yAxisMin, _yAxisMax);
                UpdateTransform();
            }
        }

        /// <summary>
        /// Max value of Y axis
        /// </summary>
        public double YAxisMax
        {
            get => _yAxisMax;
            set
            {
                if (_yAxisMax == value) return;
                _yAxisMax = value;
                _yAxisMin = ClipMin(_yAxisMin, _yAxisMax);
                UpdateTransform();
            }
        }

        /// <summary>
        /// Min value of Z axis
        /// </summary>
        public double ZAxisMin
        {
            get => _zAxisMin;
            set
            {
                if (_zAxisMin == value) return;
                _zAxisMin = value;
                _zAxisMax = ClipMax(_zAxisMin, _zAxisMax);
                UpdateTransform();
            }
        }

        /// <summary>
        /// Max value of Z axis
        /// </summary>
        public double ZAxisMax
        {
            get => _zAxisMax;
            set
            {
                if (_zAxisMax == value) return;
                _zAxisMax = value;
                _zAxisMin = ClipMin(_zAxisMin, _zAxisMax);
                UpdateTransform();
            }
        }

        #endregion

        #region Events

        public System.Windows.Forms.PaintEventHandler OnDrawCustomMarkers;

        public EventHandler<DrawTracesEventArgs> OnDrawTraces;

        #endregion

        static ChartElement()
        {
            AppDomain.CurrentDomain.ProcessExit += CurrentDomain_ProcessExit;
            _glTakScheduler = new SingleThreadTaskScheduler(CancellationToken.None, "OpenGLThread");
            _glTakScheduler.Start();

        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            _glTakScheduler.Complete();
        }

        public ChartElement()
        {
            BackgroundColor = Color.FromArgb(0, 0, 0, 0);
            Camera.CheckBoundary = true;
            Content = Canvas;
            Canvas.Children.Add(_image);
            Annotation = new Annotation(Camera)
            {
                FontSize = 13,
                FontFamily = new FontFamily("Arial"),
                XAxisUnit = "sym",
                YAxisUnit = "dBm",
                ZAxisUnit = "carr",
                Color = Colors.Red
            };
            foreach (var textBlock in Annotation.TextBlocks)
            {
                Canvas.Children.Add(textBlock);
            }

            // flip image vertically, as image copied from renderTexture is flipped vertically, so we need unflip it
            _image.RenderTransformOrigin = new Point(0.5, 0.5);
            ScaleTransform flipTrans = new ScaleTransform
            {
                ScaleY = Is3DEnabled ? -1 : 1
            };
            _image.RenderTransform = flipTrans;

            Grid.GridRectangleChanged += OnGridRectangleChanged;
            SizeChanged += OnSizeChanged;
        }

        #region Public Methods

        public async Task Render()
        {

            if (!_renderTextureIsReady) return;
           await _glTakScheduler.Run(() =>
           {
               try
               {
                   SetupAnnotation();
                   _renderChart.Camera = Camera;
                   _renderChart.Annotation = Annotation;
                   _renderChart.Grid = Grid;
                   _renderChart.Markers = Markers;
                   _renderChart.Transform = _transformable.NativePointer;
                   _renderChart.BackgroundColor = BackgroundColor;
                   _renderChart.Draw(Is3DEnabled);

                   OnDrawTraces?.Invoke(this,
                       new DrawTracesEventArgs(_renderChart.RenderTarget, _renderChart.RenderState));

                   _renderChart.UpdateBuffer();
               }
               catch(System.AccessViolationException e)
               {
                   Console.WriteLine("exception: " + e.Message);
               }
               catch
               {
                   Console.WriteLine("exception: ");
               }

           });
            return;
        }

        public void Draw()
        {
            if (!_renderTextureIsReady || _renderChart.RenderBuffer == IntPtr.Zero) return;

            if(Is3DEnabled)
                Annotation.Draw(_renderChart.RenderTarget);

            _drawnBitmap.Lock();
            int bytesToCopy = _drawnBitmap.PixelHeight * _drawnBitmap.PixelWidth * 4;
            unsafe
            {
                // .NET 4.6 support following method
                Buffer.MemoryCopy(_renderChart.RenderBuffer.ToPointer(), _drawnBitmap.BackBuffer.ToPointer(), bytesToCopy, bytesToCopy);

                //Span<byte> srcSpan = new Span<byte>(_renderChart.RenderBuffer.ToPointer(), bytesToCopy);
                //Span<byte> dstSpan = new Span<byte>(_drawnBitmap.BackBuffer.ToPointer(), bytesToCopy);
                //srcSpan.CopyTo(dstSpan);
            }
            using (Bitmap bitmap = new Bitmap(_drawnBitmap.PixelWidth, _drawnBitmap.PixelHeight,
                _drawnBitmap.BackBufferStride, System.Drawing.Imaging.PixelFormat.Format32bppRgb,
                _drawnBitmap.BackBuffer))
            using (Graphics g = Graphics.FromImage(bitmap))
            {
                OnDrawCustomMarkers?.Invoke(this, new PaintEventArgs(g, Rectangle.Empty));
            }

            _drawnBitmap.AddDirtyRect(new Int32Rect(0, 0, (int)_drawnBitmap.Width, (int)_drawnBitmap.Height));
            _drawnBitmap.Unlock();
            _image.Source = _drawnBitmap;
            return;
        }

        /// <summary>
        /// Get the X-pixel position corresponding the passed X-axis value.
        /// </summary>
        /// <param name="value">The X-pixel value.</param>
        /// <param name="gridLimit">Whether to limit the pixel position to fit within the grid</param>
        /// <returns>The X-pixel position.</returns>
        public int PixelAtX(double value, bool gridLimit)
        {
            Vector3f pixelPoint = _transformable.GetTransform().TransformPoint(new Vector3f((float)value, 0.0f));
            float pixelAtX = pixelPoint.X;
            if (gridLimit)
            {
                Rect rect = Grid.ClientRectangle;
                if (pixelAtX < rect.Left)
                    pixelAtX = (float)rect.Left;
                if (pixelAtX > rect.Right)
                    pixelAtX = (float)rect.Right;
            }
            return (int)Math.Round(pixelAtX);
        }

        /// <summary>
        /// Get the Y-pixel position corresponding the passed Y-axis value.
        /// </summary>
        /// <param name="value">The Y-pixel value</param>
        /// <param name="gridLimit">Whether to limit the pixel position to fit within the grid.</param>
        /// <returns>The Y-pixel position</returns>
        public int PixelAtY(double value, bool gridLimit)
        {

            Vector3f pixelPoint = _transformable.GetTransform().TransformPoint(new Vector3f(0.0f, (float)value));
            float pixelAtY = pixelPoint.Y;
            Rect rect = Grid.ClientRectangle;
            if (gridLimit)
            {
                if (pixelAtY < rect.Bottom)
                    pixelAtY = (float)rect.Bottom;
                if (pixelAtY > rect.Top)
                    pixelAtY = (float)rect.Top;
            }

            return (int)Math.Round(rect.Height - pixelAtY);
        }

        /// <summary>
        /// Get the X-axis value corresponding the passed X-pixel position.
        /// </summary>
        /// <param name="value">The X-pixel position.</param>
        /// <returns>The X-axis value</returns>
        public float XAtPixel(int value)
        {

            Vector3f point = _transformable.GetInverseTransform().TransformPoint(new Vector3f((float)value, 0.0f));
            float xVal = point.X;
            if (xVal < XAxisMin)
                xVal = (float)XAxisMin;
            if (xVal > XAxisMax)
                xVal = (float)XAxisMax;
            return xVal;
        }

        /// <summary>
        /// Get the Y-axis value corresponding the passed Y-pixel position.
        /// </summary>
        /// <param name="value">The Y-pixel position.</param>
        /// <returns>The Y-axis value.</returns>
        public float YAtPixel(int value)
        {

            Vector3f point = _transformable.GetInverseTransform().TransformPoint(new Vector3f(0.0f, (float)value));
            float yVal = point.Y;
            if (yVal < YAxisMin)
                yVal = (float)YAxisMin;
            if (yVal > YAxisMax)
                yVal = (float)YAxisMax;
            return yVal;
        }

        #endregion

        #region Help Methods

        static private bool IsInRange(float value, float low, float high)
        {
            return !(value < low) && (value < high);
        }

        private void SetupAnnotation()
        {
            if(!Is3DEnabled) return;

            Annotation.XAxisMin = XAxisMin;
            Annotation.XAxisMax = XAxisMax;
            Annotation.YAxisMin = YAxisMin;
            Annotation.YAxisMax = YAxisMax;
            Annotation.ZAxisMin = ZAxisMin;
            Annotation.ZAxisMax = ZAxisMax;
        }
        private void UpdateTransform()
        {
            float xRange = (float)Grid.GridRectangle.Width;
            float yRange = (float)Grid.GridRectangle.Height;
            float zMinValue = GridShape.kMinZValue;
            float zMaxValue = GridShape.kMaxZValue;
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

            Vector3f scale = new Vector3f(xRange / (float)(XAxisMax - XAxisMin),
                yRange / (float)(YAxisMax - YAxisMin),
                zRange / (float)(ZAxisMax - ZAxisMin));

            if (Is3DEnabled)
            {
                Vector3f origin = new Vector3f((float)(XAxisMin + XAxisMax) / 2.0f,
                    (float)(YAxisMin + YAxisMax) / 2.0f,
                    (float)(ZAxisMin + ZAxisMax) / 2.0f);
                _transformable.Position = new Vector3f(0, 0, 0);
                _transformable.Origin = origin;
            }
            else
            {
                Vector3f position = new Vector3f(-(float)XAxisMin * xRange / (float)(XAxisMax - XAxisMin),
                    -(float)YAxisMin * yRange / (float)(YAxisMax - YAxisMin));
                _transformable.Origin = new Vector3f(0, 0, 0);
                _transformable.Position = position;

                scale.Z = 1.0f;
            }

            _transformable.Scale = scale;
        }

        private double ClipMin(double min, double max)
        {
            if (max < min)
                return max - Math.Abs(max) * 0.1;
            return min;
        }

        private double ClipMax(double min, double max)
        {
            if (min > max)
                return min + Math.Abs(min) * 0.1;
            return max;
        }

        #endregion

        #region Event Handlers

        private async void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            int width = (int)Math.Max(10.0, ActualWidth);
            int height = (int)Math.Max(10.0, ActualHeight);

            await _glTakScheduler.Run(() =>
            {
                _renderTextureIsReady = _renderChart.CreateRenderTexture(width, height, Is3DEnabled);
            });
            _drawnBitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Pbgra32, BitmapPalettes.WebPalette);
            //_camera->Position = sf::Vector3f(1.0f, 1.0f, 1.0f);
            Grid.WindowRectangle = new Rect(0, 0, width, height);
            await Render();
            Draw();
        }


        private void OnGridRectangleChanged(object sender, EventArgs e)
        {
            UpdateTransform();
        }

        #endregion

        #region IDisposable 
        private bool _isDisposed = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        { 
            if (!_isDisposed)
            {
                if (disposing)
                {
                    // dispose managed state (managed objects).
                    _renderChart.Dispose();
                    _transformable.Dispose();
                    _camera.Dispose();
                    _markers.Dispose();
                    _grid.Dispose();
                }

                // free unmanaged resources (unmanaged objects) and override a finalizer below.
                // set large fields to null.

                _isDisposed = true;
            }
        }

        ~ChartElement()
        {
            Dispose(false);
        }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
             GC.SuppressFinalize(this);
        }
        #endregion
    }
}
