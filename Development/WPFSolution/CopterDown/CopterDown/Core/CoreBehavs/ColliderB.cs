using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game.Types;

namespace CopterDown.Core.CoreBehavs
{
    public class ColliderB : ABehavior
    {
        protected State firstCollidableGroups;
        protected State secondCollidableGroups;

        public ColliderB(State firstCollidableGroups, State secondCollidableGroups) : base(ElementType.MODEL, new State())
        {
            this.firstCollidableGroups = firstCollidableGroups;
            this.secondCollidableGroups = secondCollidableGroups;
        }

        public ColliderB(int collidableGroup) : this(new State(collidableGroup), new State(collidableGroup)){}

        public ColliderB(int firstCollidableGroup, int secondCollidableGroup) : this(new State(firstCollidableGroup), new State(secondCollidableGroup)) { }


        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var allChildren = GameObject.Children.ToList();

            for (int i = 0; i < allChildren.Count - 1; i++)
            {
                var first = allChildren[i];
                bool isInFirstGroup = first.Groups.ContainsAtLeastOne(firstCollidableGroups);
                bool isInSecondGroup = first.Groups.ContainsAtLeastOne(secondCollidableGroups);

                if (isInFirstGroup || isInSecondGroup)
                {
                    for (int j = i + 1; j < allChildren.Count; j++)
                    {
                        var second = allChildren[j];
                        bool isSecondInFirstGroup = second.Groups.ContainsAtLeastOne(firstCollidableGroups);
                        bool isSecondInSecondGroup = second.Groups.ContainsAtLeastOne(secondCollidableGroups);

                        if ((isInFirstGroup && isSecondInSecondGroup) || (isInSecondGroup && isSecondInFirstGroup))
                        {
                            var firstBounds = first.FindAtt<Bounds>(Attr.BOUNDS);

                            if (firstBounds!= null && firstBounds.Value.Collides(first, second))
                            {
                                GameObject.SendMessage(new Message(ElementType.MODEL, Traverses.BEH_FIRST,
                                    Actions.COLISION_OCURRED, SenderType.BEHAVIOR, Id, new Collision(first.Id,second.Id)));
                            }
                        }
                    }
                }
            }
        }
    }
}
