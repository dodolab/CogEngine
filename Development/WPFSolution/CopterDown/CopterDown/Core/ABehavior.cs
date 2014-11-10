using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;

namespace CopterDown.Core
{
    public abstract class ABehavior
    {
        private static int idCounter;

        protected Random rnd = new Random();

        protected ABehavior(ElementType elemType, State messageListeners)
        {
            this.Active = true;
            this.Id = idCounter++;
            this.ElemType = elemType;
            this.MessageListeners = messageListeners;
        }

        public ElementType ElemType { get; private set; }

        public int Id { get; protected set; }
        public bool Active { get; set; }
        public State MessageListeners { get; protected set; }

        public GameObject GameObject { get; set; }

        public abstract void OnMessage(Message msg);

        public abstract void Update(TimeSpan delta, TimeSpan absolute);

        protected Message SendMessage(State traverse, int action, object data)
        {
            var msg = new Message(ElemType, traverse, action, SenderType.BEHAVIOR, Id, data);
            GameObject.SendMessage(msg);
            return msg;
        }

        protected Message SendMessage(int traverse, int action, object data)
        {
            var msg = new Message(ElemType, traverse, action, SenderType.BEHAVIOR, Id, data);
            GameObject.SendMessage(msg);
            return msg;
        }

    }
}
