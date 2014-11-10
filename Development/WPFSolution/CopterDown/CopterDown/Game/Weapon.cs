using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Game
{
    public class Weapon
    {
        public Weapon(string name, int cadency, int penetration, float speed, string imgSource, bool isImmortal, int multiCount)
        {
            this.Name = name;
            this.Cadency = cadency;
            this.Penetration = penetration;
            this.Speed = speed;
            this.ImgSource = imgSource;
            this.IsImmortal = isImmortal;
            this.MultiCount = multiCount;
        }

        public int MultiCount { get; set; }
        public string ImgSource { get; set; }
        public string Name { get; set; }
        public int Cadency { get; set; }
        public int Penetration { get; set; }
        public float Speed { get; set; }
        public bool IsImmortal { get; set; }
    }
}
