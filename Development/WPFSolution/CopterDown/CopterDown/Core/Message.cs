using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;

namespace CopterDown
{
    public class Message
    {

        public Message(ElementType cat, State traverse, int action, SenderType senderType, int ownerId, object data)
        {
            this.Category = cat;
            this.Data = data;
            this.Traverse = traverse;
            this.Action = action;
            this.OwnerId = ownerId;
            this.SenderType = senderType;
        }

        public Message(ElementType cat, int traverse, int action, SenderType senderType, int ownerId, object data)
            : this(cat, new State(traverse), action, senderType, ownerId, data)
        {
        }

        public Message(ElementType cat, int action, SenderType senderType, int ownerId, object data)
            : this(cat, new State(), action, senderType, ownerId, data)
        {
        }

        /// <summary>
        /// Message Type id, is registered to communicate with some special behaviours
        /// </summary>
        public int Action { get; set; }
        public ElementType Category { get; set; }
        public State Traverse { get; set; }
        public SenderType SenderType { get; set; }
        public object Data { get; set; }
        // message response
        public object Response { get; set; }
        public int OwnerId { get; protected set; }
    }

}
