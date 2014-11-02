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
            Scale = new Vector2d(1,1);
            RotationOrigin = new Vector2d(0.5f, 0.5f);
        }

        public Transform(float posX, float posY, float rotation) : this(posX,posY)
        {
            this.Rotation = rotation;
        }

        public Transform(float posX, float posY, float rotation, int zIndex) :this(posX,posY,rotation)
        {
            this.ZIndex = zIndex;
        }

        public Transform(float posX, float posY, float rotation, int zIndex, float rotOriginX, float rotOriginY)
            : this(posX, posY, rotation, zIndex)
        {
            this.RotationOrigin = new Vector2d(rotOriginX, rotOriginY);
        }

        public Vector2d LocalPos { get; set; }
        public Vector2d Scale { get; set; }
        public float Rotation { get; set; }
        public Vector2d RotationOrigin { get; set; }
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
