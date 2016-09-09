using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class ParaB : ABehavior
    {
        public ParaB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var isGrounded = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISGROUNDED);
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISHIT);

            var hitFrame =  GameObject.FindAtt<int>(AT.AT_COPTER_HITFRAME);

            if (!isGrounded.Value && transform.LocalPos.Y > 264)
            {
                GameObject.GetSceneRoot().FindAtt<int>(AT.AT_COPTER_PLAYER_LIVES).Value--;
                isGrounded.Value = true;
            }
            else if (!isGrounded.Value) transform.LocalPos.Y += (float)Math.Sqrt(transform.LocalPos.Y / 50) * 0.5f;

            if (isHit.Value && hitFrame.Value++ > 10) GameObject.Destroy();
        }
    }
}
