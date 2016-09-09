using System;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
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

            var lastShot = GameObject.FindAtt <DateTime>(Attr.CANON_LASTSHOT);
            var actualCadency = GameObject.FindAtt<float>(Attr.CANON_CADENCY);
            var angle = GameObject.FindAtt<Pair<float>>(Attr.CANON_MINMAXANGLE);
            var transform = GameObject.Transform;

            if (userActions.ActionsStarted.Contains(Act.LEFT))
            {
                if (transform.Rotation > angle.Value.Item1) transform.Rotation -= (float)delta.TotalSeconds * 160;
            }
            if (userActions.ActionsStarted.Contains(Act.RIGHT))
            {
                if (transform.Rotation < angle.Value.Item2) transform.Rotation += (float)delta.TotalSeconds * 160;
            }
            if (userActions.ActionsStarted.Contains(Act.FIRE))
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

            var bullet = new GameBuilder().CreateBullet(new Transform(posX, posY, rotation, 6), new Vector2d(velX, velY));
            GameObject.SceneRoot.AddChild(bullet);
        }
    }
}
