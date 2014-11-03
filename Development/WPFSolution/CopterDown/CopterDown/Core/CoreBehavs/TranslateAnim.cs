using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.Utils;
using CopterDown.Messages;

namespace CopterDown.Core.CoreBehavs
{
    public class TranslateAnim : ABehavior
    {
        private Vector2d from;
        private Vector2d to;
        private float speed;
        private float distX;
        private float distY;
        private bool removeAfterFinish;

        // 0 to 1
        private Vector2d actual;

        public TranslateAnim(Vector2d from, Vector2d to, float speed, bool removeAfterFinish)
        {
            this.from = from;
            this.to = to;
            this.speed = speed;
            this.actual = new Vector2d(from);
            this.distX = to.X - from.X;
            this.distY = to.Y - from.Y;
            this.removeAfterFinish = removeAfterFinish;
        }

        public override void OnMessage(Message msg)
        {
            
        }


        public override void Update(TimeSpan delta, TimeSpan absolute)
        {/*
            if (Active)
            {
                // toto je diskutabilni
                if (VectorUt.DistSquare(from, to) < VectorUt.DistSquare(from, actual))
                {
                    if (removeAfterFinish)
                    {
                        GameObject.RemoveModelBehavior(this);
                    }
                    Active = false;
                    return;
                }

                float diffX = (float) (distX/1000*speed*delta.TotalMilliseconds);
                float diffY = (float) (distY/1000*speed*delta.TotalMilliseconds);


                actual.X += diffX;
                actual.Y += diffY;

                var position =
                    GetPosition(GameObject);

                position.Value.X += diffX;
                position.Value.Y += diffY;
            }*/
        }
    }
}
