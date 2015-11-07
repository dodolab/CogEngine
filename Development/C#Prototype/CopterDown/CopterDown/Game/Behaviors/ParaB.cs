using System;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;

using CopterDown.Enums;
using CopterDown.Game.Types;

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

            var hitFrame = GameObject.FindAtt<int>(GameAttr.HITFRAME);

            if (!isGrounded && transform.LocalPos.Y > 264)
            {
                GameObject.States.SetState(States.IS_GROUNDED);
                SendMessage(new State(Traverses.SCENEROOT, Traverses.CHILD_FIRST), GameActions.PARA_GROUNDED, GameObject);
            }
            else if (!isGrounded) transform.LocalPos.Y += (float)Math.Sqrt(transform.LocalPos.Y / 50) * 0.5f;

            if (isHit && hitFrame.Value++ > 10)
            {
                GameObject.States.ResetState(States.IS_HIT);
                hitFrame.Value = 0;

                var targetLives = GameObject.FindAtt<int>(GameAttr.PPLIVES);
                if (targetLives.Value == 0)
                {
                    SendMessage(new State(Traverses.SCENEROOT, Traverses.CHILD_FIRST), GameActions.GAMEOBJECT_KILLED, GameObject);
                    GameObject.Destroy();
                }
            }
        }
    }
}
