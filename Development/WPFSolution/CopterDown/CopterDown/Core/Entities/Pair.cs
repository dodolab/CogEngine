using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Entities
{
    public class Pair<T>
    {
        public Pair(T item1, T item2)
        {
            this.Item1 = item1;
            this.Item2 = item2;
        }

        public T Item1 { get; set; }
        public T Item2 { get; set; }
    }
}
