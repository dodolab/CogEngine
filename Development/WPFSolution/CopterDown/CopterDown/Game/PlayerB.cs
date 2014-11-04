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
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class PlayerB : ABehavior
    {
        public PlayerB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        private Random rnd = new Random();

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var lives = GameObject.FindAtt<int>(AT.AT_COPTER_PLAYER_LIVES);
            if (lives.Value == 0)
            {
                var root = GameObject.GetParent();
                GameObject.Destroy();
                root.AddChild(new GameBuilder().CreateIntroScene());
                return;
            }

            var spawnInterval = GameObject.FindAtt<float>(AT.AT_COPTER_SPAWNINTERVAL);
            var lastSpawn = GameObject.FindAtt<DateTime>(AT.AT_COPTER_ACTUALSPAWN);

            if ((GameObject.GetChildren() == null || GameObject.GetChildren().Where(child => child.GetObjectCategory() == ObjTypes.COPTER).Count() < 6 )
                && ((DateTime.Now - lastSpawn.Value).TotalSeconds > 1.0 / spawnInterval.Value))
            {
                // spawn helicopter
                lastSpawn.Value = DateTime.Now;

                float posY = rnd.Next(0, 50);
                float posX = rnd.NextDouble() < 0.5 ? -111 : 640;

                bool leftdirection = posX == 640;

                GameObject copter = new GameObject(ObjectType.OBJECT, "copter");
                copter.SetObjectCategory(ObjTypes.COPTER);
                copter.SetTransform(new Core.CoreAttribs.Transform(posX, posY));
                copter.AddAttribute(ElementType.MODEL, AT.AT_COPTER_LEFTDIRECTION, leftdirection);
                copter.AddAttribute(ElementType.VIEW, AT.AT_COM_FRAME, 0);
                copter.AddAttribute(ElementType.MODEL, AT.AT_COPTER_PARA_ISHIT, false);
                copter.AddAttribute(ElementType.VIEW, AT.AT_COPTER_HITFRAME, 0);
                copter.AddBehavior(new CopterDrawB());
                copter.AddAttribute(ElementType.MODEL, AT.AT_COM_BOUNDS, new Bounds()
                {
                    Width=115,
                    Height = 51
                });
                copter.SetGroup(Group.COLLIDABLE);
                copter.AddBehavior(new CopterB());

                GameObject.GetSceneRoot().AddChild(copter);
            }

        }
    }
}
