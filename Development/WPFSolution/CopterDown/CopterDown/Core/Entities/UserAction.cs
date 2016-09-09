using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core.Enums;

namespace CopterDown.Core.Entities
{
    public class UserAction
    {
        public UserAction()
        {
            ActionsStarted = new List<Act>();
            ActionsEnded = new List<Act>();
            TouchesPositions = new List<Vector2d>();
        }

        public List<Act> ActionsStarted { get; set; }
        public List<Act> ActionsEnded { get; set; }
        public List<Vector2d> TouchesPositions { get; set; } 
    }

}
