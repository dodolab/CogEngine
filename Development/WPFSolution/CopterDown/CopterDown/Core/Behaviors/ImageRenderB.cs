using System;
using System.Collections.Generic;
using System.Linq;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game;
using CopterDown.Game.Types;
using CopterDown.Utils;

namespace CopterDown.Core.CoreBehavs
{
    public class ImageRenderB : ABehavior
    {
        public ImageRenderB() : base(ElementType.VIEW, new State()) { }

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var imgSource = GameObject.FindAtt<string>(Attr.IMGSOURCE);
            var transform = GameObject.Transform.GetAbsoluteMatrix();
            var zindex = GameObject.Transform.GetAbsoluteZIndex();

            Helper.DrawImage(GameLoop._canvas, imgSource.Value, transform, zindex);
        }

    }
}
