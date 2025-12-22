#ifndef NAVIGATION_MODEL_HPP
#define NAVIGATION_MODEL_HPP

#include <string>
#include "Event.hpp"
#include "InstructionIcon.hpp"

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
    void setNextInstructionIcon(const InstructionIcon::Values icon);

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

    const InstructionIcon::Values getNextInstructionIcon() const
    {
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
    InstructionIcon::Values nextInstructionIcon_m{InstructionIcon::Values::UNKNOWN};
};

#endif // NAVIGATION_MODEL_HPP
