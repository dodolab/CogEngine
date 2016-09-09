using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class Attribute
    {
        public Attribute(ElementType type)
        {
            this.Type = type;
        }

        public ElementType Type { get; protected set; }
        public int Key { get; set; }
    }

    public class Attribute<T> : Attribute
    {
        public Attribute(ElementType type, T val) : base(type)
        {
            this.Value = val;
        }

        public T Value { get; set; }
    }
}
