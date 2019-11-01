using System;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows;
using System.Runtime.InteropServices;

namespace Xsa.Presentation.Graph
{

    public partial class ScatterChartControl : UserControl, IDisposable 
    {
        #region --- Fields ---
        public ChartElement Chart { get; private set; } = new ChartElement();

        public readonly VertexArray[] DisplayTraces = {new VertexArray(), new VertexArray()};
        #endregion --- Fields ---


        public ScatterChartControl()
        {
            Content = Chart;
            Loaded += OnLoaded;
            Unloaded += OnUnloaded;
            Chart.Grid.NumberOfXAxisDivisions = 5;
            Chart.Grid.NumberOfYAxisDivisions = 5;
            Chart.Grid.NumberOfZAxisDivisions = 0;
            Chart.Grid.IsVisible = true;
            Chart.Is3DEnabled = false;
            Chart.IsPolorCoordinate = true;
            Chart.OnDrawTraces += Chart_OnDrawTraces;
        }

        private void Chart_OnDrawTraces(object sender, DrawTracesEventArgs e)
        {
            foreach (var displayTrace in DisplayTraces)
            {
                unsafe
                {
                   displayTrace.Draw(e.Target, e.States); 
                }
            }
        }

        #region --- Event Handlers ---
        private void OnUnloaded(object sender, RoutedEventArgs e)
        {
            SizeChanged -= OnSizeChanged;
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            SizeChanged += OnSizeChanged;
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
        }
        public async void Draw()
        {
            await Chart.Render();
            Chart.Draw();
        }

        public void Dispose()
        {
            Chart.Dispose();
        }
        #endregion --- Event Handlers ---


    }
}
