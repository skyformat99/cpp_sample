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
    struct Event2 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct Init:msm::front::state<> {};
        struct State1_:msm::front::state_machine_def<State1_>
        {
            struct Entry1:msm::front::entry_pseudo_state<0> {};
            struct State1_1 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State1_1::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State1_1::on_exit()" << std::endl;
                }
            };
            typedef Entry1 initial_state;
            // Actions
            struct Action1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&)
                {
                    std::cout << "Action1()" << std::endl;
                }
            };
            // Transition table
            struct transition_table:mpl::vector<
                //          Start       Event       Next        Action      Guard
                msmf::Row < Entry1,     msmf::none, State1_1,   msmf::none, msmf::none >,
                msmf::Row < State1_1,   Event1,     msmf::none, Action1,    msmf::none >
            > {};
        };
        typedef msm::back::state_machine<State1_> State1;

        // Set initial state
        typedef Init initial_state;

        // Actions
        struct Action2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "Action2()" << std::endl;
            }
        };

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                Action      Guard

            msmf::Row < Init,   msmf::none, State1::entry_pt
                                            <State1_::Entry1>,  Action2,    msmf::none >,
            msmf::Row < State1, Event2,     msmf::none,         Action2,    msmf::none >
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
        std::cout << "> Send Event2" << std::endl;
        sm1.process_event(Event2());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// State1::on_entry()
// > Send Event1
// State1::on_exit()
// Action1()
// State1::on_entry()
// > Send Event2
// Action2()
