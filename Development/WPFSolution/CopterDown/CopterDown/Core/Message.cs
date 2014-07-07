using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class Message
    {
        public Message(MessageType type, object data)
        {
            this.MessageType = type;
            this.Data = data;
        }

        public MessageType MessageType { get; set; }
        public object Data { get; set; }
    }

    public enum MessageType
    {
        RENDER
    }
}
