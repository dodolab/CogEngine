using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using Attribute = System.Attribute;

namespace CopterDown.Behavior
{
    public abstract class ABehavior
    {
        public ABehavior()
        {
            Attributes = new List<Attribute>();
        }

        public List<Attribute> Attributes { get; set; }

        public abstract void OnMessage(Message msg);

        public abstract void Update(TimeSpan delta, TimeSpan absolute);
    }
}
