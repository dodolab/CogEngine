using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.CoreBehavs;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class CanonB : ABehavior
    {
        public CanonB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var keys = GameObject.GetRoot().FindAtt<List<Key>>(AT.AT_COPTER_KEYINPUT).Value;

            var lastShot = GameObject.FindAtt <DateTime>(AT.AT_COPTER_CANON_LASTSHOT);
            var actualCadency = GameObject.FindAtt<float>(AT.AT_COPTER_CANON_CADENCY);
            var transform = GameObject.GetTransform();

            if (keys.Contains(Key.Left))
            {
                if (transform.Rotation > -75) transform.Rotation -= (float)delta.TotalSeconds * 60;
            }
            if (keys.Contains(Key.Right))
            {
                if (transform.Rotation < 75) transform.Rotation += (float)delta.TotalSeconds * 60;
            }
            if (keys.Contains(Key.Space))
            {
                if ((DateTime.Now - lastShot.Value).TotalSeconds > 1.0 / actualCadency.Value)
                {
                    Shoot(transform.Rotation);
                    lastShot.Value = DateTime.Now;
                }
            }
        }

        private void Shoot(float rotation)
        {
            float canonLength = 37;
            float posX = (float)(318 + 2 + canonLength * Math.Sin(rotation / 180 * Math.PI));
            float posY = (float)(223 + (canonLength - canonLength * Math.Cos(rotation / 180 * Math.PI)));

            float velX = (float)Math.Sin(rotation / 180 * Math.PI);
            float velY = -(float)Math.Cos(rotation / 180 * Math.PI);

            var bullet = new GameObject(ObjectType.OBJECT, "bullet");
            bullet.SetObjectCategory(ObjTypes.BULLET);
            bullet.SetTransform(new Transform(posX, posY, rotation, 6));

            bullet.AddAttribute(ElementType.VIEW, AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/bullet.png");
            bullet.AddAttribute(ElementType.MODEL, AT.AT_COM_VELOCITY,new Vector2d(velX, velY));
            bullet.AddAttribute(ElementType.MODEL, AT.AT_COPTER_BULLETSPEED, 5f);
            bullet.AddBehavior(new BulletB());
            bullet.AddBehavior(new ImageRenderB());
            bullet.AddAttribute(ElementType.MODEL, AT.AT_COM_BOUNDS, new Bounds()
            {
                Width = 5,
                Height = 5
            });
            bullet.SetGroup(Group.COLLIDABLE);
            GameObject.GetSceneRoot().AddChild(bullet);
        }
    }
}
