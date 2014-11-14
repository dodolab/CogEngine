using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Types;
using CopterDown.Enums;
using CopterDown.Utils;

namespace CopterDown.Game
{
    public class ParaDrawB : ABehavior
    {
        public ParaDrawB() : base(ElementType.VIEW, new State()){}
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var isGrounded = GameObject.States.HasState(States.IS_GROUNDED);
            var transform = GameObject.Transform;
            var isHit = GameObject.States.HasState(States.IS_HIT);

            if (isGrounded)
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/soldier.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5, 1, 1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/para.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.0f, 0.0f, 5, 1, 1);
            }

            if (isHit) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion2.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5, 1, 1);
        }
    }
}
