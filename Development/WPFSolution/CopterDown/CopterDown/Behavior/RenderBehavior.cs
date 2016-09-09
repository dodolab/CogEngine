using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Behavior
{
    public class RenderBehavior : ABehavior
    {
        private Rectangle _rect;

        public RenderBehavior(Rectangle rect)
        {
            this._rect = rect;
        }

        public override void OnMessage(Message msg)
        {
            
        }

        protected SimpleValAttribute<Vector2d> GetPosition(GameObject obj, bool absolute)
        {
            var position =
                obj.FindModelAttributeById(absolute ? AttributeList.ATTR_ABS_POSITION : AttributeList.ATTR_POSITION) as SimpleValAttribute<Vector2d>;
            if (position == null)
            {
                position = new SimpleValAttribute<Vector2d>(new Vector2d(0, 0));
                obj.AddModelAttribute(position, absolute ? AttributeList.ATTR_ABS_POSITION : AttributeList.ATTR_POSITION);
            }
            return position;
        }

        protected SimpleValAttribute<Vector2d> GetOrigin(GameObject obj, bool absolute)
        {
            var origin =
                obj.FindModelAttributeById(absolute ? AttributeList.ATTR_ABS_ORIGIN : AttributeList.ATTR_ORIGIN) as SimpleValAttribute<Vector2d>;
            if (origin == null)
            {
                origin = new SimpleValAttribute<Vector2d>(new Vector2d(0, 0));
                obj.AddModelAttribute(origin,absolute ? AttributeList.ATTR_ABS_ORIGIN : AttributeList.ATTR_ORIGIN);
            }
            return origin;
        }

        protected SimpleValAttribute<float> GetRotation(GameObject obj, bool absolute)
        {
            var rotation =
                obj.FindModelAttributeById(absolute ? AttributeList.ATTR_ABS_ROTATION : AttributeList.ATTR_ROTATION) as SimpleValAttribute<float>;

            if (rotation == null)
            {
                rotation = new SimpleValAttribute<float>(0);
                obj.AddModelAttribute(rotation, absolute ? AttributeList.ATTR_ABS_ROTATION : AttributeList.ATTR_ROTATION);
            }
            return rotation;
        }

        protected SimpleValAttribute<int> GetZIndex(GameObject obj)
        {
            var zindex =
                obj.FindModelAttributeById(AttributeList.ATTR_ZINDEX) as SimpleValAttribute<int>;

            if (zindex == null)
            {
                zindex = new SimpleValAttribute<int>(0);
                obj.AddModelAttribute(zindex, AttributeList.ATTR_ZINDEX);
            }
            return zindex;
        } 



        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var position = GetPosition(GameObject,false).Value;
            var rotation = GetRotation(GameObject,false).Value;
            var zindex = GetZIndex(GameObject).Value;
            var origin = GetOrigin(GameObject,false).Value;

            if (GameObject.GetParent() != null)
            {
                var parent = GameObject.GetParent();
                var parentPos = GetPosition(parent,true).Value;
                var parentRot = GetRotation(parent,true).Value;
                var parentOr = GetOrigin(parent,true).Value;

                position = new Vector2d(position.X+parentPos.X,position.Y+parentPos.Y);
                rotation = rotation + parentRot;
                origin = new Vector2d(parentOr.X+origin.X,parentOr.Y+origin.Y);
            }

            // save absolute attributes
            GetPosition(GameObject,true).Value = new Vector2d(position.X,position.Y);
            GetRotation(GameObject, true).Value = rotation;
            GetOrigin(GameObject,true).Value = new Vector2d(origin.X,origin.Y);

            Canvas cnv = GameLoop._canvas;
            Canvas.SetLeft(_rect, position.X);
            Canvas.SetTop(_rect,position.Y);
            Canvas.SetZIndex(_rect,zindex);

            _rect.RenderTransform = new RotateTransform(rotation, origin.X, origin.Y);

            cnv.Children.Add(_rect);
        }
    }
}
