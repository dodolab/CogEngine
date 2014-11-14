using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Types;
using CopterDown.Enums;
using CopterDown.Game;
using CopterDown.Types;

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

        public UserAction KeysPressed
        {
            get { return root.FindAtt<UserAction>(Attr.USERACTION).Value; }
        } 

        public GameLoop(Dispatcher disp, Canvas canvas, int drawInterval, int updateInterval)
        {
            this._drawInterval = drawInterval;
            this._updateInterval = updateInterval;
            _canvas = canvas;
            this.root = new GameObject(ObjectType.ROOT, Subtypes.OTHER, "root");
            this.root.AddAttribute(ElementType.MODEL, Attr.USERACTION, new UserAction());

            this._disp = disp;

      /*      Rectangle rect = new Rectangle();
            rect.Fill = Brushes.Black;
            rect.Width = 100;
            rect.Height = 200;
            root.AddViewBehavior(new RenderBehavior(rect));
            root.AddModelBehavior(new TranslateAnim(new Vector2d(0,0),new Vector2d(200,0),0.1f,true));
            root.AddModelBehavior(new RotateAnim(0,180f,0.1f,true));

            root.AddAttribute(ElementType.MODELnew Attribute<Vector2d>(new Vector2d(0,0)),AttributeList.AT_COM_POSITION);
            root.AddAttribute(ElementType.MODELnew Attribute<float>(0), AttributeList.AT_COM_ROTATION);
            root.AddAttribute(ElementType.MODELnew Attribute<Vector2d>(new Vector2d((float)rect.Width/2,(float)rect.Height/2)), AttributeList.AT_COM_ORIGIN);
        
            root.AddModelBehavior(new MovementBehavior());

            var innerRect = new GameObject();
            rect = new Rectangle();
            rect.Fill = Brushes.Red;
            rect.Width = 50;
            rect.Height = 80;
            innerRect.AddViewBehavior(new RenderBehavior(rect));
            innerRect.AddModelBehavior(new TranslateAnim(new Vector2d(0, 0), new Vector2d(200, 0), 0.1f, true));
            innerRect.AddAttribute(ElementType.MODELnew Attribute<int>(1000),AttributeList.AT_COM_ZINDEX);

            root.AddChild(innerRect);
            */
            root.AddChild(new GameBuilder().CreateIntroScene());

        /*    var innerInnerRect = new GameObject();
            rect = new Rectangle();
            rect.Fill = Brushes.Green;
            rect.Width = 25;
            rect.Height = 40;
            innerInnerRect.AddViewBehavior(new RenderBehavior(rect));

            innerInnerRect.AddAttribute(ElementType.MODELnew Attribute<int>(2000), AttributeList.AT_COM_ZINDEX);

            innerRect.AddChild(innerInnerRect);*/
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
            try
            {
                _disp.Invoke(() =>
                {
                    var now = DateTime.Now;
                    _canvas.Children.Clear();
                    root.Draw(now - lastDraw, now - start);
                    lastDraw = DateTime.Now;
                });
            }
            catch
            {
                
            }
        }

        private void Update(object state)
        {
            lock (this)
            {
                var now = DateTime.Now;
                root.Update(now - lastUpdate, now - start);
                lastUpdate = DateTime.Now;
            }
        }

    }
}
