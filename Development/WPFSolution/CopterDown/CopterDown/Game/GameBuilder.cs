using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Behavior;
using CopterDown.Core;
using CopterDown.Core.CoreAttribs;
using CopterDown.Core.CoreBehavs;

namespace CopterDown.Game
{
    public class GameBuilder
    {
        public GameObject CreateGameScene()
        {
            GameObject player = new GameObject(ObjectType.SCENE, "player");
            player.AddAttribute(ElementType.MODEL, AT.AT_COPTER_PLAYER_LIVES, 12);
            player.AddAttribute(ElementType.MODEL, AT.AT_COPTER_PLAYER_SCORE, 0);
            player.AddAttribute(ElementType.MODEL, AT.AT_COPTER_SPAWNINTERVAL, 0.3f);
            player.AddAttribute(ElementType.MODEL, AT.AT_COPTER_ACTUALSPAWN, DateTime.Now);
            player.AddAttribute(ElementType.MODEL, AT.AT_COPTER_COPTERSPEED, 6f);
            player.AddBehavior(new ScoreB());
            player.AddBehavior(new HudB());
            player.AddBehavior(new ColliderB());

            GameObject background = new GameObject(ObjectType.ROOT, "background");
            background.SetTransform(new Transform(0, 0, 0));
            background.AddAttribute(ElementType.VIEW, AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/background.png");
            background.AddBehavior(new ImageRenderB());
            player.AddChild(background);


            GameObject canon = new GameObject(ObjectType.OBJECT, "canon");
            canon.SetTransform(new Transform(318, 223, 0, 2, 0.5f, 1));
            canon.AddAttribute(ElementType.VIEW, AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/canon.png");
            canon.AddBehavior(new ImageRenderB());
            canon.AddBehavior(new CanonB());
            canon.AddAttribute(ElementType.MODEL, AT.AT_COPTER_CANON_CADENCY, 5f);
            canon.AddAttribute(ElementType.MODEL, AT.AT_COPTER_CANON_LASTSHOT, DateTime.Now);
            player.AddChild(canon);


            player.AddBehavior(new PlayerB());
            return player;
        }

        public GameObject CreateIntroScene()
        {
            GameObject intro = new GameObject(ObjectType.SCENE, "intro");
            intro.AddBehavior(new ImageRenderB());
            intro.AddAttribute(ElementType.VIEW, AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/intro.png");
            intro.AddBehavior(new IntroB());
            return intro;
        }
    }
}
