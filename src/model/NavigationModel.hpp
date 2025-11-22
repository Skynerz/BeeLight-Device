#ifndef NAVIGATION_MODEL_HPP
#define NAVIGATION_MODEL_HPP

#include <string>
#include "Event.hpp"

class NavigationModel
{
public:
    enum NavigationEvents : uint8_t
    {
        EVENT_CURRENT_TIME_UPDATED = 1,
        EVENT_EST_TIME_ARRIVAL_UPDATED,
        EVENT_EST_DISTANCE_ARRIVAL_UPDATED,
        EVENT_ARRIVING_TIME_UPDATED,
        EVENT_NEXT_INSTRUCTION_UPDATED,
        EVENT_REMAINING_DISTANCE_UPDATED,
        EVENT_NEXT_INSTRUCTION_ICON_UPDATED,
    };

    enum InstructionIcon : uint8_t
    {
        UNKNOWN = 0,
        DEPART,
        DESTINATION,
        DESTINATION_LEFT,
        DESTINATION_RIGHT,
        STRAIGHT,
        TURN_LEFT,
        TURN_RIGHT,
        TURN_KEEP_LEFT,
        TURN_KEEP_RIGHT,
        TURN_SLIGHT_LEFT,
        TURN_SLIGHT_RIGHT,
        TURN_SHARP_LEFT,
        TURN_SHARP_RIGHT,
        TURN_U_TURN_CLOCKWISE,
        TURN_U_TURN_COUNTERCLOCKWISE,
        MERGE_UNSPECIFIED,
        MERGE_LEFT,
        MERGE_RIGHT,
        FORK_LEFT,
        FORK_RIGHT,
        ON_RAMP_UNSPECIFIED,
        ON_RAMP_LEFT,
        ON_RAMP_RIGHT,
        ON_RAMP_KEEP_LEFT,
        ON_RAMP_KEEP_RIGHT,
        ON_RAMP_SLIGHT_LEFT,
        ON_RAMP_SLIGHT_RIGHT,
        ON_RAMP_SHARP_LEFT,
        ON_RAMP_SHARP_RIGHT,
        ON_RAMP_U_TURN_CLOCKWISE,
        ON_RAMP_U_TURN_COUNTERCLOCKWISE,
        OFF_RAMP_UNSPECIFIED,
        OFF_RAMP_LEFT,
        OFF_RAMP_RIGHT,
        OFF_RAMP_KEEP_LEFT,
        OFF_RAMP_KEEP_RIGHT,
        OFF_RAMP_SLIGHT_LEFT,
        OFF_RAMP_SLIGHT_RIGHT,
        OFF_RAMP_SHARP_LEFT,
        OFF_RAMP_SHARP_RIGHT,
        OFF_RAMP_U_TURN_CLOCKWISE,
        OFF_RAMP_U_TURN_COUNTERCLOCKWISE,
        ROUNDABOUT_CLOCKWISE,
        ROUNDABOUT_COUNTERCLOCKWISE,
        ROUNDABOUT_STRAIGHT_CLOCKWISE,
        ROUNDABOUT_STRAIGHT_COUNTERCLOCKWISE,
        ROUNDABOUT_LEFT_CLOCKWISE,
        ROUNDABOUT_LEFT_COUNTERCLOCKWISE,
        ROUNDABOUT_RIGHT_CLOCKWISE,
        ROUNDABOUT_RIGHT_COUNTERCLOCKWISE,
        ROUNDABOUT_SLIGHT_LEFT_CLOCKWISE,
        ROUNDABOUT_SLIGHT_LEFT_COUNTERCLOCKWISE,
        ROUNDABOUT_SLIGHT_RIGHT_CLOCKWISE,
        ROUNDABOUT_SLIGHT_RIGHT_COUNTERCLOCKWISE,
        ROUNDABOUT_SHARP_LEFT_CLOCKWISE,
        ROUNDABOUT_SHARP_LEFT_COUNTERCLOCKWISE,
        ROUNDABOUT_SHARP_RIGHT_CLOCKWISE,
        ROUNDABOUT_SHARP_RIGHT_COUNTERCLOCKWISE,
        ROUNDABOUT_U_TURN_CLOCKWISE,
        ROUNDABOUT_U_TURN_COUNTERCLOCKWISE,
        ROUNDABOUT_EXIT_CLOCKWISE,
        ROUNDABOUT_EXIT_COUNTERCLOCKWISE,
        FERRY_BOAT,
        FERRY_TRAIN,
        NAME_CHANGE,
        LAST_ICON, //no more after
    };

    static NavigationModel *instance()
    {
        static NavigationModel *instance = nullptr;
        if (instance == nullptr)
        {
            instance = new NavigationModel();
        }
        return instance;
    }

    void reset();

    void setCurrentTime(const std::string &time);
    void setEstTimeBeforeArrival(const std::string &eta);
    void setEstDistanceBeforeArrival(const std::string &eda);
    void setArrivingTime(const std::string &arrivingTime);
    void setNextInstruction(const std::string &instruction);
    void setRemainingDistanceBeforeNextInstruction(const std::string &distance);
    void setNextInstructionIcon(const InstructionIcon icon);

    const std::string &getCurrentTime() const
    {
        return currentTime_m;
    }

    const std::string &getEstTimeBeforeArrival() const
    {
        return estTimeBeforeArrival_m;
    }

    const std::string &getEstDistanceBeforeArrival() const
    {
        return estDistanceBeforeArrival_m;
    }

    const std::string &getArrivingTime() const
    {
        return arrivingTime_m;
    }

    const std::string &getNextInstruction() const
    {
        return nextInstruction_m;
    }

    const std::string &getRemainingDistanceBeforeNextInstruction() const
    {
        return remainingDistanceBeforeNextInstruction_m;
    }

    const InstructionIcon getNextInstructionIcon() const {
        return nextInstructionIcon_m;
    }

private:
    #define UNDEFINED std::string("--")
    NavigationModel();
    std::string currentTime_m{UNDEFINED};
    std::string estTimeBeforeArrival_m{UNDEFINED};
    std::string estDistanceBeforeArrival_m{UNDEFINED};
    std::string arrivingTime_m{UNDEFINED};
    std::string nextInstruction_m{UNDEFINED};
    std::string remainingDistanceBeforeNextInstruction_m{UNDEFINED};
    InstructionIcon nextInstructionIcon_m{UNKNOWN};
};

#endif // NAVIGATION_MODEL_HPP
