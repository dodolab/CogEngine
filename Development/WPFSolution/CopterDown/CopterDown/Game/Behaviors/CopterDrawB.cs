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
                        var leftDirection = GameObject.States.HasState(States.LEFT_DIR);
            var transform = GameObject.Transform;
            if (!leftDirection) transform.Scale.X = -1;
            else transform.Scale.X = 1;
            var matrix = transform.GetAbsoluteMatrix();
            var isHit = GameObject.States.HasState(States.IS_HIT);


            if (frame.Value++ % 10 < 5)
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter1.png", matrix, 1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter2.png", matrix, 1);

            }

            if (isHit) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion.png", matrix, 1);
        }
    }
}
