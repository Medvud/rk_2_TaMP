#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "DataContainers.h"
#include "database.h"
#include "Proxy.h"

using ::testing::Return;
using ::testing::_;

class MockDataContainer : public DataContainer {
public:
    MOCK_METHOD(std::list<std::string>, GetList, (), (override));
    MOCK_METHOD(void, Append, (const std::string& data), (override));
    MOCK_METHOD(void, Truncate, (), (override));
};


class DummyContainer : public DataContainer {
    std::list<std::string> _list;
public:
    void Append(const std::string& data) override {
        _list.push_back(data);
    }
    std::list<std::string> GetList() override {
        return _list;
    }
    void Truncate() override {
        _list.clear();
    }
};

class MockDataBase : public DataBase {
public:
    MOCK_METHOD(void, Append, (const std::string& data), (override));
    MOCK_METHOD(std::list<std::string>, GetList, (), (override));
    MOCK_METHOD(void, Truncate, (), (override));
};

TEST(DataContainerTest, AppendAndGetList) {
    DummyContainer dc;
    dc.Append("one");
    dc.Append("two");

    auto list = dc.GetList();
    ASSERT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), "one");
    EXPECT_EQ(list.back(), "two");
}

TEST(DataContainerTest, TruncateClearsList) {
    DummyContainer dc;
    dc.Append("temp");
    dc.Truncate();

    auto list = dc.GetList();
    EXPECT_TRUE(list.empty());
}

TEST(DataBaseTest, AppendAndGetList) {
    DataBase db;

    db.Append("first");
    db.Append("second");

    auto list = db.GetList();
    ASSERT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), "first");
    EXPECT_EQ(list.back(), "second");
}

TEST(DataBaseTest, TruncateClearsList) {
    DataBase db;

    db.Append("temp");
    db.Truncate();

    auto list = db.GetList();
    EXPECT_TRUE(list.empty());
}

TEST(ProxyTest, AppendWithoutLoginDoesNothing) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);

    EXPECT_CALL(mockDb, Append(_)).Times(0);
    proxy.Append("test");
}

TEST(ProxyTest, AppendAfterLoginCallsDatabase) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);
    proxy.Login("user", "pass");

    EXPECT_CALL(mockDb, Append("test")).Times(1);
    proxy.Append("test");
}

TEST(ProxyTest, TruncateWithoutLoginDoesNothing) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);

    EXPECT_CALL(mockDb, Truncate()).Times(0);
    proxy.Truncate();
}

TEST(ProxyTest, TruncateAfterLoginCallsDatabase) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);
    proxy.Login("user", "pass");

    EXPECT_CALL(mockDb, Truncate()).Times(1);
    proxy.Truncate();
}

TEST(ProxyTest, GetListWithoutLoginReturnsEmpty) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);

    auto list = proxy.GetList();
    EXPECT_TRUE(list.empty());
}

TEST(ProxyTest, GetListAfterLoginReturnsData) {
    MockDataBase mockDb;
    Proxy proxy(&mockDb);
    proxy.Login("user", "pass");

    EXPECT_CALL(mockDb, GetList())
        .Times(1)
        .WillOnce(::testing::Return(std::list<std::string>{"a", "b"}));

    auto list = proxy.GetList();
    ASSERT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), "a");
    EXPECT_EQ(list.back(), "b");
}

int main(int argc, char** argv) {
    ::testing::FLAGS_gmock_verbose = "error";
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}