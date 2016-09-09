using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using CopterDown.Behavior;
using CopterDown.Core.CoreAttribs;

namespace CopterDown.Core
{
    public class GameObject
    {
        private Dictionary<int, Attribute> _modelAttributes;
        private Dictionary<int, Attribute> _viewAttributes;

        private List<ABehavior> _modelBehaviors;
        private List<ABehavior> _viewBehaviors;

        private List<Attribute> _viewAttribToRemove = new List<Attribute>();
        private List<Attribute> _modelAttribToRemove = new List<Attribute>(); 
        private List<ABehavior> _viewBehavToRemove = new List<ABehavior>();
        private List<ABehavior> _modelBehavToRemove = new List<ABehavior>(); 

        private GameObject _parent;
        private List<GameObject> _children;
        private int _id;
        private string _tag;
        private ObjectType type;
        private bool isSceneRoot;

        private static int ids = 0;


        public GameObject(ObjectType type, string tag)
        {
            this.type = type;
            _id = ids++;
            this._tag = tag;
        }

        /// <summary>
        /// Destroys this object
        /// </summary>
        public void Destroy()
        {
           // todo: destroy logic here
        }

        public void SendMessage(Message msg)
        {
            if (msg.Traverse == TraverseMode.TRAV_CHILDFIRST)
            {
                if (_children != null) foreach (var child in _children) child.SendMessage(msg);
            }

            if (msg.Category == MessageCat.MODEL)
            {
                if (_modelBehaviors != null) foreach (var beh in _modelBehaviors) beh.OnMessage(msg);
            }
            else if (msg.Category == MessageCat.VIEW)
            {
                if (_viewBehaviors != null) foreach (var beh in _viewBehaviors) beh.OnMessage(msg);
            }

            if (msg.Traverse == TraverseMode.TRAV_BEHFIRST)
            {
                if (_children != null) foreach (var child in _children) child.SendMessage(msg);
            }
        }

        public void Update(TimeSpan delta, TimeSpan absolute)
        {

            if(_children != null) foreach (var child in _children.ToList()) child.Update(delta, absolute);

            if(_modelBehaviors != null) foreach (var beh in _modelBehaviors) beh.Update(delta, absolute);

            foreach (var modelAttrib in _modelAttribToRemove) _modelAttributes[modelAttrib.Key] = null;
            foreach (var viewAttrib in _viewAttribToRemove) _viewAttributes[viewAttrib.Key] = null;
            foreach (var behav in _modelBehavToRemove) _modelBehaviors.Remove(behav);
            foreach (var behav in _viewBehavToRemove) _viewBehaviors.Remove(behav);

            _viewAttribToRemove.Clear();
            _modelAttribToRemove.Clear();
            _viewBehavToRemove.Clear();
            _modelBehavToRemove.Clear();
        }

        public void Draw(TimeSpan delta, TimeSpan absolute)
        {
            if (_children != null) foreach (var child in _children.ToList()) child.Draw(delta, absolute);

            if(_viewBehaviors != null) foreach (var beh in _viewBehaviors) beh.Update(delta, absolute);
        }

        public void AddModelBehavior(ABehavior beh)
        {
            if (_modelBehaviors == null) _modelBehaviors = new List<ABehavior>();
            beh.GameObject = this;
            _modelBehaviors.Add(beh);
            SendMessage(new Message(MessageCat.MODEL, TraverseMode.TRAV_BEHFIRST, MessageType.BEHAVIOR_CREATED, beh));
        }

        public void RemoveModelBehavior(ABehavior beh)
        {
            _modelBehavToRemove.Add(beh);
            SendMessage(new Message(MessageCat.MODEL, TraverseMode.TRAV_BEHFIRST, MessageType.BEHAVIOR_REMOVED, beh));
        }

