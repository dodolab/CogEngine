using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class ParaDrawB : ABehavior
    {
        public ParaDrawB() : base(ElementType.VIEW){}
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var isGrounded = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISGROUNDED);
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISHIT);

            if (isGrounded.Value)
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/soldier.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5, 1, 1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/para.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5, 1, 1);
            }

            if (isHit.Value) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion2.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5, 1, 1);
        }
    }
}
