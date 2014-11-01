using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Prototype.Core
{
    public class Parashooter
    {
        public float PosX { get; set; }
        public float PosY { get; set; }
        public bool IsGrounded { get; set; }
        public bool IsHit { get; set; }
        public int HitFrame { get; set; }

        public Parashooter(float posX, float posY)
        {
            this.PosX = posX;
            this.PosY = posY;
        }
    }
}
