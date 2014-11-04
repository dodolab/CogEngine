using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class CopterB : ABehavior
    {
        public CopterB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        private Random rnd = new Random();

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISHIT);
            var leftDirection =GameObject.FindAtt<bool>(AT.AT_COPTER_LEFTDIRECTION);

            var copterSpeed = GameObject.GetSceneRoot().FindAtt<float>(AT.AT_COPTER_COPTERSPEED);
            var hitFrame = GameObject.FindAtt<int>(AT.AT_COPTER_HITFRAME);

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
            GameObject para = new GameObject(ObjectType.OBJECT, "para");
            para.SetObjectCategory(ObjTypes.PARA);
            para.SetTransform(new Transform(x,y));

            para.AddAttribute(ElementType.MODEL, AT.AT_COPTER_PARA_ISGROUNDED,false);
            para.AddAttribute(ElementType.MODEL, AT.AT_COPTER_PARA_ISHIT, false);
            para.AddAttribute(ElementType.VIEW, AT.AT_COPTER_HITFRAME, 0);
            para.AddBehavior(new ParaB());
            para.AddBehavior(new ParaDrawB());
            para.AddAttribute(ElementType.MODEL, AT.AT_COM_BOUNDS, new Bounds()
            {
                Width=20,
                Height = 20
            });
            para.SetGroup(Group.COLLIDABLE);
            GameObject.GetParent().AddChild(para);
        }
    }
}
