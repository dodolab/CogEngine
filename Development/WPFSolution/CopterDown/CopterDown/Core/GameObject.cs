using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using CopterDown.Behavior;

namespace CopterDown.Core
{
    public class GameObject
    {
        private Canvas _canvas;

        public GameObject(Canvas canvas)
        {
            this._canvas = canvas;
            Attributes = new List<Attribute>();
            Behaviors = new List<ABehavior>();
        }

        public void OnMessage(Message msg)
        {
            foreach(var beh in Behaviors) beh.OnMessage(msg);
        }

        public void Update(TimeSpan delta, TimeSpan absolute)
        {
            foreach (var beh in Behaviors) beh.Update(delta,absolute);
        }

        public List<Attribute> Attributes { get; set; }
 
        public List<ABehavior> Behaviors { get; set; } 
    }
}
