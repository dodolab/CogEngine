using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using CopterDown.Core;

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
            switch (msg.MessageType)
            {
                case MessageType.RENDER:
                    Canvas cnv = msg.Data as Canvas;
                    cnv.Children.Add(_rect);
                    break;
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            
        }
    }
}
