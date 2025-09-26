#include "NavigationModel.hpp"

NavigationModel::NavigationModel()
{
    Event::instance()->registerEvent(EVENT_CURRENT_TIME_UPDATED);
    Event::instance()->registerEvent(EVENT_EST_TIME_ARRIVAL_UPDATED);
    Event::instance()->registerEvent(EVENT_EST_DISTANCE_ARRIVAL_UPDATED);
    Event::instance()->registerEvent(EVENT_ARRIVING_TIME_UPDATED);
    Event::instance()->registerEvent(EVENT_NEXT_INSTRUCTION_UPDATED);
    Event::instance()->registerEvent(EVENT_REMAINING_DISTANCE_UPDATED);
}

void NavigationModel::setCurrentTime(const std::string &time)
{
    if (time != currentTime_m)
    {
        currentTime_m = time;
        Event::instance()->emit(EVENT_CURRENT_TIME_UPDATED, &currentTime_m);
    }
}

void NavigationModel::setEstTimeBeforeArrival(const std::string &eta)
{
    if (estTimeBeforeArrival_m != eta)
    {
        estTimeBeforeArrival_m = eta;
        Event::instance()->emit(EVENT_EST_TIME_ARRIVAL_UPDATED, &estTimeBeforeArrival_m);
    }
}

void NavigationModel::setEstDistanceBeforeArrival(const std::string &eda)
{
    if (estDistanceBeforeArrival_m != eda)
    {
        estDistanceBeforeArrival_m = eda;
        Event::instance()->emit(EVENT_EST_DISTANCE_ARRIVAL_UPDATED, &estDistanceBeforeArrival_m);
    }
}

void NavigationModel::setArrivingTime(const std::string &arrivingTime)
{
    if (arrivingTime_m != arrivingTime)
    {
        arrivingTime_m = arrivingTime;
        Event::instance()->emit(EVENT_ARRIVING_TIME_UPDATED, &arrivingTime_m);
    }
}

void NavigationModel::setNextInstruction(const std::string &instruction)
{
    if (nextInstruction_m != instruction)
    {
        nextInstruction_m = instruction;
        Event::instance()->emit(EVENT_NEXT_INSTRUCTION_UPDATED, &nextInstruction_m);
    }
}

void NavigationModel::setRemainingDistanceBeforeNextInstruction(const std::string &distance)
{
    if (remainingDistanceBeforeNextInstruction_m != distance)
    {
        remainingDistanceBeforeNextInstruction_m = distance;
        Event::instance()->emit(EVENT_REMAINING_DISTANCE_UPDATED, &remainingDistanceBeforeNextInstruction_m);
    }
}