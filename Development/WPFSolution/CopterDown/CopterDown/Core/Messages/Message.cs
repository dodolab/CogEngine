using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Messages
{
    public class Message
    {
        public Message(MessageCat cat, TraverseMode traverse, object data)
        {
            this.Category = cat;
            this.Data = data;
            this.Traverse = traverse;
        }

        public Message(MessageCat cat, TraverseMode traverse, MessageType type, object data)
        {
            this.Category = cat;
            this.Data = data;
            this.Traverse = traverse;
            this.Type = type;
        }

        /// <summary>
        /// Message Type id, is registered to communicate with some special behaviours
        /// </summary>
        public int TypeId { get; set; }
        public MessageType Type { get; set; }
        public MessageCat Category { get; set; }
        public TraverseMode Traverse { get; set; }
        public object Data { get; set; }
    }

    public enum MessageType
    {
        BEHAVIOR_CREATED,
        BEHAVIOR_REMOVED,
        OBJECT_CREATED,
        OBJECT_REMOVED,
        USER_INPUT,
        COLISION_OCURRED,
        GAMEOBJECT_DESTROYED
    }

    public enum MessageCat
    {
        VIEW,MODEL
    }

    public enum TraverseMode
    {
        NOTRAV, TRAV_CHILDFIRST, TRAV_BEHFIRST
    }
}
