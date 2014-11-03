using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core
{
    public class AT
    {
        // common attributes
        public const int AT_COM_TRANSFORM = 1;
        public const int AT_COM_VELOCITY = 17;
        public const int AT_COM_FRAME = 19;

        public const int AT_COM_ABS_POSITION = 5;
        public const int AT_COM_ABS_ROTATION = 6;
        public const int AT_COM_ABS_ORIGIN = 7;

        // copterdown attributes
        public const int AT_COPTER_PARA_ISGROUNDED = 8;
        public const int AT_COPTER_PARA_ISHIT = 9;

        public const int AT_COPTER_CANON_LASTSHOT = 10;
        public const int AT_COPTER_CANON_CADENCY = 11;
        public const int AT_COPTER_PLAYER_SCORE = 12;
        public const int AT_COPTER_PLAYER_LIVES = 13;

        public const int AT_COPTER_KEYINPUT = 16;
        public const int AT_COPTER_BULLETSPEED = 18;
        public const int AT_COPTER_COPTERSPEED = 25;

        public const int AT_COPTER_LEFTDIRECTION = 20;

        public const int AT_COPTER_SPAWNINTERVAL = 21;
        public const int AT_COPTER_ACTUALSPAWN = 22;

        public const int AT_COPTER_HITFRAME = 23;

        public const int AT_COM_IMGSOURCE = 24;

        public const int AT_COM_BOUNDS = 26;
    }
}
