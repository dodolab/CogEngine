using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.CoreAttribs
{
    public class ComplexState
    {
        private readonly Dictionary<int, int> states = new Dictionary<int,int>(); 

        public bool HasState(int state)
        {
            var index = GetStateIndex(state);
            var offset = GetStateOffset(state);
            var binary = 1 << offset;

            return states.ContainsKey(index) && ((states[index] & binary) == binary);
        }

        public void SetState(int state)
        {
            var index = GetStateIndex(state);
            var offset = GetStateOffset(state);
            var binary = 1 << offset;

            if (!states.ContainsKey(index)) states.Add(index, binary);
            else states[index] |= binary;
        }

        public void ResetState(int state)
        {
            var index = GetStateIndex(state);
            var offset = GetStateOffset(state);
            var binary = 1 << offset;

            if (states.ContainsKey(index)) states[index] ^= binary;
        }

        private int GetStateIndex(int state)
        {
            return state/sizeof (int);
        }

        private int GetStateOffset(int state)
        {
            return state%(sizeof (int));
        }
    }
}
