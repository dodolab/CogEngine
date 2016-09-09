using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Documents;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Core.Types;
using CopterDown.Enums;
using CopterDown.Types;

namespace CopterDown.Game
{
    public class CanonB : ABehavior
    {
        public CanonB() : base(ElementType.MODEL, new State()){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var userActions = GameObject.Root.FindAtt<UserAction>(Attr.USERACTION).Value;
            var actualWeapon = GameObject.FindAtt<Weapon>(Attr.WEAPON);
            var lastShot = GameObject.FindAtt <DateTime>(Attr.CANON_LASTSHOT);
            var actualCadency = actualWeapon.Value.Cadency;
            var angle = GameObject.FindAtt<Pair<float>>(Attr.CANON_MINMAXANGLE);
            var transform = GameObject.Transform;

            if (userActions.KeyActions.Any(k => k.ActValue == Act.LEFT))
            {
                if (transform.Rotation > angle.Value.Item1) transform.Rotation -= (float)delta.TotalSeconds * 160;
            }
            if (userActions.KeyActions.Any(k => k.ActValue == Act.RIGHT))
            {
                if (transform.Rotation < angle.Value.Item2) transform.Rotation += (float)delta.TotalSeconds * 160;
            }
            if (userActions.KeyActions.Any(k => k.ActValue == Act.FIRE))
            {
                if ((DateTime.Now - lastShot.Value).TotalSeconds > 1.0 / actualCadency)
                {
                    if (actualWeapon.Value.MultiCount != 0)
                    {
                        var rotOffset = transform.Rotation - actualWeapon.Value.MultiCount;
                        var speed = actualWeapon.Value.Speed;

                        for (int i = 0; i < actualWeapon.Value.MultiCount; i++)
                        {
                            var randomSpeed = ((float)(rnd.NextDouble()/2+0.5)*(speed));
                            Shoot(rotOffset+i, randomSpeed, actualWeapon.Value);
                        }

                    }else Shoot(transform.Rotation, actualWeapon.Value.Speed, actualWeapon.Value);
                    lastShot.Value = DateTime.Now;
                }
            }
            if (userActions.KeyActions.Any(k => k.ActValue == Act.SWITCH))
            {
                // set action as handled
                userActions.KeyActions.FirstOrDefault(k => k.ActValue == Act.SWITCH).Handled = true;

                var info = GameObjectManager.Get.FindGameObjectBySubType(Subtypes.INFO);
                var allWeapons = info.FindAttValue <List<Weapon>>(Attr.WEAPONINFO);

                int index = 0;

                for (int i = 0; i < allWeapons.Count; i++)
                {
                    if (allWeapons[i].Name == actualWeapon.Value.Name) index = i;
                }


                index = (index + 1)%allWeapons.Count;
                actualWeapon.Value = allWeapons[index];
            }
        }

        private void Shoot(float rotation, float speed, Weapon actualWeapon)
        {
            float canonLength = 37;
            float posX = (float)(318 + 2 + canonLength * Math.Sin(rotation / 180 * Math.PI));
            float posY = (float)(223 + (canonLength - canonLength * Math.Cos(rotation / 180 * Math.PI)));

            float velX = (float)Math.Sin(rotation / 180 * Math.PI)/4;
            float velY = -(float)Math.Cos(rotation / 180 * Math.PI)/4;

            velX *= speed;
            velY *= speed;

            var bullet = new GameBuilder().CreateBullet(new Transform(posX, posY, rotation, 6), 
                new Vector2d(velX, velY), actualWeapon.ImgSource, actualWeapon.IsImmortal);
            GameObject.SceneRoot.AddChild(bullet);
        }
    }
}
