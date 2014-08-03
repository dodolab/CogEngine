using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Core.Utils
{
    public class VectorUt
    {
        public static Vector2d Diff(Vector2d a, Vector2d b)
        {
            return new Vector2d(a.X-b.X,a.Y-b.Y);
        }

        public static float DistSquare(Vector2d a, Vector2d b)
        {
            float dist = (a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y);

            return dist < 0 ? -dist : dist;
        }

        public static float Dist(Vector2d a, Vector2d b)
        {
            return (float)Math.Sqrt(DistSquare(a, b));
        }
    }
}
