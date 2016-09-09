using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Messages;

namespace CopterDown.Core
{
    public abstract class ABehavior
    {
        private static int idCounter;

        protected ABehavior(ElementType elemType)
        {
            this.Active = true;
            this.Id = idCounter++;
            this.ElementType = elemType;
        }

        public ElementType ElementType { get; private set; }

        public int Id { get; protected set; }
        public bool Active { get; set; }

        public GameObject GameObject { get; set; }

        public abstract void OnMessage(Message msg);

        public abstract void Update(TimeSpan delta, TimeSpan absolute);
    }
}
