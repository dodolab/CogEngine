using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;
using CopterDown.Behavior;
using CopterDown.Core;

namespace CopterDown
{
    public class GameLoop
    {
        private int _drawInterval;
        private int _updateInterval;
        private Timer _drawTimer;
        private Timer _updateTimer;
        private Canvas _canvas;
        private GameObject root;
        private Dispatcher _disp;

        public GameLoop(Dispatcher disp, Canvas canvas, int drawInterval, int updateInterval)
        {
            this._drawInterval = drawInterval;
            this._updateInterval = updateInterval;
            this._canvas = canvas;
            this.root = new GameObject(null);
            this._disp = disp;

            Rectangle rect = new Rectangle();
            rect.Fill = Brushes.Black;
            rect.Width = 100;
            rect.Height = 200;
            root.Behaviors.Add(new RenderBehavior(rect));
        }

        public void Start()
        {
            _drawTimer = new Timer(Draw);
            _drawTimer.Change(0, _drawInterval);
            _updateTimer = new Timer(Update);
            _updateTimer.Change(0, _updateInterval);
        }

        private void Draw(object state)
        {
            _disp.Invoke(() =>
            {

                _canvas.Children.Clear();
                root.OnMessage(new Message(MessageType.RENDER, _canvas));
            });
        }

        private void Update(object state)
        {
            
        }
       
    }
}
