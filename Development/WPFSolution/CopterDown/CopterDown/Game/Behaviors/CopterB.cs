using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game.Types;

namespace CopterDown.Game
{
    public class CopterB : ABehavior
    {
        public CopterB() : base(ElementType.MODEL, new State()){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var transform = GameObject.Transform;
            var isHit = GameObject.States.HasState(States.IS_HIT);
            var leftDirection = GameObject.States.HasState(States.LEFT_DIR);

            var copterSpeed = GameObject.SceneRoot.FindAtt<float>(Attr.COPTERSPEED);
            var hitFrame = GameObject.FindAtt<int>(Attr.HITFRAME);

            // update copter

            int next = rnd.Next(0, 60);
            transform.LocalPos.X += copterSpeed.Value * (leftDirection ? -1 : 1);

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

            if (transform.LocalPos.X < -111 || transform.LocalPos.X > 640)
            {
                if (leftDirection) GameObject.States.ResetState(States.LEFT_DIR);
                else GameObject.States.SetState(States.LEFT_DIR);
            }

            if (!isHit && next == 15 && (transform.LocalPos.X <= 280 || transform.LocalPos.X >= 340) && (transform.LocalPos.X > 150 || transform.LocalPos.X < 580))
            {
                GameObject para =
                    new GameBuilder().CreatePara(new Transform(transform.LocalPos.X + 55, transform.LocalPos.Y + 50));

                GameObject.Parent.AddChild(para);
            }
        }
    }
}
