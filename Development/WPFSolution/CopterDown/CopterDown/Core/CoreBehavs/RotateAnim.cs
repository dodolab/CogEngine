using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreBehavs
{
    public class RotateAnim : ABehavior
    {
        private float from;
        private float to;
        private float speed;

        private bool removeAfterFinish;

        // 0 to 1
        private float actual;

        public RotateAnim(float from, float to, float speed, bool removeAfterFinish)
        {
            this.from = from;
            this.to = to;
            this.speed = speed;
            this.actual = from;
            this.removeAfterFinish = removeAfterFinish;
        }

        public override void OnMessage(Message msg)
        {
            
        }

  /*      protected Attribute<float> GetRotation(GameObject obj)
        {
            var rotation =
                obj.FindModelAtt( AT.AT_COM_ROTATION) as Attribute<float>;

            if (rotation == null)
            {
                rotation = new Attribute<float>(0);
                obj.AddModelAttribute(rotation,  AT.AT_COM_ROTATION);
            }
            return rotation;
        }
        */
        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            /*if (Active)
            {
                // toto je diskutabilni
                if (actual >= to)
                {
                    if (removeAfterFinish)
                    {
                        GameObject.RemoveModelBehavior(this);
                    }
                    Active = false;
                    return;
                }

                float diff = (float) ((to-from)/1000*speed*delta.TotalMilliseconds);
                actual += diff;
                if (actual >= to) actual = to;

                var rotation =
                    GetRotation(GameObject);

                rotation.Value += diff;
            }*/
        }
    }
}
