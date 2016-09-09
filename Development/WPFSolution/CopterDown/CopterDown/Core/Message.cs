using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class Message
    {
        public Message(MessageCat cat, TraverseMode traverse, object data)
        {
            this.Category = cat;
            this.Data = data;
            this.Traverse = traverse;
        }

        public MessageCat Category { get; set; }
        public TraverseMode Traverse { get; set; }
        public object Data { get; set; }
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
