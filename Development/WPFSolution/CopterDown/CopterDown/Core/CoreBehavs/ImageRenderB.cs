using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using CopterDown.Core;
using CopterDown.Game;
using CopterDown.Messages;

namespace CopterDown.Core.CoreBehavs
{
    public class ImageRenderB : ABehavior
    {
        public ImageRenderB() : base(ElementType.VIEW){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var imgSource = GameObject.FindAtt<string>(AT.AT_COM_IMGSOURCE);
            var transform = GameObject.GetTransform();

            Helper.DrawImage(GameLoop._canvas, imgSource.Value, transform.LocalPos.X, transform.LocalPos.Y, 
                transform.Rotation, transform.RotationOrigin.X, 
                transform.RotationOrigin.Y, transform.ZIndex, transform.Scale.X, transform.Scale.Y);
        }

    }
}
