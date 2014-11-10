using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Entities
{
    public class InputAct<T>
    {
        public InputAct(T actValue)
        {
            this.ActValue = actValue;
        }

        public T ActValue { get; set; }
        public bool Handled { get; set; }
        public bool Ended { get; set; }
        public int CycleNumber { get; set; }
    }
}
