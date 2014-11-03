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
using CopterDown.Messages;

namespace CopterDown.Behavior
{
    public class RenderBehavior : ABehavior
    {
        private UIElement _rect;

        public RenderBehavior(UIElement rect)
        {
            this._rect = rect;
        }

        public UIElement Element
        {
            get { return _rect; }
        }

        public override void OnMessage(Message msg)
        {
            
        }
/*
        protected Attribute<Vector2d> GetPosition(GameObject obj, bool absolute)
        {
            var position =
                obj.FindModelAtt(absolute ? AT.AT_COM_ABS_POSITION : AT.AT_COM_POSITION) as Attribute<Vector2d>;
            if (position == null)
            {
                position = new Attribute<Vector2d>(new Vector2d(0, 0));
                obj.AddModelAttribute(position, absolute ? AT.AT_COM_ABS_POSITION : AT.AT_COM_POSITION);
            }
            return position;
        }

        protected Attribute<Vector2d> GetOrigin(GameObject obj, bool absolute)
        {
            var origin =
                obj.FindModelAtt(absolute ? AT.AT_COM_ABS_ORIGIN : AT.AT_COM_ORIGIN) as Attribute<Vector2d>;
            if (origin == null)
            {
                origin = new Attribute<Vector2d>(new Vector2d(0, 0));
                obj.AddModelAttribute(origin,absolute ? AT.AT_COM_ABS_ORIGIN : AT.AT_COM_ORIGIN);
            }
            return origin;
        }

        protected Attribute<float> GetRotation(GameObject obj, bool absolute)
        {
            var rotation =
                obj.FindModelAtt(absolute ? AT.AT_COM_ABS_ROTATION : AT.AT_COM_ROTATION) as Attribute<float>;

            if (rotation == null)
            {
                rotation = new Attribute<float>(0);
                obj.AddModelAttribute(rotation, absolute ? AT.AT_COM_ABS_ROTATION : AT.AT_COM_ROTATION);
            }
            return rotation;
        }

        protected Attribute<int> GetZIndex(GameObject obj)
        {
            var zindex =
                obj.FindModelAtt(AT.AT_COM_ZINDEX) as Attribute<int>;

            if (zindex == null)
            {
                zindex = new Attribute<int>(0);
                obj.AddModelAttribute(zindex, AT.AT_COM_ZINDEX);
            }
            return zindex;
        } 

        */

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
        /*    var position = GetPosition(GameObject,false).Value;
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

            cnv.Children.Add(_rect);*/
        }
    }
}
