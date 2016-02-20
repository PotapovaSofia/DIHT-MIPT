#ifndef GAUSS_ATOMIC_WRAPPER_H
#define GAUSS_ATOMIC_WRAPPER_H

#include <atomic>

template <typename T>
struct atomwrapper
{
  std::atomic<T> a;

  atomwrapper()
    :a()
  {}

  atomwrapper(const std::atomic<T> &a_)
    :a(a_.load())
  {}

  atomwrapper(const atomwrapper &other)
    :a(other.a.load())
  {}

  atomwrapper &operator=(const atomwrapper &other)
  {
    a.store(other.a.load());
  }

  T load() {
    return a.load();
  }

  void store(T x) {
    a.store(x);
  }
};

#endif //GAUSS_ATOMIC_WRAPPER_H
