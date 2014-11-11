using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Game.Types;

namespace CopterDown.Core.Entities
{
    public class Bounds
    {
        public Bounds(float width, float height)
        {
            this.Width = width;
            this.Height = height;
        }

        public float Width { get; set; }
        public float Height { get; set; }

        public bool Collides(GameObject first, GameObject second)
        {
            var transformFirst = first.Transform;
            var transformSecond = second.Transform;
            var boundsFirst = first.FindAtt<Bounds>(Attr.BOUNDS);
            var boundsSecond = second.FindAtt<Bounds>(Attr.BOUNDS);

            if (transformFirst != null && transformSecond != null && boundsFirst != null && boundsSecond != null)
            {
                var xDiff = Math.Abs(transformFirst.LocalPos.X - transformSecond.LocalPos.X);
                var yDiff = Math.Abs(transformFirst.LocalPos.Y - transformSecond.LocalPos.Y);

                bool hasXIntersect = (transformFirst.LocalPos.X > transformSecond.LocalPos.X &&
                                      xDiff < boundsSecond.Value.Width) ||
                                     (transformFirst.LocalPos.X < transformSecond.LocalPos.X &&
                                      xDiff < boundsFirst.Value.Width);

                bool hasYIntersect = (transformFirst.LocalPos.Y > transformSecond.LocalPos.Y &&
                                      yDiff < boundsSecond.Value.Height) ||
                                     (transformFirst.LocalPos.Y < transformSecond.LocalPos.Y &&
                                      yDiff < boundsFirst.Value.Height);

                return hasXIntersect && hasYIntersect;
            }

            return false;
        }
    }
}
