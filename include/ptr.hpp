// Copyright 2021 LemuriiL <LemuriiL@yandex.ru>
#ifndef INCLUDE_PTR_HPP_
#define INCLUDE_PTR_HPP_

#include <atomic>
#include <utility>

template <class T>
class MySharedPtr {
 public:
  MySharedPtr() : user(nullptr), value(nullptr) {}

  MySharedPtr(T *ptr) : user(ptr), value(new std::atomic_uint(1)) {}

  MySharedPtr(const MySharedPtr &r) : user(r.user), value(r.value) {
    ++(*value);
  }

  MySharedPtr(MySharedPtr &&rr) : user(nullptr), value(nullptr) {
    std::swap(user, rr.user);
    std::swap(value, rr.value);
  }

  ~MySharedPtr() {
    if (!value) return;
    if (--(*value) == 0) {
      delete user;
      delete value;
    }
  }

  MySharedPtr &operator=(const MySharedPtr &r) {
    reset();
    user = r.user;
    value = r.value;
    ++(*value);
    return *this;
  }

  MySharedPtr &operator=(MySharedPtr &&rr) {
    reset();
    std::swap(user, rr.user);
    std::swap(value, rr.value);

    return *this;
  }

  explicit operator bool() const { return user != nullptr; }

  T &operator*() const { return *user; }

  T *operator->() const { return user; }

  T *get() { return user; }

  void reset() {
    if (!value) return;
    --(*value);
    if (*value == 0) {
      delete user;
      delete value;
    }
    user = nullptr;
    value = nullptr;
  }

  void reset(T *ptr) {
    reset();
    user = ptr;
    value = new std::atomic_uint(1);
  }

  std::size_t WhatTheValue() const {
    if (!value) return 0;
    return *value;
  }

 private:
  T *user;
  std::atomic_uint *value;
};

#endif  // INCLUDE_PTR_HPP_
