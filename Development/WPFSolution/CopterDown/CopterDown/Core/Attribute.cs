using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class Attribute
    {
        public int Key { get; set; }
    }

    public class Attribute<T> : Attribute
    {
        public Attribute(T val)
    {
        this.Value = val;
    } 

        public T Value { get; set; }
    }
}
