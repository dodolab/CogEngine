using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreAttribs
{
    public class SimpleState
    {
        private int _state;

        public bool HasState(int state)
        {
            int offset = 1 << state;
            return (_state & offset) == state;
        }

        public void SetState(int state)
        {
            int offset = 1 << state;
            _state = _state | offset;
        }

        public void ResetState(int state)
        {
            int offset = 1 << state;
            _state = _state ^ offset;
        }
    }
}
