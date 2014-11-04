using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using CopterDown.Behavior;
using CopterDown.Core.CoreAttribs;
using CopterDown.Messages;

namespace CopterDown.Core
{
    public class GameObject
    {
        protected Dictionary<int, Attribute> _attributes;
        protected List<ABehavior> _behaviors;

        protected GameObject _parent;
        protected List<GameObject> _children;
        protected int _id;
        protected string _tag;
        protected ObjectType type;
        protected int category;
        protected List<Group> _groups;

        protected static int ids = 0;


        public GameObject(ObjectType type, string tag)
        {
            SetTransform(new Transform(0,0));
            this.type = type;
            _id = ids++;
            this._tag = tag;
        }

        /// <summary>
        /// Destroys this object
        /// </summary>
        public void Destroy()
        {
           GetParent().RemoveChild(this);
        }

        public void SendMessage(Message msg)
        {
            // traverse children at first
            if (msg.Traverse == TraverseMode.TRAV_CHILDFIRST)
            {
                if (_children != null) foreach (var child in _children.ToList()) child.SendMessage(msg);
            }

            foreach (var behavior in _behaviors)
            {
                if(behavior.ElementType == msg.Category) behavior.OnMessage(msg);
            }

            // traverse children at last
            if (msg.Traverse == TraverseMode.TRAV_BEHFIRST)
            {
                if (_children != null) foreach (var child in _children.ToList()) child.SendMessage(msg);
            }
        }

        public void Update(TimeSpan delta, TimeSpan absolute)
        {

            if (_children != null)
            {
                foreach (var child in _children.ToList())
                {
                    // make transformation node
                    child.GetTransform().UpdateTransform(GetTransform());
                }

                foreach (var child in _children.ToList())
                {
                    // update children
                    child.Update(delta, absolute);
                }
            }

            if (_behaviors != null)
            {
                foreach (var beh in _behaviors)
                {
                    if (beh.ElementType == ElementType.MODEL)
                    {
                        beh.Update(delta, absolute);
                    }
                }
            }
        }

        public void Draw(TimeSpan delta, TimeSpan absolute)
        {
            if (_children != null) foreach (var child in _children.ToList()) child.Draw(delta, absolute);

            if (_behaviors != null)
            {
                foreach (var beh in _behaviors)
                {
                    if (beh.ElementType == ElementType.VIEW)
                    {
                        beh.Update(delta, absolute);
                    }
                }
            }
        }

        public void AddBehavior(ABehavior beh)
        {
            if (_behaviors == null) _behaviors = new List<ABehavior>();
            beh.GameObject = this;
            _behaviors.Add(beh);
            SendMessage(new Message(beh.ElementType, TraverseMode.TRAV_BEHFIRST, MessageType.BEHAVIOR_CREATED, beh));
        }

        public void RemoveBehavior(ABehavior beh)
        {
            _behaviors.Remove(beh);
            SendMessage(new Message(beh.ElementType, TraverseMode.TRAV_BEHFIRST, MessageType.BEHAVIOR_REMOVED, beh));
        }

        public void RemoveAttribute(int key)
        {
            _attributes.Remove(key);
        }

        public Attribute<T> FindAtt<T>(int id)
        {
            return (_attributes != null && _attributes.ContainsKey(id)) ? (Attribute<T>)_attributes[id] : null;
        }

        public T FindAttValue<T>(int id)
        {
            var output = FindAtt<T>(id);
            return output == null ? default(T) : output.Value;
        }

        public IReadOnlyCollection<Attribute> GetAttributes()
        {
            return _attributes.Values.ToList().AsReadOnly();
        }

        public IReadOnlyCollection<ABehavior> GetBehaviors()
        {
            return _behaviors;
        }

        public IReadOnlyCollection<GameObject> GetChildren()
        {
            return _children;
        }

        public void AddChild(GameObject child)
        {
            if (_children == null) _children = new List<GameObject>();
            child._parent = this;
            _children.Add(child);
        }

        public void RemoveChild(GameObject child)
        {
            child.SetParent(null);
            _children.Remove(child);
        }

        public GameObject GetParent()
        {
            return _parent;
        }

        public void SetParent(GameObject parent)
        {
            this._parent = parent;
        }

        public int GetId()
        {
            return _id;
        }

        public string GetTag()
        {
            return _tag;
        }

        public void SetTag(string tag)
        {
            this._tag = tag;
        }

        public ObjectType GetObjectType()
        {
            return type;
        }

        public int GetObjectCategory()
        {
            return category;
        }

        public void SetObjectCategory(int value)
        {
            this.category = value;
        }

        public GameObject FindParent(ObjectType type)
        {
            var parent = GetParent();
            while (parent != null && parent.type != type) parent = parent.GetParent();
            return parent;
        }

        public GameObject GetSceneRoot()
        {
            return type == ObjectType.SCENE ? this : FindParent(ObjectType.SCENE);
        }

        public GameObject GetRoot()
        {
            return type == ObjectType.ROOT ? this : FindParent(ObjectType.ROOT);
        }

        public void SetTransform(Transform transform)
        {
            AddAttribute(ElementType.MODEL, AT.AT_COM_TRANSFORM, transform);
        }

        public Transform GetTransform()
        {
            return FindAtt<Transform>(AT.AT_COM_TRANSFORM).Value;
        }

        public void AddAttribute<T>(ElementType type, int key, T value)
        {
            var newAttrib = new Attribute<T>(type, value);
            newAttrib.Key = key;
            if (_attributes == null) _attributes = new Dictionary<int, Attribute>();
            _attributes[newAttrib.Key] = newAttrib;
        
        }

        public void SetGroup(Group group)
        {
            if(_groups == null) _groups = new List<Group>();
            if(!_groups.Contains(group)) _groups.Add(group);
        }

        public void RemoveGroup(Group group)
        {
            if (_groups != null) _groups.Remove(group);
        }

        public bool IsInGroup(Group group)
        {
            return (_groups != null && _groups.Contains(group));
        }
    }
}
