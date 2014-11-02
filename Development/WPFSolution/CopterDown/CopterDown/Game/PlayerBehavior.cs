using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using CopterDown.Behavior;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Game
{
    public class PlayerBehavior : ABehavior
    {

        public override void OnMessage(Message msg)
        {
            
        }

        private Random rnd = new Random();

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var lives = GameObject.FindModelAtt<int>(AT.AT_COPTER_PLAYER_LIVES);

            if (lives.Value == 0)
            {
                var root = GameObject.GetParent();
                GameObject.Destroy();
                root.AddChild(new GameBuilder().CreateIntroScene());
                return;
            }

            var score = GameObject.FindModelAtt<int>(AT.AT_COPTER_PLAYER_SCORE);

            TextBlock text = new TextBlock();
            text.Text = "Score: " + score.Value.ToString();
            text.FontSize = 20;
            GameLoop._canvas.Children.Add(text);
            Canvas.SetLeft(text, 20);
            Canvas.SetTop(text, 20);
            Canvas.SetZIndex(text,10);

            text = new TextBlock();
            text.Text = "Lives: " + lives.Value.ToString();
            text.FontSize = 20;
            GameLoop._canvas.Children.Add(text);
            Canvas.SetLeft(text, 500);
            Canvas.SetTop(text, 20);
            Canvas.SetZIndex(text, 10);

            var spawnInterval = GameObject.FindModelAtt<float>(AT.AT_COPTER_SPAWNINTERVAL);
            var lastSpawn = GameObject.FindModelAtt<DateTime>(AT.AT_COPTER_ACTUALSPAWN);

            if ((GameObject.GetChildren() == null || GameObject.GetChildren().Where(child => child.GetObjectType() == ObjectType.COPTER).Count() < 6 )
                && ((DateTime.Now - lastSpawn.Value).TotalSeconds > 1.0 / spawnInterval.Value))
            {
                // spawn helicopter
                lastSpawn.Value = DateTime.Now;

                float posY = rnd.Next(0, 50);
                float posX = rnd.NextDouble() < 0.5 ? -111 : 640;

                bool leftdirection = posX == 640;

                GameObject copter = new GameObject(ObjectType.COPTER, "copter");
                copter.SetTransform(new Core.CoreAttribs.Transform(posX, posY));
                copter.AddModelAttribute<bool>(AT.AT_COPTER_LEFTDIRECTION, leftdirection);
                copter.AddViewAttribute<int>(AT.AT_COM_FRAME, 0);
                copter.AddModelAttribute<bool>(AT.AT_COPTER_PARA_ISHIT, false);
                copter.AddViewAttribute<int>(AT.AT_COPTER_HITFRAME, 0);

                copter.AddViewBehavior(new CopterBehavior());

                GameObject.AddChild(copter);
            }

        }
    }
}
