using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.Messages;
using CopterDown.Messages;

namespace CopterDown.Core.CoreBehavs
{
    public class ColliderB : ABehavior
    {
        public ColliderB() : base(ElementType.MODEL){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var allChildren = GameObject.GetChildren().Where(child => child.IsInGroup(Group.COLLIDABLE)).ToList();

            for (int i = 0; i < allChildren.Count-1; i++)
            {
                var first = allChildren[i];

                for (int j = i + 1; j < allChildren.Count; j++)
                {
                    var second = allChildren[j];
                    if (Collides(first, second))
                    {
                        GameObject.SendMessage(new Message(ElementType.MODEL, TraverseMode.TRAV_BEHFIRST,
                            MessageType.COLISION_OCURRED, new Collision()
                            {
                                FirstId = first.GetId(),
                                SecondId = second.GetId()
                            }));
                    }
                }
            }

        }

        private bool Collides(GameObject first, GameObject second)
        {
            var transformFirst = first.GetTransform();
            var transformSecond = second.GetTransform();
            var boundsFirst = first.FindAtt<Bounds>(AT.AT_COM_BOUNDS);
            var boundsSecond = second.FindAtt<Bounds>(AT.AT_COM_BOUNDS);

            if (transformFirst != null && transformSecond != null && boundsFirst != null && boundsSecond != null)
            {
                var xDiff = Math.Abs(transformFirst.LocalPos.X - transformSecond.LocalPos.X);
                var yDiff = Math.Abs(transformFirst.LocalPos.Y - transformSecond.LocalPos.Y);

                bool hasXIntersect = (transformFirst.LocalPos.X > transformSecond.LocalPos.X &&
                                      xDiff < boundsSecond.Value.Width) ||
                                     (transformFirst.LocalPos.X < transformSecond.LocalPos.X &&
                                      xDiff < boundsFirst.Value.Width);

                bool hasYIntersect = (transformFirst.LocalPos.Y > transformSecond.LocalPos.Y &&
                                      yDiff < boundsSecond.Value.Height) ||
                                     (transformFirst.LocalPos.Y < transformSecond.LocalPos.Y &&
                                      yDiff < boundsFirst.Value.Height);

                return hasXIntersect && hasYIntersect;
            }

            return false;
        }
    }
}
