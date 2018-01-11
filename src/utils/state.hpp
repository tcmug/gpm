#ifndef STATE_HPP
#define STATE_HPP

#include <stack>

class state {
    int x;
};

class state_stack {

    private:
        std::stack <state*> stack;

    public:

        void push(state *state) {
            stack.push_back(state);
        }

        state *pop() {
            stack.push_back(state);
        }

};

#endif
