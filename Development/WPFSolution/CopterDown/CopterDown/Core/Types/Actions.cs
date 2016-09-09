using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Types
{
    public class Actions
    {
        public const int BEHAVIOR_ADDED = 0;
        public const int BEHAVIOR_REMOVED = 1;
        public const int OBJECT_CREATED = 2;
        public const int OBJECT_REMOVED = 3;
        public const int USER_INPUT = 4;
        public const int COLISION_OCURRED = 5;
        public const int GAMEOBJECT_DESTROYED = 6;
        public const int ATTRIBUTE_CHANGED = 7;
        public const int GAMEOBJECT_KILLED = 8;
        public const int PARA_GROUNDED = 9;
    }
}
