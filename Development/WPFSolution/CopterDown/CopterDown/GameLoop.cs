using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Enums;
using CopterDown.Game;
using CopterDown.Game.Types;

namespace CopterDown
{
    public class GameLoop
    {
        private int _drawInterval;
        private int _updateInterval;
        private Thread _drawTimer;
        private Thread _updateTimer;
        public GameObject root;
        private Dispatcher _disp;

        public static Canvas _canvas;

        public UserAction UserActions
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


            root.AddChild(new GameBuilder().CreateIntroScene());
        }

        private DateTime start;
        private DateTime lastDraw;
        private DateTime lastUpdate;

        public void Start()
        {
            start = lastDraw = lastUpdate = DateTime.Now;
            _drawTimer = new Thread(Draw);   
            _updateTimer = new Thread(Update);
            _drawTimer.Start();
            _updateTimer.Start();

        }

        private void Draw(object state)
        {
            while (true)
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
                    Thread.Sleep(_drawInterval);
                }
                catch
                {

                }
            }
        }

        private void Update(object state)
        {
            while (true)
            {
                lock (this)
                {
                    var now = DateTime.Now;
                    root.Update(now - lastUpdate, now - start);
                    lastUpdate = DateTime.Now;

                    foreach (var act in UserActions.KeyActions.ToList())
                    {
                        if (act.Ended || act.Handled)
                        {
                            UserActions.KeyActions.Remove(act);
                        }
                        else act.CycleNumber++;

                    }

                    Thread.Sleep(_updateInterval);
                }
            }
        }

    }
}
