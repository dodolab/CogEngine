using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using CopterDown.Core;

namespace CopterDown.Game
{
    public class HudB : ABehavior
    {
        public HudB() : base(ElementType.VIEW)
        {
        }

        public override void OnMessage(Messages.Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var lives = GameObject.FindAtt<int>(AT.AT_COPTER_PLAYER_LIVES);
            var score = GameObject.FindAtt<int>(AT.AT_COPTER_PLAYER_SCORE);

            TextBlock text = new TextBlock();
            text.Text = "Score: " + score.Value;
            text.FontSize = 20;
            GameLoop._canvas.Children.Add(text);
            Canvas.SetLeft(text, 20);
            Canvas.SetTop(text, 20);
            Canvas.SetZIndex(text, 10);

            text = new TextBlock();
            text.Text = "Lives: " + lives.Value;
            text.FontSize = 20;
            GameLoop._canvas.Children.Add(text);
            Canvas.SetLeft(text, 500);
            Canvas.SetTop(text, 20);
            Canvas.SetZIndex(text, 10);
        }
    }
}
