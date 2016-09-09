using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;

namespace CopterDown.Core
{
    public abstract class ABehavior
    {
        protected ABehavior()
        {
            this.Active = true;
        }

        public bool Active { get; set; }

        public GameObject GameObject { get; set; }

        public abstract void OnMessage(Message msg);

        public abstract void Update(TimeSpan delta, TimeSpan absolute);
    }
}
