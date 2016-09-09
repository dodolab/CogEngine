using System;
using System.Threading;
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
        public GameObject root;
        private Dispatcher _disp;

        public static Canvas _canvas;

        public GameLoop(Dispatcher disp, Canvas canvas, int drawInterval, int updateInterval)
        {
            this._drawInterval = drawInterval;
            this._updateInterval = updateInterval;
            _canvas = canvas;
            this.root = new GameObject(1);
            this._disp = disp;

            Rectangle rect = new Rectangle();
            rect.Fill = Brushes.Black;
            rect.Width = 100;
            rect.Height = 200;
            root.AddViewBehavior(new RenderBehavior(rect));
            root.AddModelBehavior(new TranslateAnim(new Vector2d(0,0),new Vector2d(200,0),0.1f,true));
            root.AddModelBehavior(new RotateAnim(0,180f,0.1f,true));

            root.AddModelAttribute(new SimpleValAttribute<Vector2d>(new Vector2d(0,0)),AttributeList.ATTR_POSITION);
            root.AddModelAttribute(new SimpleValAttribute<float>(0), AttributeList.ATTR_ROTATION);
            root.AddModelAttribute(new SimpleValAttribute<Vector2d>(new Vector2d((float)rect.Width/2,(float)rect.Height/2)), AttributeList.ATTR_ORIGIN);
        
            root.AddModelBehavior(new MovementBehavior());

            var innerRect = new GameObject(2);
            rect = new Rectangle();
            rect.Fill = Brushes.Red;
            rect.Width = 50;
            rect.Height = 80;
            innerRect.AddViewBehavior(new RenderBehavior(rect));
            innerRect.AddModelBehavior(new TranslateAnim(new Vector2d(0, 0), new Vector2d(200, 0), 0.1f, true));
            innerRect.AddModelAttribute(new SimpleValAttribute<int>(1000),AttributeList.ATTR_ZINDEX);

            root.AddChild(innerRect);



            var innerInnerRect = new GameObject(3);
            rect = new Rectangle();
            rect.Fill = Brushes.Green;
            rect.Width = 25;
            rect.Height = 40;
            innerInnerRect.AddViewBehavior(new RenderBehavior(rect));

            innerInnerRect.AddModelAttribute(new SimpleValAttribute<int>(2000), AttributeList.ATTR_ZINDEX);

            innerRect.AddChild(innerInnerRect);
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
