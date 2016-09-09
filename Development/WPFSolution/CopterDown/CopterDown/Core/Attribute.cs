using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using CopterDown.Core.Enums;
using CopterDown.Core.Types;
using CopterDown.Enums;

namespace CopterDown.Core
{
    public class Attribute
    {
        public Attribute(ElementType type,GameObject owner, int key)
        {
            this.Type = type;
            this.Key = key;
            this.Owner = owner;
        }

        public GameObject Owner { get; protected set; }
        public ElementType Type { get; protected set; }
        public int Key { get; protected set; }
    }

    public class Attribute<T> : Attribute
    {
        protected T _value;

        public Attribute(ElementType type, GameObject owner, int key, T val) : base(type, owner, key)
        {
            _value = val;
        }

        public T Value
        {
            get
            {
                return _value;
            }
            set
            {
                T temp = _value;
                _value = value;
                OnAttributeChanged(temp, _value);
            }
        }

        protected void OnAttributeChanged(T oldAttr, T newAttr)
        {
            Owner.SendMessage(new Message(ElementType.ALL, Traverses.BEH_FIRST, Actions.ATTRIBUTE_CHANGED,
                SenderType.ATTRIBUTE, Key, new Tuple<T,T>(oldAttr,newAttr)));
        }
    }
}
