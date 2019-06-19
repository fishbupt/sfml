using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Toolchains.CsProj;
using System;
using System.Security.Cryptography;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using Xsa.Presentation.Graph;
using System.Windows.Threading;
using System.Windows.Media;

namespace ChartBenchmark
{
    public class Program
    {
        public static Application app;
        static void Main(string[] args)
        {
#if DEBUG
            var test = new BenchmarkTests<MainWindow3DChart>();
            test.Setup();
            Task.Delay(5000).Wait();
            test.Cleanup();
#else
            var summary2D = BenchmarkRunner.Run<BenchmarkTests<MainWindow2DChart>>();
            var summary3D = BenchmarkRunner.Run<BenchmarkTests<MainWindow3DChart>>();

#endif
        }

        [MarkdownExporter, AsciiDocExporter, HtmlExporter, CsvExporter]
        public class BenchmarkTests<ChartType> where ChartType: MainWindowChart, new()
        {
            System.Windows.Application app;
            ChartType mainWindow;

            [Params(1000, 10000, 100000, 1000000, 5000000)]
            public int DataSize;


            [GlobalSetup]
            public void Setup()
            {
                app = UIInitializer<ChartType>.CreateUIApplication(1024, 768);
                app.Dispatcher.Invoke(() =>
                {
                    mainWindow = app.MainWindow as ChartType;
#if DEBUG
                    mainWindow.DataSize = 1000;
#else
                    mainWindow.DataSize = DataSize; // GlobalSetup will be executed once per each DataSize value
#endif
                    mainWindow.PrepareData();
                });
            }

            [GlobalCleanup]
            public void Cleanup()
            {
                app.Dispatcher.Invoke(() =>
                {
                    app.Shutdown();
                    app = null;
                });
            }


            [Benchmark]
            public void Draw()
            {
                mainWindow.Dispatcher.Invoke(() => mainWindow.Draw());
            }

            [Benchmark]
            public void UpdateTraces()
            {
                mainWindow.Dispatcher.Invoke(() => mainWindow.UpdateTraces());
            }
        }
    }

    public abstract class MainWindowChart : System.Windows.Window
    {

        public abstract void SetData();

        public abstract void PrepareData();

        public void Draw() => _chart.Draw();

        public void UpdateTraces()
        {
            SetData();
            Draw();
        }

        public int DataSize { get; set; }

        public MainWindowChart()
        {
            Loaded += OnLoaded;
            Closing += OnClosing;
        }


        private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _chart.Dispose();
        }

        private void OnLoaded(object sender, RoutedEventArgs args)
        {
            Content = _chart;

#if DEBUG
            var refreshRate = new TimeSpan(0, 0, 0, 0, 1);
            _timer.Interval = refreshRate;
            _timer.Tick += (s, e) => Draw();
            _timer.Start();
#endif
        }

        protected ScatterChart _chart = new ScatterChart();
        private readonly DispatcherTimer _timer = new DispatcherTimer();
    }


    public class MainWindow2DChart : MainWindowChart
    {
        public MainWindow2DChart() : base()
        {
            _chart.NumberOfTraces = 2;
            _chart.Grid.NumberOfXAxisDivisions = 10;
            _chart.Grid.NumberOfYAxisDivisions = 10;
            _chart.Grid.IsVisible = true;
            _chart.YAxisMin = -3.0;
            _chart.YAxisMax = 3.0;
            _chart.XAxisMin = -5.0;
            _chart.XAxisMax = 5.0;
        }

        public override void SetData()
        {
            unsafe
            {
                fixed (float* pData = &_data[0])
                {
                    _chart.DataShapes[0].TraceColor = Colors.Green;
                    _chart.DataShapes[0].SetXYData(pData, _data.Length);
                }
            }
        }
        public override void PrepareData()
        {
            _data = new float[DataSize * 2];
            float[] range = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            GenerateData(_data, range);
            SetData();
        }

        private void GenerateData(float[] data, float[] valueRangle)
        {
            //float[] range = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            int len = data.Length / 2;
            int rangeLen = valueRangle.Length;
            var rand = new Random();
            for (int i = 0; i < len; i++)
            {
                float x = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                float y = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                data[2 * i] = x;
                data[2 * i + 1] = y;
            }
        }
        private float[] _data;

    }

    public class MainWindow3DChart : MainWindowChart
    {
        public MainWindow3DChart() : base()
        {
            _chart.NumberOfTraces = 2;
            _chart.Grid.NumberOfXAxisDivisions = 5;
            _chart.Grid.NumberOfYAxisDivisions = 5;
            _chart.Grid.NumberOfZAxisDivisions = 5;
            _chart.IsPolorCoordinate = false;
            _chart.Is3DEnabled = true;
            _chart.Grid.IsVisible = true;
            _chart.YAxisMin = -3.0;
            _chart.YAxisMax = 3.0;
            _chart.XAxisMin = -5.0;
            _chart.XAxisMax = 5.0;
            _chart.ZAxisMin = -3.0;
            _chart.ZAxisMax = 3.0;

            _chart.Camera.Azimuth = -37.5f;
            _chart.Camera.Elevation = 30.0f;
            _chart.Camera.UseOrthographicCamera();
        }

        public override void SetData()
        {
            unsafe
            {
                fixed (float* pData = &_data[0])
                {
                    _chart.DataShapes[0].TraceColor = Colors.Green;
                    _chart.DataShapes[0].SetXYZData(pData, _data.Length);

                }
            }

        }

        public override void PrepareData()
        {
            _data = new float[DataSize * 3];
            float[] range = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            GenerateData(_data, range, 1.0f);
            SetData();

        }
        private void GenerateData(float[] data, float[] valueRangle, float zValue = 0.0f)
        {
            int len = data.Length / 3;
            int rangeLen = valueRangle.Length;
            var rand = new Random();
            for (int i = 0; i < len; i++)
            {
                float x = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                float y = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                data[3 * i] = x;
                data[3 * i + 1] = y;
                data[3 * i + 2] = zValue + ((float)rand.NextDouble() - 0.5f) * 0.1f;
            }
        }

        private float[] _data;
    }
}
