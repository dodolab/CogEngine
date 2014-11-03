using System;
using System.Linq;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.Messages;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class BulletB : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            if (msg.Type == MessageType.COLISION_OCURRED)
            {
                    var collision = msg.Data as Collision;
                if (collision.FirstId == GameObject.GetId() || collision.SecondId == GameObject.GetId())
                {
                    var second = GameObject.GetSceneRoot().GetChildren().FirstOrDefault(chld => chld.GetId() ==
                                                                                 (collision.FirstId ==
                                                                                  GameObject.GetId()
                                                                                     ? collision.SecondId
                                                                                     : collision.FirstId));

                    var isHit = second.FindModelAtt<bool>(AT.AT_COPTER_PARA_ISHIT);

                    if (isHit != null && !isHit.Value)
                    {
                        second.FindModelAtt<bool>(AT.AT_COPTER_PARA_ISHIT).Value = true;
                        GameObject.GetSceneRoot()
                            .SendMessage(new Message(MessageCat.MODEL, TraverseMode.NOTRAV,
                                MessageType.GAMEOBJECT_DESTROYED, second));
                        GameObject.Destroy();
                    }
                }
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {

            var transform = GameObject.GetTransform();
            var velocity = GameObject.FindModelAtt<Vector2d>(AT.AT_COM_VELOCITY);

            var bulletSpeed = GameObject.FindModelAtt<float>(AT.AT_COPTER_BULLETSPEED);


            transform.LocalPos.X += velocity.Value.X * bulletSpeed.Value;
            transform.LocalPos.Y += velocity.Value.Y * bulletSpeed.Value;

            if (transform.LocalPos.X < 0 || transform.LocalPos.X > 640 || transform.LocalPos.Y < 0 || transform.LocalPos.Y > 340)
            {
                GameObject.Destroy();
            }
        }
    }
}
