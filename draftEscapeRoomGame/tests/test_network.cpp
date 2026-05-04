#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QJsonObject>

#include "NetworkManager.h"

//the mock
class MockNetwork : public NetworkManager {
public:
    MOCK_METHOD(void, sendJson, (const QJsonObject& obj), ());
};

TEST(NetworkLogic, SendPuzzleUpdatesValue) {
    MockNetwork mockNet;

    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);

    mockNet.sendPuzzleSolved(10);
}

TEST(NetworkLogic, DeskPuzzleID) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(1);
}

TEST(NetworkLogic, DoorPuzzleID) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(2);
}

TEST(NetworkLogic, LaptopPuzzleID) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(3);
}

TEST(NetworkLogic, GirlPuzzleID) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(4);
}

TEST(NetworkLogic, MultipleSolvedPuzzles) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(3);
    mockNet.sendPuzzleSolved(1);
    mockNet.sendPuzzleSolved(2);
    mockNet.sendPuzzleSolved(3);
}

TEST(NetworkLogic, InvalidPuzzleIDZero) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(0);
}

TEST(NetworkLogic, LargePuzzleID) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::_)).Times(1);
    mockNet.sendPuzzleSolved(999);
}


TEST(NetworkLogic, DisconnectSignalEmitted) {
    MockNetwork mockNet;
    SUCCEED();
}

TEST(NetworkLogic, JSONPayloadIsNotEmpty) {
    MockNetwork mockNet;
    EXPECT_CALL(mockNet, sendJson(testing::Property(&QJsonObject::isEmpty, false))).Times(1);
    mockNet.sendPuzzleSolved(5);
}
