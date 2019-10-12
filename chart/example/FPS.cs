
using System.Diagnostics;

namespace ChartExample
{
    class FPS
    {
        private int _frame = 0;
        private int _fps = 0;
        private Stopwatch _sw = new Stopwatch();


        public void Start()
        {
            _sw.Start();
        }
        public void Update()
        {
            if(_sw.ElapsedMilliseconds >= 1000.0f)
            {
                _fps = _frame;
                _frame = 0;
                _sw.Restart();
            }
            ++_frame;
        }
        public override string ToString()
        {
            return _fps.ToString();
        }
    }
}