        public void AddViewBehavior(ABehavior beh)
        {
            if (_viewBehaviors == null) _viewBehaviors = new List<ABehavior>();
            beh.GameObject = this;
            _viewBehaviors.Add(beh);
        }

        public void RemoveViewBehavior(ABehavior beh)
        {
            _viewBehavToRemove.Add(beh);
        }

        public void AddModelAttribute(Attribute attr, int key)
        {
            attr.Key = key;

            if (_modelAttributes == null) _modelAttributes = new Dictionary<int, Attribute>();
            _modelAttributes[attr.Key] = attr;
        }

        public void RemoveModelAttribute(Attribute attr)
        {
            _modelAttribToRemove.Add(attr);
        }

        public void AddViewAttribute(Attribute attr, int key)
        {
            attr.Key = key;

            if (_viewAttributes == null) _viewAttributes = new Dictionary<int, Attribute>();
            _viewAttributes[attr.Key] = attr;
        }

        public void RemoveViewAttribute(Attribute attr)
        {
            _viewAttribToRemove.Add(attr);
        }

        public Attribute FindModelAtt(int id)
        {
            return (_modelAttributes != null && _modelAttributes.ContainsKey(id)) ? _modelAttributes[id] : null;
        }

        public Attribute FindViewAtt(int id)
        {
            return (_viewAttributes != null && _viewAttributes.ContainsKey(id)) ? _viewAttributes[id] : null;
        }

        public IReadOnlyCollection<Attribute> GetModelAttributes()
        {
            return _modelAttributes.Values.ToList().AsReadOnly();
        }

        public IReadOnlyCollection<Attribute> GetViewAttributes()
        {
            return _viewAttributes.Values.ToList().AsReadOnly();
        }

        public IReadOnlyCollection<ABehavior> GetModelBehaviors()
        {
            return _modelBehaviors;
        }

        public IReadOnlyCollection<ABehavior> GetViewBehaviors()
        {
            return _viewBehaviors;
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
            child.Destroy();
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

        public void SetIsSceneRoot(bool val)
        {
            isSceneRoot = val;
        }

        public bool GetIsSceneRoot()
        {
            return isSceneRoot;
        }

        public GameObject GetSceneRoot()
        {
            var parent = GetParent();

            while (parent != null)
            {
                if (parent.isSceneRoot) return parent;
                parent = parent.GetParent();
            }
            return parent;
        }

        public GameObject GetRoot()
        {
            var parent = GetParent();
            while (parent != null && parent.GetParent() != null) parent = parent.GetParent();
            return parent;
        }

        public void SetTransform(Transform transform)
        {
            AddModelAttribute(new Attribute<Transform>(transform),AT.AT_COM_TRANSFORM);
        }

        public Transform GetTransform()
        {
            return ((Attribute<Transform>)FindModelAtt(AT.AT_COM_TRANSFORM)).Value;
        }

        public Attribute<int> GetIntModelAttr(int key)
        {
            return FindModelAtt(key) as Attribute<int>;
        }

        public Attribute<int> GetIntViewAttr(int key)
        {
            return FindViewAtt(key) as Attribute<int>;
        }

        public Attribute<float> GetFloatModelAttr(int key)
        {
            return FindModelAtt(key) as Attribute<float>;
        }

        public Attribute<float> GetFloatViewAttr(int key)
        {
            return FindViewAtt(key) as Attribute<float>;
        }

        public Attribute<bool> GetBoolModelAttr(int key)
        {
            return FindModelAtt(key) as Attribute<bool>;
        }

        public Attribute<bool> GetBoolViewAttr(int key)
        {
            return FindViewAtt(key) as Attribute<bool>;
        }

        public Attribute<Vector2d> GetVectorModelAttr(int key)
        {
            return FindModelAtt(key) as Attribute<Vector2d>;
        }

        public Attribute<Vector2d> GetVectorViewAttr(int key)
        {
            return FindViewAtt(key) as Attribute<Vector2d>;
        }
    }
}
