#include <gtest/gtest.h>

#define private public
#include "NavigationModel.hpp"

extern uint8_t registerEventCalledCount;
extern uint8_t emitCalledCount;
extern uint8_t connectCalledCount;

TEST(NavigationModel, instance)
{
    NavigationModel *model = NavigationModel::instance();
    EXPECT_TRUE(model != nullptr);
    NavigationModel *model2 = NavigationModel::instance();
    EXPECT_EQ(model2, model);
}

TEST(NavigationModel, constructor)
{
    registerEventCalledCount = 0;
    emitCalledCount = 0;
    connectCalledCount = 0;
    NavigationModel model;
    EXPECT_EQ(model.currentTime_m, UNDEFINED);
    EXPECT_EQ(model.estTimeBeforeArrival_m, UNDEFINED);
    EXPECT_EQ(model.estDistanceBeforeArrival_m, UNDEFINED);
    EXPECT_EQ(model.arrivingTime_m, UNDEFINED);
    EXPECT_EQ(model.nextInstruction_m, UNDEFINED);
    EXPECT_EQ(model.remainingDistanceBeforeNextInstruction_m, UNDEFINED);
    EXPECT_EQ(model.nextInstructionIcon_m, InstructionIcon::UNKNOWN);

    EXPECT_EQ(registerEventCalledCount, 7);
    EXPECT_EQ(emitCalledCount, 0);
    EXPECT_EQ(connectCalledCount, 0);
}

TEST(NavigationModel, reset)
{
    registerEventCalledCount = 0;
    emitCalledCount = 0;
    connectCalledCount = 0;
    NavigationModel model;
    model.setCurrentTime("plop");
    model.setEstTimeBeforeArrival("plop");
    model.setEstDistanceBeforeArrival("plop");
    model.setArrivingTime("plop");
    model.setNextInstruction("plop");
    model.setRemainingDistanceBeforeNextInstruction("plop");
    model.setNextInstructionIcon(InstructionIcon::DEPART);
    EXPECT_EQ(emitCalledCount, 7);
    model.reset();
    EXPECT_EQ(emitCalledCount, 14);
    EXPECT_EQ(model.currentTime_m, UNDEFINED);
    EXPECT_EQ(model.estTimeBeforeArrival_m, UNDEFINED);
    EXPECT_EQ(model.estDistanceBeforeArrival_m, UNDEFINED);
    EXPECT_EQ(model.arrivingTime_m, UNDEFINED);
    EXPECT_EQ(model.nextInstruction_m, UNDEFINED);
    EXPECT_EQ(model.remainingDistanceBeforeNextInstruction_m, UNDEFINED);
    EXPECT_EQ(model.nextInstructionIcon_m, InstructionIcon::UNKNOWN);
}

TEST(NavigationModel, CurrentTime)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string time = "plop";
    std::string time2 = "plop2";
    model.setCurrentTime(time);
    EXPECT_EQ(model.getCurrentTime(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setCurrentTime(time);
    EXPECT_EQ(model.getCurrentTime(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setCurrentTime(time2);
    EXPECT_EQ(model.getCurrentTime(), time2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, EstTimeBeforeArrival)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string time = "plop";
    std::string time2 = "plop2";
    model.setEstTimeBeforeArrival(time);
    EXPECT_EQ(model.getEstTimeBeforeArrival(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setEstTimeBeforeArrival(time);
    EXPECT_EQ(model.getEstTimeBeforeArrival(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setEstTimeBeforeArrival(time2);
    EXPECT_EQ(model.getEstTimeBeforeArrival(), time2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, EstDistanceBeforeArrival)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string distance = "plop";
    std::string distance2 = "plop2";
    model.setEstDistanceBeforeArrival(distance);
    EXPECT_EQ(model.getEstDistanceBeforeArrival(), distance);
    EXPECT_EQ(emitCalledCount, 1);
    model.setEstDistanceBeforeArrival(distance);
    EXPECT_EQ(model.getEstDistanceBeforeArrival(), distance);
    EXPECT_EQ(emitCalledCount, 1);
    model.setEstDistanceBeforeArrival(distance2);
    EXPECT_EQ(model.getEstDistanceBeforeArrival(), distance2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, ArrivingTime)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string time = "plop";
    std::string time2 = "plop2";
    model.setArrivingTime(time);
    EXPECT_EQ(model.getArrivingTime(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setArrivingTime(time);
    EXPECT_EQ(model.getArrivingTime(), time);
    EXPECT_EQ(emitCalledCount, 1);
    model.setArrivingTime(time2);
    EXPECT_EQ(model.getArrivingTime(), time2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, NextInstruction)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string instruction = "plop";
    std::string instruction2 = "plop2";
    model.setNextInstruction(instruction);
    EXPECT_EQ(model.getNextInstruction(), instruction);
    EXPECT_EQ(emitCalledCount, 1);
    model.setNextInstruction(instruction);
    EXPECT_EQ(model.getNextInstruction(), instruction);
    EXPECT_EQ(emitCalledCount, 1);
    model.setNextInstruction(instruction2);
    EXPECT_EQ(model.getNextInstruction(), instruction2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, RemainingDistanceBeforeNextInstruction)
{
    NavigationModel model;
    emitCalledCount = 0;
    std::string distance = "plop";
    std::string distance2 = "plop2";
    model.setRemainingDistanceBeforeNextInstruction(distance);
    EXPECT_EQ(model.getRemainingDistanceBeforeNextInstruction(), distance);
    EXPECT_EQ(emitCalledCount, 1);
    model.setRemainingDistanceBeforeNextInstruction(distance);
    EXPECT_EQ(model.getRemainingDistanceBeforeNextInstruction(), distance);
    EXPECT_EQ(emitCalledCount, 1);
    model.setRemainingDistanceBeforeNextInstruction(distance2);
    EXPECT_EQ(model.getRemainingDistanceBeforeNextInstruction(), distance2);
    EXPECT_EQ(emitCalledCount, 2);
}

TEST(NavigationModel, NextInstructionIcon)
{
    NavigationModel model;
    emitCalledCount = 0;
    InstructionIcon::Values instruction = InstructionIcon::Values::DEPART;
    InstructionIcon::Values instruction2 = InstructionIcon::Values::DESTINATION;
    model.setNextInstructionIcon(instruction);
    EXPECT_EQ(model.getNextInstructionIcon(), instruction);
    EXPECT_EQ(emitCalledCount, 1);
    model.setNextInstructionIcon(instruction);
    EXPECT_EQ(model.getNextInstructionIcon(), instruction);
    EXPECT_EQ(emitCalledCount, 1);
    model.setNextInstructionIcon(instruction2);
    EXPECT_EQ(model.getNextInstructionIcon(), instruction2);
    EXPECT_EQ(emitCalledCount, 2);
}
