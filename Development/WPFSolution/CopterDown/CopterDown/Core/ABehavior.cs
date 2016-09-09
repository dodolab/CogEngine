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
        private static int idCounter;

        protected ABehavior()
        {
            this.Active = true;
            this.Id = idCounter++;
        }

        public int Id { get; protected set; }
        public bool Active { get; set; }

        public GameObject GameObject { get; set; }


        public abstract void OnMessage(Message msg);

        public abstract void Update(TimeSpan delta, TimeSpan absolute);
    }
}
