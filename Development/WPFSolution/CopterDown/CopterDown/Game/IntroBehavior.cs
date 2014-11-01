using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CopterDown.Core;

namespace CopterDown.Game
{
    public class IntroBehavior : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var keys = (Attribute<List<Key>>)GameObject.GetParent().FindModelAtt(AT.AT_COPTER_KEYINPUT);

            if (keys.Value.Any())
            {
                var root = GameObject.GetParent();
                root.RemoveChild(GameObject);
                root.AddChild(new GameBuilder().CreateGameScene());
            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/intro.png", 0, 0, 0, 0, 0,1);
            }

        }
    }
}
