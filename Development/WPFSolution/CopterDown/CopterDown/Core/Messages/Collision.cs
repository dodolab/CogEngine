using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Messages
{
    public class Collision
    {
        public int FirstId { get; set; }
        public int SecondId { get; set; }
    }
}
