using System;
using CopterDown.Core.Entities;
using CopterDown.Enums;

namespace CopterDown.Core.CoreBehavs
{
    public class RotateAnim : ABehavior
    {
        private float from;
        private float to;
        private float speed;
        private bool additive;

        // 0 to 1
        private float actual;

        public RotateAnim(float from, float to, float speed, bool additive)
            : base(ElementType.MODEL, new State())
        {
            this.from = from;
            this.to = to;
            this.speed = speed;
            this.actual = from;
            this.additive = additive;
        }

        public override void OnMessage(Message msg)
        {
            
        }

        private bool stopped = false;

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            if (stopped) return;
            float diff = (float) ((to == from ? 1 : (to - from))/1000*speed*delta.TotalMilliseconds);
            actual += diff;
            // if to == from, it is infinite rotation
            if (to != from && ((to >= from && actual >= to) || (to < from && actual < to)))
            {
                actual = to;
                stopped = true;
            }

            var transform = GameObject.Transform;

            if (additive) transform.Rotation += diff;  
            else transform.Rotation = actual; 
        }
    }
}
