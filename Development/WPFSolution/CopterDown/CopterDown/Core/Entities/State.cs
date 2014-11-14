using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopterDown.Core.Entities
{
    public class State
    {
        private enum StateOperation
        {
            HAS, SET, RESET
        }

        private int simpleState;
        private Dictionary<int, int> states;

        public State()
        {
            
        }

        public State(IEnumerable<int> states)
        {
            foreach(var st in states) SetState(st);
        }

        public State(int state) : this(state, -1, -1, -1, -1){}

        public State(int state1, int state2) : this(state1,state2,-1,-1,-1){}

        public State(int state1, int state2, int state3) :this(state1,state2,state3,-1,-1){}

        public State(int state1, int state2, int state3, int state4) : this(state1,state2,state3,state4,-1){}

        public State(int state1, int state2, int state3, int state4, int state5)
        {
            if (state1 != -1) SetState(state1);
            if (state2 != -1) SetState(state2);
            if (state3 != -1) SetState(state3);
            if (state4 != -1) SetState(state4);
            if (state5 != -1) SetState(state5);
        }

        public State(State copy)
        {
            simpleState = copy.simpleState;

            if (copy.states != null)
            {
                states = new Dictionary<int, int>();
                foreach (var st in copy.states)
                {
                    states.Add(st.Key, st.Value);
                }
            }
        }

        public List<int> GetAllStates()
        {
            var output = new List<int>();

            for (int i = 0; i < intSize; i++)
            {
                if(HasState(i)) output.Add(i);
            }

            if (states != null)
            {
                foreach (var key in states.Keys)
                {
                    int val = states[key];
                    if (val != 0)
                    {
                        for (int i = 0; i < intSize; i++)
                        {
                            int state = key * intSize + i;
                            if(HasState(state)) output.Add(state);
                        }
                    }
                }
            }

            return output;
        } 

        public bool HasState(int state)
        {
            return DoStateOperation(StateOperation.HAS, state);
        }

        public void SetState(int state)
        {
            DoStateOperation(StateOperation.SET, state);
        }

        public void ResetState(int state)
        {
            DoStateOperation(StateOperation.RESET, state);
        }

        private bool DoStateOperation(StateOperation oper, int state)
        {
            var index = GetStateIndex(state);
            var offset = GetStateOffset(state);
            var binary = 1 << offset;

            if (index == 0)
            {
                switch (oper)
                {
                    case StateOperation.HAS:
                        return (simpleState & binary) == binary;
                    case StateOperation.SET:
                        simpleState |= binary;
                        break;
                    case StateOperation.RESET:
                        simpleState ^= binary;
                        break;
                }
                return true;
            }

            if (states == null) states = new Dictionary<int, int>();

            switch (oper)
            {
                case StateOperation.HAS:
                    return states.ContainsKey(index) && ((states[index] & binary) == binary);
                case StateOperation.SET:
                    if (!states.ContainsKey(index)) states.Add(index, binary);
                    else states[index] |= binary;
                    break;
                case StateOperation.RESET:
                    if (states.ContainsKey(index)) states[index] ^= binary;
                    break;
            }
            return true;
        }

        public static bool operator ==(State st1, int st2)
        {
            return st1.HasState(st2);
        }

        public static bool operator !=(State st1, int st2)
        {
            return !st1.HasState(st2);
        }

        public static State operator +(State st1, int st2)
        {
            var output = new State(st1);
            output.SetState(st2);
            return output;
        }

        public static State operator -(State st1, int st2)
        {
            var output = new State(st1);
            output.ResetState(st2);
            return output;
        }

        public bool Contains(State other)
        {
            foreach (var state in other.GetAllStates())
            {
                if (!HasState(state)) return false;
            }
            return true;
        }

        public bool ContainsAtLeastOne(State other)
        {
            foreach (var state in other.GetAllStates())
            {
                if (HasState(state)) return true;
            }
            return false;
        }

        private int GetStateIndex(int state)
        {
            return state / intSize;
        }

        private int GetStateOffset(int state)
        {
            return state%intSize;
        }

        private int intSize = sizeof (int)*8;

        public override bool Equals(object obj)
        {
            if (obj is State)
            {
                var compObj = obj as State;
                return simpleState == compObj.simpleState && compObj.states.Any(state => !states.ContainsKey(state.Key) || states[state.Key] != state.Value);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return simpleState*1000000 + states.GetHashCode();
        }
    }
}
