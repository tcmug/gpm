
#include "behavior.hpp"

#include <iostream>

behavior_node::behavior_node(): state(READY) {
}


void behavior_node::print(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    switch (state) {
        case READY:
            std::cout << "READY";
        break;
        case COMPLETED:
            std::cout << "COMPLETED";
        break;
        case FAILED:
            std::cout << "FAILED";
        break;
        case RUNNING:
            std::cout << "RUNNING";
        break;
    }
    std::cout << std::endl;
}


void behavior_composite::print(int depth) {
    behavior_node::print(depth);
    for (auto child : children) {
        child->print(depth + 1);
    }
}

void behavior_node::reset() {
    if (state != RUNNING) {
        state = READY;
    }
}


behavior_node::STATE behavior_node::run() {
    return state;
}


behavior_node::STATE behavior_node::get_state() {
    return state;
}

behavior_composite::behavior_composite(): behavior_node() {
}

void behavior_composite::add(behavior_node* node) {
    children.push_back(node);
}

void behavior_composite::reset() {
    behavior_node::reset();
    for (auto child : children) {
        child->reset();
    }
}




behavior_selector_priority::behavior_selector_priority(): behavior_composite() {
}


void behavior_selector_priority::reset() {
    behavior_composite::reset();
}


behavior_node::STATE behavior_selector_priority::run() {

    state = FAILED;

    for (auto child : children) {
        STATE child_state = child->run();
        if (child_state != FAILED) {
            state = child_state;
            break;
        }
    }

    return state;

}




behavior_selector_sequence::behavior_selector_sequence(): behavior_composite() {
    last_active = 0;
}


void behavior_selector_sequence::reset() {
    behavior_composite::reset();
}


behavior_node::STATE behavior_selector_sequence::run() {

    state = COMPLETED;

    for (auto child : children) {
        STATE child_state = child->run();
        if (child_state != COMPLETED) {
            state = child_state;
            break;
        }
    }

    return state;
}



behavior_node::STATE behavior_selector_concurrent::run() {
    state = COMPLETED;
    for (auto child : children) {
        if (child->get_state() == COMPLETED) {
            continue;
        }
        STATE child_state = child->run();
        switch (child_state) {
            case RUNNING:
                state = RUNNING;
            break;
            case FAILED:
                state = child_state;
                return state;
        }
    }

    return state;
}


behavior_wait::behavior_wait(int _steps): steps(_steps), to_go(_steps) {

}


void behavior_wait::reset() {
    behavior_node::reset();
}


behavior_node::STATE behavior_wait::run() {
    if (to_go-- <= 0) {
        state = behavior_node::COMPLETED;
        std::cout << "< COMPLETED" << std::endl;
        //to_go = steps;
    }
    else {
        std::cout << "< RUNNING" << std::endl;
        state = behavior_node::RUNNING;
    }
    return state;
}


behavior_fail_randomly::behavior_fail_randomly(): behavior_node() {

}


behavior_node::STATE behavior_fail_randomly::run() {
    if (rand() > RAND_MAX / 2) {
        std::cout << "RANDOM COMPLETED" << std::endl;
        state = COMPLETED;
    }
    else {
        std::cout << "RANDOM FAIL" << std::endl;
        state = FAILED;
    }
    return state;
}


behavior_msg::behavior_msg(std::string _msg): msg(_msg), behavior_node() {
}


behavior_node::STATE behavior_msg::run() {
    std::cout << "< " << msg << std::endl;
    state = COMPLETED;
    return state;
}

