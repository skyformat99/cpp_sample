#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/euml.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace msme = boost::msm::front::euml;
    namespace mpl = boost::mpl;

    // Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {

        struct State1_:msm::front::state_machine_def<State1_>
        {
            struct State1_1:msm::front::state<> {};
            struct State1_2:msm::front::state<> {};
            struct State1_3:msm::front::state<> {};
            struct State1_4:msm::front::state<> {};
            struct State1_5:msm::front::state<> {};
             // Guards
            struct Guard1_1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_1" << std::endl;
                    return false;
                }
            };
            struct Guard1_2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_2" << std::endl;
                    return true;
                }
            };
            struct Guard1_3 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_3" << std::endl;
                    return true;
                }
            };
            struct Guard1_4 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_4" << std::endl;
                    return false;
                }
            };
            // Actions
            struct Action1_2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Action1_2" << std::endl;
                }
            };
            struct Action1_3 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState& tst) const 
                {
//                  BOOST_STATIC_ASSERT((boost::is_same<TargetState, msmf::none>::value));
                    std::cout << "Action1_3" << std::endl;
                }
            };
            // Set initial state
            typedef State1_1 initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start     Event   Next        Action      Guard
                msmf::Row < State1_1, Event1, State1_1,   msmf::none, Guard1_1 >,
                msmf::Row < State1_1, Event1, msmf::none, Action1_2,  Guard1_2 >,
                msmf::Row < State1_1, Event1, msmf::none, Action1_3,  Guard1_3 >,
                msmf::Row < State1_1, Event1, State1_4,   msmf::none, Guard1_4 >
            > {};
        };
        // back-end
        typedef msm::back::state_machine<State1_> State1;
        struct State2:msm::front::state<> {};
        struct State3:msm::front::state<> {};
        struct State4:msm::front::state<> {};
        struct State5:msm::front::state<> {};

         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard1" << std::endl;
                return false;
            }
        };
        struct Guard2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard2" << std::endl;
                return true;
            }
        };
        struct Guard3 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard3" << std::endl;
                return true;
            }
        };
        struct Guard4 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard4" << std::endl;
                return false;
            }
        };

        // Actions
        struct Action2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action2" << std::endl;
            }
        };
        struct Action3 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action3" << std::endl;
            }
        };
        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next        Action      Guard
            msmf::Row < State1, Event1, State1,     msmf::none, Guard1 >,
            msmf::Row < State1, Event1, msmf::none, msmf::none, Guard2 >,
            msmf::Row < State1, Event1, msmf::none, Action3,    Guard3 >,
            msmf::Row < State1, Event1, State4,     msmf::none, Guard4 >
        > {};
    };

    // back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    // No handled event handler
    template <class Fsm,class Event> 
    void no_transition(Event const& e, Fsm& ,int state) {
        std::cout << "No handled event in Sm1 " << typeid(e).name() << " on State " << state << std::endl;
    }

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
// Guard1_1
// Guard1_2
// Action1_2
// Guard1_3
// Action1_3
// Guard1_4
// Guard1
// Guard2
// Guard3
// Action3
// Guard4
