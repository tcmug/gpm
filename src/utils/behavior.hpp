#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include <vector>

class behavior_node;

class tick {

    public:

        std::vector <behavior_node*> stack;

        void push(behavior_node *pop);
        behavior_node *pop();

};


class behavior_node {

    public:

        enum STATE {
            READY,
            COMPLETED,
            RUNNING,
            FAILED,
        };

        behavior_node();

        virtual void reset();
        virtual STATE run();

        STATE get_state();
        virtual void print(int depth = 1);

    protected:

        STATE state;

};


class behavior_composite: public behavior_node {

    public:

        behavior_composite();
        void add(behavior_node* node);
        virtual void reset();
        virtual void print(int depth = 0);

    protected:
        std::vector <behavior_node*> children;

};


class behavior_selector_priority: public behavior_composite {

    public:

        behavior_selector_priority();
        virtual void reset();
        virtual STATE run();

};


class behavior_selector_sequence: public behavior_composite {

    public:

        behavior_selector_sequence();
        virtual void reset();
        virtual STATE run();

    protected:

        std::size_t last_active;

};


class behavior_selector_concurrent: public behavior_composite {

    protected:

        virtual STATE run();

};


class behavior_wait: public behavior_node {

    public:

        behavior_wait(int _steps);
        virtual void reset();
        virtual STATE run();

    protected:

        int steps;
        int to_go;

};


class behavior_fail_randomly: public behavior_node {

    public:

        behavior_fail_randomly();
        virtual STATE run();

};


class behavior_msg: public behavior_node {

    public:

        behavior_msg(std::string _msg);
        virtual STATE run();

    protected:

        std::string msg;

};


// class behavior_selector_loop: public behavior_node {
// };

// class behavior_selector_random: public behavior_node {
// };

// class behavior_selector_concurrent: public behavior_node {
// };

// class behavior_action: public behavior_node {
// };

// class behavior_condition: public behavior_node {
// };

#endif
