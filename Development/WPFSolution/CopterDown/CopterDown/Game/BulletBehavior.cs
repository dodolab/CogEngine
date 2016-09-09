using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Game
{
    public class BulletBehavior : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {

            var transform = GameObject.GetTransform();
            var velocity = ((Attribute<Vector2d>)GameObject.FindModelAtt(AT.AT_COM_VELOCITY));

            var bulletSpeed = ((Attribute<float>)GameObject.GetParent().FindModelAtt(AT.AT_COPTER_BULLETSPEED));

            Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/bullet.png", transform.LocalPos.X, transform.LocalPos.Y, transform.Rotation,
    0.5f, 0.5f,6);

            transform.LocalPos.X += velocity.Value.X * bulletSpeed.Value;
            transform.LocalPos.Y += velocity.Value.Y * bulletSpeed.Value;

            if (transform.LocalPos.X < 0 || transform.LocalPos.X > 640 || transform.LocalPos.Y < 0 || transform.LocalPos.Y > 340)
            {
                GameObject.GetParent().RemoveChild(GameObject);
            }
            else
            {
                foreach (
                    var copter in
                        GameObject.GetParent().GetChildren().Where(chl => chl.GetObjectType() == ObjectType.COPTER))
                {
                    var isHit =
                        ((Attribute<bool>) copter.FindModelAtt(AT.AT_COPTER_PARA_ISHIT));

                    if (!isHit.Value)
                    {
                        var copterPos = copter.GetTransform();

                        if (transform.LocalPos.X >= copterPos.LocalPos.X &&
                            transform.LocalPos.X <= (copterPos.LocalPos.X + 115) &&
                            transform.LocalPos.Y >= copterPos.LocalPos.Y &&
                            transform.LocalPos.Y <= (copterPos.LocalPos.Y + 51))
                        {
                            ((Attribute<bool>)
                                copter.FindModelAtt(AT.AT_COPTER_PARA_ISHIT))
                                .Value = true;
                            ((Attribute<int>)
                                GameObject.GetParent().FindModelAtt(AT.AT_COPTER_PLAYER_SCORE))
                                .Value
                                += 100;
                            GameObject.GetParent().RemoveChild(GameObject);
                            return;
                        }
                    }
                }

                foreach (var shooter in GameObject.GetParent().GetChildren().Where(chl => chl.GetObjectType() == ObjectType.PARA))
                {
                    var isHit =
                        ((Attribute<bool>) shooter.FindModelAtt(AT.AT_COPTER_PARA_ISHIT));

                    if (!isHit.Value)
                    {

                        var shooterPos = shooter.GetTransform();

                        if (transform.LocalPos.X >= shooterPos.LocalPos.X && transform.LocalPos.X <= (shooterPos.LocalPos.X + 20) &&
                            transform.LocalPos.Y >= shooterPos.LocalPos.Y &&
                            transform.LocalPos.Y <= (shooterPos.LocalPos.Y + 20))
                        {
                            ((Attribute<bool>)
                                shooter.FindModelAtt(AT.AT_COPTER_PARA_ISHIT))
                                .Value = true;
                            ((Attribute<int>)
                                GameObject.GetParent().FindModelAtt(AT.AT_COPTER_PLAYER_SCORE))
                                .Value
                                += 20;
                            GameObject.GetParent().RemoveChild(GameObject);
                            return;
                        }
                    }
                }
            }
        }
    }
}
