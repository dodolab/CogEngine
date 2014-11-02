using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Game
{
    public class CopterBehavior : ABehavior
    {
        public override void OnMessage(Message msg)
        {
            
        }

        private Random rnd = new Random();

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var frame = GameObject.FindViewAtt<int>(AT.AT_COM_FRAME);
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindModelAtt<bool>(AT.AT_COPTER_PARA_ISHIT);
            var leftDirection =GameObject.FindModelAtt<bool>(AT.AT_COPTER_LEFTDIRECTION);

            if (frame.Value++ % 10 < 5)
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter1.png", leftDirection.Value ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.0f, 0.0f,
                    5,!leftDirection.Value ? -1 : 1,1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter2.png", leftDirection.Value ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.5f, 0.0f,
                    5,!leftDirection.Value ? -1 : 1,1);

            }

            if (isHit.Value) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5,1,1);

            var copterSpeed = GameObject.GetSceneRoot().FindModelAtt<float>(AT.AT_COPTER_COPTERSPEED);
            var hitFrame = GameObject.FindViewAtt<int>(AT.AT_COPTER_HITFRAME);

            // update copter

            int next = rnd.Next(0, 60);
            transform.LocalPos.X += copterSpeed.Value * (leftDirection.Value ? -1 : 1);

            if (isHit.Value && hitFrame.Value++ > 10) GameObject.Destroy();

            if (transform.LocalPos.X < -111 || transform.LocalPos.X > 640) leftDirection.Value = !leftDirection.Value;

            if (!isHit.Value && next == 15 && (transform.LocalPos.X <= 280 || transform.LocalPos.X >= 340) && (transform.LocalPos.X > 50 || transform.LocalPos.X < 580))
            {
                SpawnParatrooper(transform.LocalPos.X + 55, transform.LocalPos.Y + 50);
            }
        }

        private void SpawnParatrooper(float x, float y)
        {
            GameObject para = new GameObject(ObjectType.PARA, "para");
            para.SetTransform(new Transform(x,y));

            para.AddModelAttribute(AT.AT_COPTER_PARA_ISGROUNDED,false);
            para.AddModelAttribute(AT.AT_COPTER_PARA_ISHIT, false);
            para.AddViewAttribute(AT.AT_COPTER_HITFRAME, 0);
            para.AddViewBehavior(new ParaBehavior());

            GameObject.GetParent().AddChild(para);
        }
    }
}
