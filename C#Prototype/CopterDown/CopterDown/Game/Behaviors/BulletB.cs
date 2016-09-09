using System;
using System.Linq;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game.Types;

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

                    var actualWeapon =
                        GameObjectManager.Get.FindGameObjectBySubType(Subtypes.CANON).FindAttValue<Weapon>(GameAttr.WEAPON);

                    var targetArmor = second.FindAttValue<int>(GameAttr.PPARMOR);
                    var targetLives = second.FindAtt<int>(GameAttr.PPLIVES);

                    var destruction = actualWeapon.Penetration/ ((float)targetArmor) * 100;
                    targetLives.Value = (int)Math.Max(0, targetLives.Value-destruction);

                    var isHit = second.States.HasState(States.IS_HIT);

                    if (!isHit)
                    {
                        second.States.SetState(States.IS_HIT);
                        if(!GameObject.States.HasState(States.IS_IMMORTAL)) GameObject.Destroy();
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
