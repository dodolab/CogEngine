using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Enums;
using CopterDown.Game.Types;
using CopterDown.Utils;

namespace CopterDown.Game
{
    public class CopterDrawB : ABehavior
    {
        public CopterDrawB():base(ElementType.VIEW, new State()){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var frame = GameObject.FindAtt<int>(Attr.FRAME);
            var transform = GameObject.Transform;
            var isHit = GameObject.States.HasState(States.IS_HIT);
            var leftDirection = GameObject.States.HasState(States.LEFT_DIR);

            if (frame.Value++ % 10 < 5)
            {

                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter1.png", leftDirection ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.0f, 0.0f,
                    5, !leftDirection ? -1 : 1, 1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter2.png", leftDirection ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.5f, 0.0f,
                    5, !leftDirection ? -1 : 1, 1);

            }

            if (isHit) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5, 1, 1);
        }
    }
}
