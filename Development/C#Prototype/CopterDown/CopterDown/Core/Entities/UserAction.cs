using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CopterDown.Core.Enums;

namespace CopterDown.Core.Entities
{
    public class UserAction
    {
        public UserAction()
        {
            KeyActions = new List<InputAct<Act>>();
            PointActions = new List<InputAct<Vector2d>>();
        }

        public List<InputAct<Act>> KeyActions { get; set; }
        public List<InputAct<Vector2d>> PointActions { get; set; }
    }
}
