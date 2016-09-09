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
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.CoreBehavs;

namespace CopterDown
{
    public class GameLoop
    {
        private int _drawInterval;
        private int _updateInterval;
        private Timer _drawTimer;
        private Timer _updateTimer;
        private GameObject root;
        private Dispatcher _disp;

        public static Canvas _canvas;

        public GameLoop(Dispatcher disp, Canvas canvas, int drawInterval, int updateInterval)
        {
            this._drawInterval = drawInterval;
            this._updateInterval = updateInterval;
            _canvas = canvas;
            this.root = new GameObject(null,1);
            this._disp = disp;

            Rectangle rect = new Rectangle();
            rect.Fill = Brushes.Black;
            rect.Width = 100;
            rect.Height = 200;
            root.AddViewBehavior(new RenderBehavior(rect));
            root.AddModelBehavior(new TranslateAnim(new Vector2d(0,0),new Vector2d(100,0),1));

            root.AddModelAttribute(new SimpleValAttribute<Vector2d>(new Vector2d(0,0)),AttributeList.ATTR_POSITION);
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
                root.Draw(now - lastDraw, now - start);
                lastDraw = DateTime.Now;
            });
        }

        private void Update(object state)
        {
            var now = DateTime.Now;
            root.Update(now - lastUpdate, now - start);
            lastUpdate = DateTime.Now;
        }
       
    }
}
