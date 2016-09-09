using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreAttribs
{
    public class State : Attribute
    {
        private int _state;

        public bool HasState(int state)
        {
            return (_state & state) == state;
        }

        public void SetState(int state)
        {
            _state = _state | state;
        }

        public void ResetState(int state)
        {
            _state = _state ^ state;
        }
    }
}
