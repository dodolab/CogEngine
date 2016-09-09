using System;
using System.Collections.Generic;
using CopterDown.Core.Entities;
using CopterDown.Enums;
using CopterDown.Types;

namespace CopterDown.Core.CoreBehavs
{
    /// <summary>
    /// Takovy pokus na ovladani klavesnici
    /// </summary>
    public class MoveB : ABehavior
    {
        public MoveB() : base(ElementType.MODEL, new State()) { }

        public override void OnMessage(Message msg)
        {
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var transform = GameObject.Transform;
            var velocity = GameObject.FindAtt<Vector2d>(Attr.VELOCITY);

            transform.LocalPos.X += velocity.Value.X * delta.Milliseconds;
            transform.LocalPos.Y += velocity.Value.Y * delta.Milliseconds;
        }
    }
}
