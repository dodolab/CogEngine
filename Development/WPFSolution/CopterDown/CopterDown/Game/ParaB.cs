using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Core.Types;
using CopterDown.Enums;
using CopterDown.Types;

namespace CopterDown.Game
{
    public class ParaB : ABehavior
    {
        public ParaB() : base(ElementType.MODEL, new State()){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var isGrounded = GameObject.States.HasState(States.IS_GROUNDED);
            var transform = GameObject.Transform;
            var isHit = GameObject.States.HasState(States.IS_HIT);

            var hitFrame =  GameObject.FindAtt<int>(Attr.HITFRAME);

            if (!isGrounded && transform.LocalPos.Y > 264)
            {
                GameObject.States.SetState(States.IS_GROUNDED);
                SendMessage(new State(Traverses.SCENEROOT, Traverses.CHILD_FIRST), Actions.PARA_GROUNDED, GameObject);
            }
            else if (!isGrounded) transform.LocalPos.Y += (float)Math.Sqrt(transform.LocalPos.Y / 50) * 0.5f;

            if (isHit && hitFrame.Value++ > 10)
            {
                GameObject.States.ResetState(States.IS_HIT);
                hitFrame.Value = 0;

                var targetLives = GameObject.FindAtt<int>(Attr.PPLIVES);
                if (targetLives.Value == 0)
                {
                    SendMessage(new State(Traverses.SCENEROOT, Traverses.CHILD_FIRST), Actions.GAMEOBJECT_KILLED, GameObject);
                    GameObject.Destroy();
                }
            }
        }
    }
}
