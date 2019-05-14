using System;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows;
using System.Runtime.InteropServices;

namespace Xsa.Presentation.Graph
{

    public partial class ScatterChartControl : UserControl
    {
        #region --- Fields ---
        public ScatterChart Chart { get; private set; } = new ScatterChart();
        #endregion --- Fields ---


        public ScatterChartControl()
        {
            Content = Chart;
            Loaded += OnLoaded;
            Unloaded += OnUnloaded;
            Chart.NumberOfTraces = 2;
            Chart.Grid.NumberOfXAxisDivisions = 5;
            Chart.Grid.NumberOfYAxisDivisions = 5;
            Chart.Grid.NumberOfZAxisDivisions = 5;
            Chart.Grid.IsVisible = true;
            Chart.Enable3D = true;
            Chart.IsPolorCoordinate = false;
        }

        #region --- Event Handlers ---
        private void OnUnloaded(object sender, RoutedEventArgs e)
        {
            SizeChanged -= OnSizeChanged;
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            SizeChanged += OnSizeChanged;
            //CreateRenderTexture();
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            //CreateRenderTexture();
        }
        public void Draw()
        {
            Chart.Draw();
        }
        #endregion --- Event Handlers ---


    }
}
