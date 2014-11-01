using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Game
{
    public class CanonBehaviour : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var keys = ((Attribute<List<Key>>)GameObject.GetParent().FindModelAtt(AT.AT_COPTER_KEYINPUT)).Value;

            var lastShot = (Attribute<DateTime>) GameObject.FindModelAtt(AT.AT_COPTER_CANON_LASTSHOT);
            var actualCadency = (Attribute<float>)GameObject.FindModelAtt(AT.AT_COPTER_CANON_CADENCY);
            var rotation =
                (Attribute<float>) GameObject.FindModelAtt(AT.AT_COPTER_CANON_ROTATION);

            if (keys.Contains(Key.Left))
            {
                if (rotation.Value > -75) rotation.Value -= (float)delta.TotalSeconds * 60;
            }
            if (keys.Contains(Key.Right))
            {
                if (rotation.Value < 75) rotation.Value += (float)delta.TotalSeconds * 60;
            }
            if (keys.Contains(Key.Space))
            {
                if ((DateTime.Now - lastShot.Value).TotalSeconds > 1.0 / actualCadency.Value)
                {
                    Shoot(rotation.Value);
                    lastShot.Value = DateTime.Now;
                }
            }


            Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/canon.png", 318, 223, rotation.Value, 0.5f, 1,2);
        }

        private void Shoot(float rotation)
        {
            float canonLength = 37;
            float posX = (float)(318 + 2 + canonLength * Math.Sin(rotation / 180 * Math.PI));
            float posY = (float)(223 + (canonLength - canonLength * Math.Cos(rotation / 180 * Math.PI)));

            float velX = (float)Math.Sin(rotation / 180 * Math.PI);
            float velY = -(float)Math.Cos(rotation / 180 * Math.PI);

            var bullet = new GameObject(ObjectType.BULLET, "bullet");
            bullet.SetTransform(new Transform(posX, posY, rotation));
            bullet.AddModelAttribute(new Attribute<Vector2d>(new Vector2d(velX, velY)), AT.AT_COM_VELOCITY);
            bullet.AddViewBehavior(new BulletBehavior());

            GameObject.AddChild(bullet);
        }
    }
}
