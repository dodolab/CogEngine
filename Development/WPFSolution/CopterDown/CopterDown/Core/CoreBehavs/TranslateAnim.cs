using System;
using System.Collections.Generic;
using System.Linq;
using CopterDown.Core.Entities;
using CopterDown.Core.Utils;
using CopterDown.Enums;

namespace CopterDown.Core.CoreBehavs
{
    public class TranslateAnim : ABehavior
    {
        private float speed;
        private float distX;
        private float distY;
        private bool additive;
        private Vector2d to;
        private Vector2d from;
        private bool repeat;

        // 0 to 1
        private Vector2d actual;

        public TranslateAnim(Vector2d from, Vector2d to, float speed, bool additive, bool repeat)
            : base(ElementType.MODEL, new State())
        {
            this.speed = speed;
            this.actual = new Vector2d(from);
            this.distX = to.X - from.X;
            this.distY = to.Y - from.Y;
            this.additive = additive;
            this.to = to;
            this.from = from;
            this.repeat = repeat;
        }

        public override void OnMessage(Message msg)
        {

        }

        private bool stopped = false;

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            if (stopped) return;

            float diffX = (float) (distX/1000*speed*delta.TotalMilliseconds);
            float diffY = (float) (distY/1000*speed*delta.TotalMilliseconds);

            if (VectorUt.DistSquare(from, to) < VectorUt.DistSquare(from, actual))
            {
                actual = new Vector2d(to.X, to.Y);
                if (repeat)
                {
                    var temp = from;
                    from = new Vector2d(to.X,to.Y);
                    to = new Vector2d(temp.X,temp.Y);
                    this.distX = to.X - from.X;
                    this.distY = to.Y - from.Y;
                }
                else
                {
                    stopped = true;
                }
            }

            actual.X += diffX;
            actual.Y += diffY;

            var transform = GameObject.Transform;

            if (additive)
            {
                transform.LocalPos.X += diffX;
                transform.LocalPos.Y += diffY;
            }
            else
            {
                transform.LocalPos = new Vector2d(actual.X, actual.Y);
            }
        }
    }
}
