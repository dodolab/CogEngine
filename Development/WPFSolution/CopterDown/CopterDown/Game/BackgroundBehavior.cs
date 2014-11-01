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

namespace CopterDown.Game
{
    public class BackgroundBehavior : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/background.png", 0, 0, 0, 0, 0,1);
        }

    }
}
