#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>


namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:msmf::state_machine_def<Sm1_>
    {
        struct State1_:msmf::state<> {};
        struct State2_:msmf::state_machine_def<State2_>
        {
            struct State2_1:msmf::state<>
            {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "State2_1::on_entry()" << std::endl;
                }
            };
            struct State2_2:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "State2_2::on_entry()" << std::endl;
                }
            };
            struct Entry1:msmf::entry_pseudo_state<1> {};
            // Actions
            struct Action1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&)
                {
                    std::cout << "Action1()" << std::endl;
                }
            };
            struct Action2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&)
                {
                    std::cout << "Action2()" << std::endl;
                }
            };
            // Set initial state
            typedef mpl::vector<State2_1, State2_2> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start     Event       Next        Action      Guard
                msmf::Row < Entry1,   msmf::none, State2_2,   msmf::none, msmf::none >,
                msmf::Row < State2_1, Event1,     msmf::none, Action1,    msmf::none >,
                msmf::Row < State2_2, Event1,     msmf::none, Action2,    msmf::none >
                > {};
        };
        typedef msm::back::state_machine<State2_> State2;

        // Set initial state
        typedef State1_ initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event   Next               Action      Guard
            msmf::Row < State1_, Event1, State2, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event1" << std::endl;
        sm1.process_event(Event1());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// > Send Event1
// State2_1::on_entry()
// State2_2::on_entry()
// Action1()
