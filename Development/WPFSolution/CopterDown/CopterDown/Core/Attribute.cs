using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class Attribute<T>
    {
        public int Key { get; set; }
        public T Value { get; set; }
    }

    public class Attribute : Attribute<object>
    {
    }
}
