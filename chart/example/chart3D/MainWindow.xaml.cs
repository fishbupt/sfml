using System;
using System.Windows;
using System.Windows.Threading;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Text.RegularExpressions;
using System.Windows.Media;
using System.Runtime.InteropServices;
using Xsa.Presentation.Graph;

namespace ChartExample
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : System.Windows.Window
    {
        private readonly DispatcherTimer _timer;
        private FPS _fps;
        private Point _mouseStartPos;

        public double XAxisMin { get; set; } = -5.0;
        public double XAxisMax { get; set; } = 5.0;
        public double YAxisMin { get; set; } = -3.0;
        public double YAxisMax { get; set; } = 3.0;
        public double ZAxisMin { get; set; } = -3.0;
        public double ZAxisMax { get; set; } = 3.0;

        public MainWindow()
        {
            InitializeComponent();
            tbYAxisMin.Text = YAxisMin.ToString();
            tbYAxisMax.Text = YAxisMax.ToString();
            tbXAxisMin.Text = XAxisMin.ToString();
            tbXAxisMax.Text = XAxisMax.ToString();
            tbZAxisMin.Text = ZAxisMin.ToString();
            tbZAxisMax.Text = ZAxisMax.ToString();

            ScatterChart.Chart.YAxisMin = YAxisMin;
            ScatterChart.Chart.YAxisMax = YAxisMax;
            ScatterChart.Chart.XAxisMin = XAxisMin;
            ScatterChart.Chart.XAxisMax = XAxisMax;
            ScatterChart.Chart.ZAxisMin = ZAxisMin;
            ScatterChart.Chart.ZAxisMax = ZAxisMax;

            var refreshRate = new TimeSpan(0, 0, 0, 0, 1);
            _timer = new DispatcherTimer { Interval = refreshRate };
            _fps = new FPS();
            _fps.Start();
            _timer.Tick += Timer_Tick;
            _timer.Start();
            Reset_Click(null, null);
        }


        private void Timer_Tick(object sender, EventArgs e)
        {
            _fps.Update();
            this.FpsLabel.Content = "FPS: " + _fps.ToString();
            ScatterChart.Draw();
        }

        private void GenerateData(float[] data, float[] valueRangle, float zValue = 0.0f)
        {
            //float[] range = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            int len = data.Length / 3;
            int rangeLen = valueRangle.Length;
            var rand = new Random();
            for (int i = 0; i < len; i++)
            {
                float x = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                float y = valueRangle[rand.Next(0, rangeLen)] + ((float)rand.NextDouble() - 0.5f) * 0.1f;
                data[3 * i] = x;
                data[3 * i + 1] = y;
                data[3 * i + 2] = zValue;
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
            float[] data = new float[size * 3];
            float[] range1 = new float[] { 1.0f, 2.0f, -1.0f, -2.0f };
            float[] range2 = new float[] { 1.5f, -1.5f };
            unsafe
            {
                fixed (float* pData = &data[0])
                {
                    GenerateData(data, range1, -2.0f);
                    ScatterChart.Chart.DataShapes[0].TraceColor = Colors.Green;
                    ScatterChart.Chart.DataShapes[0].SetXYZData(pData, data.Length);
                    GenerateData(data, range2, 2.0f);
                    ScatterChart.Chart.DataShapes[1].TraceColor = Colors.Yellow;
                    ScatterChart.Chart.DataShapes[1].SetXYZData(pData, data.Length);
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
        private void ZAxisMin_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbZAxisMin.Text);
                ScatterChart.Chart.ZAxisMin = ZAxisMin = value;
            }
        }

        private void ZAxisMax_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                float value = Convert.ToSingle(tbZAxisMax.Text);
                ScatterChart.Chart.ZAxisMax = ZAxisMax = value;
            }
        }

        private void Distance_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ScatterChart.Chart.Camera.Distance = (float)distanceSlider.Value;
        }
        private void Azimuth_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ScatterChart.Chart.Camera.Azimuth = (float)azimuthSlider.Value;
        }
        private void Elevation_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ScatterChart.Chart.Camera.Elevation = (float)elevationSlider.Value;
        }

        private void Reset_Click(object sender, RoutedEventArgs e)
        {
            azimuthSlider.Value = 0.0f;
            elevationSlider.Value = 0.0f;
            distanceSlider.Value = 5.0f;
            ScatterChart.Chart.Camera.Azimuth = 0.0f;
            ScatterChart.Chart.Camera.Elevation = 0.0f;
            ScatterChart.Chart.Camera.Distance = 5.0f;
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            ScatterChart.Chart.Camera.UseOrthographicCamera();
        }

        private void CheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            ScatterChart.Chart.Camera.UsePerspectiveCamera();
        }

        private void ScatterChart_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            ScatterChart.Chart.Camera.ChangeScale(1.0f + e.Delta / 240.0f);
        }

        private void ScatterChart_PreviewMouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            _mouseStartPos = e.GetPosition(this);
        }

        private void ScatterChart_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if(e.LeftButton== System.Windows.Input.MouseButtonState.Pressed)
            {
                float speed = 10.0f;
                Point currentPos = e.GetPosition(this);
                Vector move = currentPos - _mouseStartPos;
                double width = ScatterChart.RenderSize.Width;
                double height = ScatterChart.RenderSize.Height;
                ScatterChart.Chart.Camera.ChangeAzimuth((float)(move.X / width * speed));
                ScatterChart.Chart.Camera.ChangleElevation((float)(move.Y / height * speed));
                azimuthSlider.Value = ScatterChart.Chart.Camera.Azimuth;
                elevationSlider.Value = ScatterChart.Chart.Camera.Elevation;
            }
        }
    }
}
