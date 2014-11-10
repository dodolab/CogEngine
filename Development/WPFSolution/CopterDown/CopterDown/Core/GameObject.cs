using System;
using System.Collections.Generic;
using System.Linq;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Core.Types;
using CopterDown.Enums;

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
        protected ObjectType _type;
        protected int _subType;
        protected State _groups;
        protected State _states;
        protected Transform _transform;

        protected static int ids = 0;


        public GameObject(ObjectType type, int subtype, string tag)
        {
            _transform = new Transform(0,0);
            this._type = type;
            this._subType = subtype;
            _id = ids++;
            // prevention from overflow into negative numbers
            if (ids < 0) ids = 0;
            this._tag = tag;
            this._groups = new State();
            this._states = new State();

            GameObjectManager.Get.AddGameObject(this);
        }

        /// <summary>
        /// Destroys this object
        /// </summary>
        public void Destroy()
        {
            Parent.RemoveChild(this);
            foreach(var beh in _behaviors) GameObjectManager.Get.RemoveBehavior(beh);
            GameObjectManager.Get.RemoveGameObject(this);
            SendMessage(new Message(ElementType.ALL, new State(Traverses.SCENEROOT), Actions.GAMEOBJECT_DESTROYED, SenderType.GAMEOBJECT, Id, this));
        }

        public void SendMessage(Message msg)
        {
            GameObjectManager.Get.OnMessage(msg);

            if (GameObjectManager.Get.IsRegisteredListener(msg.Action))
            {
                if (msg.Traverse.HasState(Traverses.ROOT))
                {
                    msg.Traverse.ResetState(Traverses.ROOT);
                    if (Root != null)
                    {
                        Root.SendMessage(msg);
                        return;
                    }
                }

                if (msg.Traverse.HasState(Traverses.SCENEROOT))
                {
                    msg.Traverse.ResetState(Traverses.SCENEROOT);
                    if (SceneRoot != null)
                    {
                        SceneRoot.SendMessage(msg);
                        return;
                    }
                }

                // check only to prevent from settings traverses to both: child first and beh first
                bool childfirst = false;

                // traverse children at first
                if (msg.Traverse.HasState(Traverses.CHILD_FIRST))
                {
                    childfirst = true;
                    if (_children != null)
                        foreach (var child in _children.ToList())
                        {
                            child.SendMessage(msg);
                        }
                }

                if (_behaviors != null)
                {
                    foreach (var behavior in _behaviors)
                    {
                        if (behavior.Id != msg.OwnerId &&
                            (msg.Category == ElementType.ALL || behavior.ElemType == msg.Category))
                        {
                            if (behavior.MessageListeners.HasState(msg.Action))
                            {
                                behavior.OnMessage(msg);
                            }
                        }
                    }
                }

                // traverse children at last
                if (!childfirst && msg.Traverse.HasState(Traverses.BEH_FIRST))
                {
                    if (_children != null) foreach (var child in _children.ToList()) child.SendMessage(msg);
                }
            }
        }

        public void Update(TimeSpan delta, TimeSpan absolute)
        {

            if (_children != null)
            {
                foreach (var child in _children.ToList())
                {
                    // make transformation node
                    child.Transform.UpdateTransform(Transform);
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
                    if (beh.ElemType == ElementType.MODEL)
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
                    if (beh.ElemType == ElementType.VIEW)
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
            GameObjectManager.Get.AddBehavior(beh);
            SendMessage(new Message(ElementType.ALL, Traverses.SCENEROOT, Actions.BEHAVIOR_ADDED, SenderType.GAMEOBJECT, _id, beh));
        }

        public void RemoveBehavior(ABehavior beh)
        {
            _behaviors.Remove(beh);
            GameObjectManager.Get.RemoveBehavior(beh);
            SendMessage(new Message(ElementType.ALL, Traverses.SCENEROOT, Actions.BEHAVIOR_REMOVED, SenderType.GAMEOBJECT, _id, beh));
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

        public IReadOnlyCollection<Attribute> Attributes
        {
            get
            {
                return _attributes.Values.ToList().AsReadOnly();
            }
        }

        public IReadOnlyCollection<ABehavior> Behaviors
        {
            get
            {
                return _behaviors;
            }
        }

        public IReadOnlyCollection<GameObject> Children
        {
            get
            {
                return _children;
            }
        }

        public void AddChild(GameObject child)
        {
            if (_children == null) _children = new List<GameObject>();
            child._parent = this;
            _children.Add(child);
        }

        public void RemoveChild(GameObject child)
        {
            child.Parent = null;
            _children.Remove(child);
        }

        public GameObject Parent
        {
            get { return _parent; }
            set { this._parent = value; }
        }

        public int Id
        {
            get { return _id; }
            protected set { _id = value; }
        }

        public string Tag
        {
            get { return _tag; }
            set { _tag = value; }
        }

        public ObjectType Type
        {
            get { return _type; }
            protected set { _type = value; }
        }

        public int SubType
        {
            get { return _subType; }
            protected set { _subType = value; }
        }

        public GameObject FindParent(ObjectType type)
        {
            var parent = Parent;
            while (parent != null && parent._type != type) parent = parent.Parent;
            return parent;
        }

        public GameObject SceneRoot
        {
            get
            {
                return _type == ObjectType.SCENE ? this : FindParent(ObjectType.SCENE);
            }
        }

        public GameObject Root
        {
            get
            {
                return _type == ObjectType.ROOT ? this : FindParent(ObjectType.ROOT);
            }
        }

        public Transform Transform
        {
            get { return _transform; }
            set { _transform = value; }
        }

        public void AddAttribute<T>(ElementType elemType, int key, T value)
        {
            var newAttrib = new Attribute<T>(elemType, this, key, value);
            if (_attributes == null) _attributes = new Dictionary<int, Attribute>();
            _attributes[newAttrib.Key] = newAttrib;
        }

        public State Groups
        {
            get { return _groups; }
            protected set { _groups = value; }
        }

        public State States
        {
            get { return _states; }
            protected set { _states = value; }
        }
    }
}
