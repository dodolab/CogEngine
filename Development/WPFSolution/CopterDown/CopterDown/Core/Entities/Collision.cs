using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Entities
{
    public class Collision
    {
        public Collision(int firstId, int secondId)
        {
            this.FirstId = firstId;
            this.SecondId = secondId;
        }

        public int FirstId { get; set; }
        public int SecondId { get; set; }
    }
}
