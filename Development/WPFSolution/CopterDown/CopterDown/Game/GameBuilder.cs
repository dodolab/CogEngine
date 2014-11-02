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
            GameObject player = new GameObject(ObjectType.SCENE_ROOT, "player");
            player.AddModelAttribute(AT.AT_COPTER_PLAYER_LIVES, 12);
            player.AddModelAttribute(AT.AT_COPTER_PLAYER_SCORE, 0);
            player.AddModelAttribute(AT.AT_COPTER_SPAWNINTERVAL, 0.3f);
            player.AddModelAttribute(AT.AT_COPTER_ACTUALSPAWN, DateTime.Now);
            player.AddModelAttribute(AT.AT_COPTER_COPTERSPEED, 6f);

            GameObject background = new GameObject(ObjectType.OTHER, "background");
            background.SetTransform(new Transform(0, 0, 0));
            background.AddViewAttribute(AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/background.png");
            background.AddViewBehavior(new ImageRenderBehavior());
            player.AddChild(background);


            GameObject canon = new GameObject(ObjectType.OTHER, "canon");
            canon.SetTransform(new Transform(318, 223, 0, 2, 0.5f, 1));
            canon.AddViewAttribute(AT.AT_COM_IMGSOURCE, "pack://application:,,,/Images/canon.png");
            canon.AddViewBehavior(new ImageRenderBehavior());
            canon.AddModelBehavior(new CanonBehaviour());
            canon.AddModelAttribute(AT.AT_COPTER_CANON_CADENCY, 5f);
            canon.AddModelAttribute(AT.AT_COPTER_CANON_LASTSHOT, DateTime.Now);
            player.AddChild(canon);


            player.AddViewBehavior(new PlayerBehavior());
            return player;
        }

        public GameObject CreateIntroScene()
        {
            GameObject intro = new GameObject(ObjectType.SCENE_ROOT, "intro");
            intro.AddViewBehavior(new IntroBehavior());
            return intro;
        }
    }
}
