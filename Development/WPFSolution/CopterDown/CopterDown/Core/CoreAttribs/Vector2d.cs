using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreAttribs
{
    public class Vector2d
    {
        public Vector2d(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }

        public Vector2d(Vector2d clone)
        {
            this.X = clone.X;
            this.Y = clone.Y;
        }

        public float X { get; set; }
        public float Y { get; set; }
    }
}
