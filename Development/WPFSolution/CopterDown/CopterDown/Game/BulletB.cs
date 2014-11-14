using System;
using System.Linq;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Types;
using CopterDown.Enums;

namespace CopterDown.Game
{
    public class BulletB : ABehavior
    {
        public BulletB() : base(ElementType.MODEL, new State(Actions.COLISION_OCURRED)){}

        public override void OnMessage(Message msg)
        {
            if (msg.Action == Actions.COLISION_OCURRED)
            {
                var collision = msg.Data as Collision;
                if (collision.FirstId == GameObject.Id || collision.SecondId == GameObject.Id)
                {
                    var second =
                        GameObjectManager.Get.FindGameObjectById(collision.FirstId == GameObject.Id
                            ? collision.SecondId
                            : collision.FirstId);

                    var isHit = second.States.HasState(States.IS_HIT);

                    if (!isHit)
                    {
                        second.States.SetState(States.IS_HIT);
                        GameObject.Destroy();
                    }
                }
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var transform = GameObject.Transform;
            if (transform.LocalPos.X < 0 || transform.LocalPos.X > 640 || transform.LocalPos.Y < 0 || transform.LocalPos.Y > 340)
            {
                GameObject.Destroy();
            }
        }
    }
}
