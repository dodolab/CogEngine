using System;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game.Types;

namespace CopterDown.Game
{
    public class ScoreB : ABehavior
    {
        public ScoreB() : base(ElementType.MODEL, new State(GameActions.GAMEOBJECT_KILLED)) { }

        public override void OnMessage(Message msg)
        {
            if (msg.Action == GameActions.GAMEOBJECT_KILLED)
            {
                var gameObj = msg.Data as GameObject;
                if (gameObj.SubType == Subtypes.COPTER)
                {
                    GameObject.SceneRoot.FindAtt<int>(GameAttr.SCORE).Value += 100;
                }
                else if (gameObj.SubType == Subtypes.PARA)
                {
                    GameObject.SceneRoot.FindAtt<int>(GameAttr.SCORE).Value += 20;
                }
                
            }
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            
        }
    }
}
