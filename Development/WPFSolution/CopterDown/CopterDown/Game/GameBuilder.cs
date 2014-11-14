using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Core.CoreBehavs;
using CopterDown.Core.Entities;
using CopterDown.Core.Types;
using CopterDown.Enums;
using CopterDown.Types;

namespace CopterDown.Game
{
    public class GameBuilder
    {
        public GameObject CreateGameScene()
        {
            GameObject player = CreatePlayer();

            GameObject background = CreateBackground();
            player.AddChild(background);

            GameObject canon = CreateCanon(new Transform(318, 223, 0, 2, 0.5f, 1));
            player.AddChild(canon);
            return player;
        }

        public GameObject CreatePlayer()
        {
            GameObject player = new GameObject(ObjectType.SCENE, Subtypes.PLAYER, "player");
            player.AddAttribute(ElementType.MODEL, Attr.LIVES, 12);
            player.AddAttribute(ElementType.MODEL, Attr.SCORE, 0);
            player.AddAttribute(ElementType.MODEL, Attr.SPAWNINTERVAL, 0.3f);
            player.AddAttribute(ElementType.MODEL, Attr.ACTUALSPAWN, DateTime.Now);
            player.AddAttribute(ElementType.MODEL, Attr.COPTERSPEED, 6f);
            player.AddBehavior(new ScoreB());
            player.AddBehavior(new HudB());
            player.AddBehavior(new PlayerB());
            player.AddBehavior(new ColliderB(new State(Groups.BULLET), new State(Groups.COPTER, Groups.PARA)));
            return player;
        }

        public GameObject CreateBackground()
        {
            GameObject background = new GameObject(ObjectType.ROOT, Subtypes.OTHER, "background");
            background.Transform = new Transform(0, 0, 0);
            background.AddAttribute(ElementType.VIEW, Attr.IMGSOURCE, "pack://application:,,,/Images/background.png");
            background.AddBehavior(new ImageRenderB());
            return background;
        }

        public GameObject CreateCanon(Transform transform)
        {
            GameObject canon = new GameObject(ObjectType.OBJECT, Subtypes.OTHER, "canon");
            canon.Transform = transform;
            canon.AddAttribute(ElementType.VIEW, Attr.IMGSOURCE, "pack://application:,,,/Images/canon.png");
            canon.AddBehavior(new ImageRenderB());
            canon.AddBehavior(new CanonB());
            canon.AddAttribute(ElementType.MODEL, Attr.CANON_CADENCY, 15f);
            canon.AddAttribute(ElementType.MODEL, Attr.CANON_LASTSHOT, DateTime.Now);
            canon.AddAttribute(ElementType.MODEL, Attr.CANON_MINMAXANGLE, new Pair<float>(-75, 75));
            return canon;
        }

        public GameObject CreateIntroScene()
        {
            GameObject intro = new GameObject(ObjectType.SCENE, Subtypes.OTHER, "intro");
            intro.AddBehavior(new ImageRenderB());
            intro.AddAttribute(ElementType.VIEW, Attr.IMGSOURCE, "pack://application:,,,/Images/intro.png");
            intro.AddBehavior(new IntroB());
            return intro;
        }

        public GameObject CreateBullet(Transform transform, Vector2d velocity)
        {
            var bullet = new GameObject(ObjectType.OBJECT, Subtypes.BULLET, "bullet");
            bullet.Transform = transform;

            bullet.AddAttribute(ElementType.VIEW, Attr.IMGSOURCE, "pack://application:,,,/Images/bullet.png");
            bullet.AddAttribute(ElementType.MODEL, Attr.VELOCITY, velocity);
            bullet.AddBehavior(new BulletB());
            bullet.AddBehavior(new ImageRenderB());
            bullet.AddBehavior(new MoveB());
            bullet.AddAttribute(ElementType.MODEL, Attr.BOUNDS, new Bounds(5, 5));
            bullet.Groups.SetState(Groups.BULLET);

            return bullet;
        }

        public GameObject CreatePara(Transform transform)
        {
            GameObject para = new GameObject(ObjectType.OBJECT, Subtypes.PARA, "para");
            para.Transform = transform;

            para.AddAttribute(ElementType.VIEW, Attr.HITFRAME, 0);
            para.AddBehavior(new ParaB());
            para.AddBehavior(new ParaDrawB());
            para.AddAttribute(ElementType.MODEL, Attr.BOUNDS, new Bounds(20, 20));
            para.Groups.SetState(Groups.PARA);
            return para;
        }

        public GameObject CreateCopter(Transform transform, bool leftDirection)
        {
            GameObject copter = new GameObject(ObjectType.OBJECT, Subtypes.COPTER, "copter");
            copter.Transform = transform;
            if(leftDirection) copter.States.SetState(States.LEFT_DIR);
            copter.AddAttribute(ElementType.VIEW, Attr.FRAME, 0);
            copter.AddAttribute(ElementType.VIEW, Attr.HITFRAME, 0);
            copter.AddBehavior(new CopterDrawB());
            copter.AddAttribute(ElementType.MODEL, Attr.BOUNDS, new Bounds(115, 51));
            copter.Groups.SetState(Groups.COPTER);
            copter.AddBehavior(new CopterB());
            return copter;
        }
    }
}
