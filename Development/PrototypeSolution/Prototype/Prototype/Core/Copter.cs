using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Prototype.Core
{
    public class Copter
    {
        public float PosX { get; set; }
        public float PosY { get; set; }
        public bool LeftDirection { get; set; }
        public int Frame { get; set; }
        public bool IsHit { get; set; }
        public int HitFrame { get; set; }

        public Copter(float posX, float posY, bool leftDirection)
        {
            this.PosX = posX;
            this.PosY = posY;
            this.LeftDirection = leftDirection;
        }
    }
}
