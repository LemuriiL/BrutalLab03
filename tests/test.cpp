#include <gtest/gtest.h>

#include <ptr.hpp>
#include <type_traits>
#include <utility>

TEST(Shared_Ptr, Default) {
  MySharedPtr<int> ptr;
  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(ptr.WhatTheValue(), 0);
}

TEST(Shared_Ptr, Point) {
  int *ptr = new int(123);
  MySharedPtr<int> mysharedPtr(ptr);
  ASSERT_EQ(mysharedPtr.get(), ptr);
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
}

TEST(Shared_Ptr, Copy) {
  MySharedPtr<int> ptr(new int(123));
  MySharedPtr<int> ptr2(ptr);
  ASSERT_EQ(ptr.get(), ptr2.get());
  ASSERT_EQ(ptr.WhatTheValue(), ptr2.WhatTheValue());
  ASSERT_EQ(ptr.WhatTheValue(), 2);
}

TEST(Shared_Ptr, Move) {
  MySharedPtr<int> ptr(new int(123));

  auto tmp = ptr.get();
  ASSERT_TRUE(std::is_move_constructible<MySharedPtr<int>>::value);
  MySharedPtr<int> ptr2(std::move(ptr));

  ASSERT_EQ(ptr.get(), nullptr);
  ASSERT_EQ(ptr.WhatTheValue(), 0);
  ASSERT_EQ(ptr2.WhatTheValue(), 1);
  ASSERT_EQ(ptr2.get(), tmp);
}

TEST(Shared_Ptr, Destructor) {
  int *ptr = new int(1234);
  MySharedPtr<int> mysharedPtr(ptr);
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
  {
    MySharedPtr<int> sharedPtr1(mysharedPtr);
    ASSERT_EQ(sharedPtr1.WhatTheValue(), 2);
    ASSERT_EQ(sharedPtr1.WhatTheValue(), mysharedPtr.WhatTheValue());
  }
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
}

TEST(Shared_Ptr, CopyOperate) {
  int *ptr = new int(1234);
  int *ptr2 = new int(5678);

  MySharedPtr<int> mysharedPtr(ptr);
  MySharedPtr<int> mysharedPtr1(mysharedPtr);

  ASSERT_EQ(mysharedPtr.WhatTheValue(), mysharedPtr1.WhatTheValue());
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 2);
  ASSERT_EQ(mysharedPtr.get(), mysharedPtr1.get());

  MySharedPtr<int> mysharedPtr2(ptr2);
  mysharedPtr1 = mysharedPtr2;

  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
  ASSERT_EQ(mysharedPtr1.WhatTheValue(), mysharedPtr2.WhatTheValue());
  ASSERT_EQ(mysharedPtr1.WhatTheValue(), 2);
  ASSERT_EQ(mysharedPtr1.get(), mysharedPtr2.get());
  ASSERT_EQ(mysharedPtr1.get(), ptr2);
}

TEST(Shared_Ptr, MoveOperate) {
  int *ptr = new int(1234);
  int *ptr2 = new int(5678);

  MySharedPtr<int> mysharedPtr(ptr);
  MySharedPtr<int> mysharedPtr1(mysharedPtr);

  ASSERT_EQ(mysharedPtr.WhatTheValue(), mysharedPtr1.WhatTheValue());
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 2);
  ASSERT_EQ(mysharedPtr.get(), mysharedPtr1.get());

  MySharedPtr<int> mysharedPtr2(ptr2);
  ASSERT_TRUE(std::is_move_assignable<MySharedPtr<int>>::value);
  mysharedPtr1 = std::move(mysharedPtr2);

  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
  ASSERT_EQ(mysharedPtr1.WhatTheValue(), 1);
  ASSERT_EQ(mysharedPtr1.get(), ptr2);

  ASSERT_FALSE(mysharedPtr2);
  ASSERT_EQ(mysharedPtr2.WhatTheValue(), 0);
}

TEST(Shared_Ptr, Reset1) {
  MySharedPtr<int> mysharedPtr(new int(1234));
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 1);
  ASSERT_TRUE(mysharedPtr);

  mysharedPtr.reset();
  ASSERT_FALSE(mysharedPtr);
  ASSERT_EQ(mysharedPtr.WhatTheValue(), 0);
}

TEST(Shared_Ptr, Reset2) {
  int *ptr = new int(123);
  int *ptr2 = new int(456);

  MySharedPtr<int> mysharedPtr1(ptr);
  MySharedPtr<int> mysharedPtr2(mysharedPtr1);

  ASSERT_EQ(mysharedPtr1.get(), ptr);
  ASSERT_EQ(mysharedPtr2.get(), ptr);
  ASSERT_EQ(mysharedPtr2.WhatTheValue(), 2);
  ASSERT_EQ(mysharedPtr1.WhatTheValue(), 2);

  mysharedPtr2.reset(ptr2);
  ASSERT_EQ(mysharedPtr2.get(), ptr2);
  ASSERT_EQ(mysharedPtr1.WhatTheValue(), 1);
  ASSERT_EQ(mysharedPtr2.WhatTheValue(), 1);
}