#if !defined(ATM_AUTH_HPP)
#define ATM_AUTH_HPP

#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>


namespace Atm {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct AuthSuccess;
    struct AccountInfo {
        AccountInfo(int amount_):balance(amount_) {}
        int balance;
    };
	struct AuthSuccess:AccountInfo {
        AuthSuccess(AccountInfo const& info):AccountInfo(info) {}
    };
    struct AuthFail {};
    struct AuthTimeout {};
    struct FingerDetect {};
    struct CardDetect {};

    // ----- State machine
    struct Auth_:msm::front::state_machine_def<Auth_>
    {
        // States
        struct Entry       :msm::front::entry_pseudo_state<> {};
        struct ExitSuccess :msm::front::exit_pseudo_state<AccountInfo> {};
        struct ExitFail    :msm::front::exit_pseudo_state<msmf::none> {};
        struct WaitingFinger:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "[DBG] Enter WaitingFinger" << std::endl;
                std::cout << "Please place your finger on the sensor" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "[DBG] Exit  WaitingFinger" << std::endl;
			}
        };
        struct Checking:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "[DBG] Enter Checking" << std::endl;
                std::cout << "Now checking" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "[DBG] Exit  Checking" << std::endl;
			}
        };

        // Set initial state
        typedef Entry initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start          Event         Next           Action      Guard
            msmf::Row < Entry,         msmf::none,   WaitingFinger, msmf::none, msmf::none >,
            msmf::Row < WaitingFinger, FingerDetect, Checking,      msmf::none, msmf::none >,
            msmf::Row < WaitingFinger, AuthTimeout,  ExitFail,      msmf::none, msmf::none >,
            msmf::Row < Checking,      AuthSuccess,  ExitSuccess,   msmf::none, msmf::none >,
            msmf::Row < Checking,      AuthFail,     ExitFail,      msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Auth_> Auth;
}

#endif // ATM_AUTH_HPP
