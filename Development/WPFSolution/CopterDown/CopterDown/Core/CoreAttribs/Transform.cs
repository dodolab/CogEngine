using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreAttribs
{
    public class Transform
    {
        public Transform(float posX, float posY)
        {
            LocalPos = new Vector2d(posX, posY);
        }

        public Transform(float posX, float posY, float rotation) : this(posX,posY)
        {
            this.Rotation = rotation;
        }

        public Vector2d LocalPos { get; set; }
        public Vector2d Scale { get; set; }
        public float Rotation { get; set; }
        public int ZIndex { get; set; }

        public Transform ParentTransform { get; set; }

        public void UpdateTransform(Transform parent)
        {
            ParentTransform = parent;
        }

        public Vector2d GetAbsolutePosition()
        {
            //todo
            return null;
        }

    }
}
