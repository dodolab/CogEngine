using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Behavior
{
    public class RenderBehavior : ABehavior
    {
        private Rectangle _rect;

        public RenderBehavior(Rectangle rect)
        {
            this._rect = rect;
        }

        public override void OnMessage(Core.Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var position =
                    this.GameObject.FindModelAttributeById(AttributeList.ATTR_POSITION) as SimpleValAttribute<Vector2d>;

            Canvas cnv = GameLoop._canvas;
            Canvas.SetLeft(_rect, position.Value.X);
            Canvas.SetTop(_rect,position.Value.Y);
            cnv.Children.Add(_rect);
        }
    }
}
