using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Messages;

namespace CopterDown.Behavior
{
    /// <summary>
    /// Takovy pokus na ovladani klavesnici
    /// </summary>
    public class MovementBehavior : ABehavior
    {
        public MovementBehavior()
        {
            
        }

        private float[] lastMovement;

        public override void OnMessage(Message msg)
        {
            if (msg.TypeId == 12)
            {
                float[] movement = msg.Data as float[];
                lastMovement = movement;
                // wait for update
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            if (lastMovement != null)
            {
                var position =
                    GameObject.GetTransform();

                position.LocalPos.X += lastMovement[0];
                position.LocalPos.Y += lastMovement[1];

                lastMovement = null;
            }  
        }
    }
}
