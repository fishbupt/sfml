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
            var test = new BenchmarkTests<MainWindow2DChart>();
            test.Setup();
            test.Draw();
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

            [Params(1000, 10000, 100000, 1000000, 7096320)]
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
                    mainWindow.Close();
                    app.Shutdown();
                    app = null;
                });
            }


            [Benchmark]
            public void Draw()
            {
                mainWindow.Dispatcher.Invoke(() =>
                {
                    mainWindow.Draw();
                });
            }

            [Benchmark]
            public void UpdateTraces()
            {
                mainWindow.Dispatcher.Invoke(() =>
                {
                    mainWindow.UpdateTraces();
                });
            }

            //[Benchmark]
            //public void Draw()
            //{
            //    mainWindow.Dispatcher.Invoke(() =>
            //    {
            //        mainWindow.Draw().Wait();
            //    });
            //}
        }
    }

    public abstract class MainWindowChart : System.Windows.Window
    {

        public abstract void SetData();

        public abstract void PrepareData();

        public void Draw()
        {
            _chart.Render().Wait(); 
            _chart.Draw();
        }

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

        }

        protected ChartElement _chart = new ChartElement();
    }


    public class MainWindow2DChart : MainWindowChart
    {
        public readonly VertexArray DisplayTrace = new VertexArray();
        public MainWindow2DChart() : base()
        {
            _chart.Grid.NumberOfXAxisDivisions = 10;
            _chart.Grid.NumberOfYAxisDivisions = 10;
            _chart.Grid.IsVisible = true;
            _chart.YAxisMin = -3.0;
            _chart.YAxisMax = 3.0;
            _chart.XAxisMin = -5.0;
            _chart.XAxisMax = 5.0;
            _chart.OnDrawTraces += Chart_OnDrawTraces;
        }

        private void Chart_OnDrawTraces(object sender, DrawTracesEventArgs e)
        {
            if(DisplayTrace.IsVisible)
            {
                unsafe
                {
                    DisplayTrace.Draw(e.Target, e.States);
                }
            }
        }

        public override void SetData()
        {
            int pointSize = _data.Length / 2;
            DisplayTrace.Type = PrimitiveType.Points;
            DisplayTrace.Size = _data.Length / 2;
            Color mediaColor = Colors.Green;
            GlColor color = new GlColor(mediaColor.R, mediaColor.G, mediaColor.B);
            unsafe
            {
                Vertex* vPointer = DisplayTrace.AsPointer();
                for (int i = 0; i < pointSize; i++)
                {
                    ref Vertex vertex = ref vPointer[i];
                    vertex.Position.X = _data[2 * i];
                    vertex.Position.Y = _data[2 * i + 1];
                    vertex.Color = color;
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
        public readonly VertexArray DisplayTrace = new VertexArray();
        public MainWindow3DChart() : base()
        {
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
            _chart.OnDrawTraces += Chart_OnDrawTraces;
        }

        private void Chart_OnDrawTraces(object sender, DrawTracesEventArgs e)
        {
            if(DisplayTrace.IsVisible)
            {
                unsafe
                {
                    DisplayTrace.Draw(e.Target, e.States);
                }
            }
        }

        public override void SetData()
        {
            int pointSize = _data.Length / 3;
            DisplayTrace.Type = PrimitiveType.Points;
            DisplayTrace.Size = _data.Length / 3;
            Color mediaColor = Colors.Red;
            GlColor color = new GlColor(mediaColor.R, mediaColor.G, mediaColor.B);
            unsafe
            {
                Vertex* vPointer = DisplayTrace.AsPointer();
                for (int i = 0; i < pointSize; i++)
                {
                    ref Vertex vertex = ref vPointer[i];
                    vertex.Position.X = _data[3 * i];
                    vertex.Position.Y = _data[3 * i + 1];
                    vertex.Position.Z = _data[3 * i + 2];
                    vertex.Color = color;
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
