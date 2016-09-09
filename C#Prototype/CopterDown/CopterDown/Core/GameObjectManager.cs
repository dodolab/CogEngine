using System;
using System.Collections.Generic;
using System.Linq;

namespace CopterDown.Core
{
    public class GameObjectManager
    {
        private static GameObjectManager _instance;
        private Dictionary<int,int> behaviorListeners;
        private Dictionary<int, int> typeCounter;

        private GameObjectManager()
        {
            allGameObjects = new List<GameObject>();
            allBehaviors = new List<ABehavior>();
            behaviorListeners = new Dictionary<int, int>();
            typeCounter = new Dictionary<int, int>();
        }

        public static GameObjectManager Get
        {
            get
            {
                if(_instance == null) _instance = new GameObjectManager();
                return _instance;
            }
        }

        private List<GameObject> allGameObjects;
        private List<ABehavior> allBehaviors;

        public void AddGameObject(GameObject obj)
        {
            allGameObjects.Add(obj);

            if (typeCounter.ContainsKey(obj.SubType))
            {
                typeCounter[obj.SubType]++;
            }
            else typeCounter.Add(obj.SubType, 1);
        }

        public int GetGameObjectCountBySubType(int type)
        {
            if (typeCounter.ContainsKey(type)) return typeCounter[type];
            else return 0;
        }

        public void RemoveGameObject(GameObject obj)
        {
            allGameObjects.Remove(obj);

            typeCounter[obj.SubType]--;
        }

        public void AddBehavior(ABehavior beh)
        {
            allBehaviors.Add(beh);

            foreach (var behListener in beh.MessageListeners.GetAllStates())
            {
                if (!behaviorListeners.ContainsKey(behListener))
                {
                    behaviorListeners.Add(behListener, 1);
                }
                else behaviorListeners[behListener]++;
            }
        }

        public void RemoveBehavior(ABehavior beh)
        {
            allBehaviors.Remove(beh);

            foreach (var listenerKey in beh.MessageListeners.GetAllStates())
            {
                if (behaviorListeners[listenerKey] == 1) behaviorListeners.Remove(listenerKey);
                else behaviorListeners[listenerKey]--;
            }
        }

        public bool IsRegisteredListener(int listenerType)
        {
            return behaviorListeners.ContainsKey(listenerType);
        }

        public GameObject FindGameObjectById(int id)
        {
            return allGameObjects.FirstOrDefault(gm => gm.Id == id);
        }

        public GameObject FindGameObjectByTag(string tag)
        {
            return allGameObjects.FirstOrDefault(gm => gm.Tag == tag);
        }

        public List<GameObject> FindGameObjectsByTag(string tag)
        {
            return allGameObjects.Where(gm => gm.Tag == tag).ToList();
        }

        public GameObject FindGameObjectBySubType(int subtype)
        {
            return allGameObjects.FirstOrDefault(gm => gm.SubType == subtype);
        }

        public List<GameObject> FindGameObjectsBySubType(int subtype)
        {
            return allGameObjects.Where(gm => gm.SubType == subtype).ToList();
        }

        public void OnMessage(Message msg)
        {
            // todo some log here
        }
    }
}
