using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CopterDown.Core;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class IntroB : ABehavior
    {
        public IntroB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var keys = GameObject.GetRoot().FindAtt<List<Key>>(AT.AT_COPTER_KEYINPUT);

            if (keys.Value.Any())
            {
                var root = GameObject.GetRoot();
                GameObject.Destroy();
                root.AddChild(new GameBuilder().CreateGameScene());
            }
        }
    }
}
