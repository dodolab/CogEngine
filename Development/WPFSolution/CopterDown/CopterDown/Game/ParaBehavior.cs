using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Game
{
    public class ParaBehavior : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var isGrounded = GameObject.FindModelAtt<bool>(AT.AT_COPTER_PARA_ISGROUNDED);
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindModelAtt<bool>(AT.AT_COPTER_PARA_ISHIT);

            var hitFrame =  GameObject.FindViewAtt<int>(AT.AT_COPTER_HITFRAME);

            if (!isGrounded.Value && transform.LocalPos.Y > 264)
            {
                GameObject.GetSceneRoot().FindModelAtt<int>(AT.AT_COPTER_PLAYER_LIVES).Value--;
                isGrounded.Value = true;
            }
            else if (!isGrounded.Value) transform.LocalPos.Y += (float)Math.Sqrt(transform.LocalPos.Y / 50) * 0.5f;

            if (isHit.Value && hitFrame.Value++ > 10) GameObject.Destroy();

            if (isGrounded.Value)
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/soldier.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5,1,1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/para.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5,1,1);
            }

            if (isHit.Value) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion2.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5,1,1);
        }
    }
}
