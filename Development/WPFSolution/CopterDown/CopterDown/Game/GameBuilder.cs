using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;

namespace CopterDown.Game
{
    public class GameBuilder
    {
        public GameObject CreateGameScene()
        {
            GameObject player = new GameObject(ObjectType.OTHER, "player");
            player.AddModelAttribute(new Attribute<int>(12), AT.AT_COPTER_PLAYER_LIVES);
            player.AddModelAttribute(new Attribute<int>(0), AT.AT_COPTER_PLAYER_SCORE);
            player.AddModelAttribute(new Attribute<float>(5), AT.AT_COPTER_CANON_CADENCY);
            player.AddModelAttribute(new Attribute<float>(5), AT.AT_COPTER_CANON_ROTATION);
            player.AddModelAttribute(new Attribute<DateTime>(DateTime.Now), AT.AT_COPTER_CANON_LASTSHOT);
            player.AddModelAttribute(new Attribute<float>(0.3f),AT.AT_COPTER_SPAWNINTERVAL);
            player.AddModelAttribute(new Attribute<DateTime>(DateTime.Now),AT.AT_COPTER_ACTUALSPAWN);
            player.AddModelAttribute(new Attribute<float>(5), AT.AT_COPTER_BULLETSPEED);
            // musi byt view, protoze vykresluje a potrebuje dispatcher
            player.AddViewBehavior(new BackgroundBehavior());
            player.AddViewBehavior(new CanonBehaviour());
            player.AddViewBehavior(new PlayerBehavior());
            return player;
        }

        public GameObject CreateIntroScene()
        {
            GameObject intro = new GameObject(ObjectType.OTHER, "intro");
            intro.AddViewBehavior(new IntroBehavior());
            return intro;
        }
    }
}
