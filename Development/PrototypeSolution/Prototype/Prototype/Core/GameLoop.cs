using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Prototype.Core
{
    public class GameLoop
    {
        private int _drawInterval;
        private int _updateInterval;
        private Timer _drawTimer;
        private Timer _updateTimer;
        private Dispatcher _disp;
        private GameController controller;
     
        public static Canvas _canvas;


        public List<Key> KeysPressed { get { return controller.KeysPressed; } }

        public Point MousePosition { get; set; }

        public GameLoop(Dispatcher disp, Canvas canvas, int drawInterval, int updateInterval)
        {
            this._drawInterval = drawInterval;
            this._updateInterval = updateInterval;
            _canvas = canvas;
            this._disp = disp;
            controller = new GameController();
        }

        private DateTime start;
        private DateTime lastDraw;
        private DateTime lastUpdate;

        public void Start()
        {
            start = lastDraw = lastUpdate = DateTime.Now;
            _drawTimer = new Timer(Draw);
            _drawTimer.Change(0, _drawInterval);
            _updateTimer = new Timer(Update);
            _updateTimer.Change(0, _updateInterval);
        }

        private void Draw(object state)
        {
            
            _disp.Invoke(() =>
            {
                var now = DateTime.Now;
                _canvas.Children.Clear();
                controller.Draw(_canvas);
                lastDraw = DateTime.Now;
            });
        }

        private void Update(object state)
        {
            var now = DateTime.Now;
            controller.MousePosition = MousePosition;
            controller.Update(now - lastUpdate, now - start);
            lastUpdate = DateTime.Now;
        }
    }
}
