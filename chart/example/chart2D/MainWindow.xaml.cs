using System;
using System.Windows;
using System.Windows.Threading;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Text.RegularExpressions;
using System.Windows.Media;
using System.Runtime.InteropServices;
using Xsa.Presentation.Graph;
using Color = System.Windows.Media.Color;

namespace ChartExample
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : System.Windows.Window
    {
        private readonly DispatcherTimer _timer;
        private FPS _fps;

        public double XAxisMin { get; set; } = -5.0;
        public double XAxisMax { get; set; } = 5.0;
        public double YAxisMin { get; set; } = -3.0;
        public double YAxisMax { get; set; } = 3.0;

        public MainWindow()
        {
            InitializeComponent();
            tbYAxisMin.Text = YAxisMin.ToString();
            tbYAxisMax.Text = YAxisMax.ToString();
            tbXAxisMin.Text = XAxisMin.ToString();
            tbXAxisMax.Text = XAxisMax.ToString();

            ScatterChart.Chart.YAxisMin = YAxisMin;
            ScatterChart.Chart.YAxisMax = YAxisMax;
            ScatterChart.Chart.XAxisMin = XAxisMin;
            ScatterChart.Chart.XAxisMax = XAxisMax;
            ScatterChart.Chart.ZAxisMin = 0;
            ScatterChart.Chart.ZAxisMax = 0;

            var refreshRate = new TimeSpan(0, 0, 0, 0, 1);
            _timer = new DispatcherTimer { Interval = refreshRate };
            _fps = new FPS();
            _fps.Start();
            _timer.Tick += Timer_Tick;
            _timer.Start();
        }


        private void Timer_Tick(object sender, EventArgs e)
        {
            _fps.Update();
            this.FpsLabel.Content = "FPS: " + _fps.ToString();
            ScatterChart.Draw();
        }

        private void GenerateData1(float[] data, float[] valueRangle)
        {
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

        private void ComboBox_Loaded(object sender, RoutedEventArgs e)
        {
            List<int> data = new List<int>();
            data.Add(1000);
            data.Add(10000);
            data.Add(100000);
            data.Add(1000000);
            data.Add(10000000);

            var comboBox = sender as ComboBox;
            comboBox.ItemsSource = data;
            comboBox.SelectedIndex = 0;
        }

        private void ComboBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            var comboBox = sender as ComboBox;

            int size = Convert.ToInt32(comboBox.SelectedItem);
            float[] data = new float[size * 2];
            float[] range1 = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            float[] range2 = new float[] { 1.5f, -1.5f };
            unsafe
            {
                fixed (float* pData = &data[0])
                {
                    GenerateData1(data, range1);
                    //ScatterChart.Chart.DataShapes[0].TraceColor = Colors.Green;
                    //ScatterChart.Chart.DataShapes[0].SetXYData(pData, data.Length);
                    SetData(ScatterChart.DisplayTraces[0], data, Colors.Blue);
                    GenerateData(data, range2);
                    //ScatterChart.Chart.DataShapes[1].TraceColor = Colors.Yellow;
                    //ScatterChart.Chart.DataShapes[1].SetXYData(pData, data.Length);
                    SetData(ScatterChart.DisplayTraces[1], data, Colors.Yellow);
                }
            }
        }
        private void SetData(VertexArray vArray, float[] data, Color color)
        {
            int pointSize = data.Length / 2;
            vArray.Type = PrimitiveType.Points;
            vArray.Size = data.Length / 2;
            GlColor glColor = new GlColor(color.R, color.G, color.B);
            unsafe
            {
                Vertex* vertexArray = vArray.AsPointer();
                for (int i = 0; i < pointSize; i++)
                {
                    vertexArray[i].Position.X = data[2 * i];
                    vertexArray[i].Position.Y = data[2 * i + 1];
                    vertexArray[i].Color = new GlColor(color.R, color.G, color.B);

                }
            }
        }
        private void NumberValidationTextBox(object sender, System.Windows.Input.TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9.-]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void YAxisMin_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbYAxisMin.Text);
                ScatterChart.Chart.YAxisMin = YAxisMin = value;
            }
        }

        private void YAxisMax_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbYAxisMax.Text);
                ScatterChart.Chart.YAxisMax = YAxisMax = value;
            }
        }

        private void XAxisMin_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbXAxisMin.Text);
                ScatterChart.Chart.XAxisMin = XAxisMin = value;
            }
        }

        private void XAxisMax_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbXAxisMax.Text);
                ScatterChart.Chart.XAxisMax = XAxisMax = value;
            }
        }
    }
}
