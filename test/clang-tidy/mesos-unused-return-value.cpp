// RUN: %check_clang_tidy %s alpha-mesos-unused-return-value %t

template <typename T>
struct Try {
  Try() = default;
  Try(Try &&) = default;
  Try(T) {}
  Try &operator=(const Try &) = default;
  ~Try() = default;
};

Try<int> f() {
  f();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 'f' returns a 'Try<int>' which should not be ignored [alpha-mesos-unused-return-value]

  Try<int> t;
  t = f();
  t = (f());
  t = Try<int>(f());
  return f();
}

struct F {
  static Try<int> f() {
    f();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 'f' returns a 'Try<int>' which should not be ignored [alpha-mesos-unused-return-value]
    return f();
  }
};

struct G {
  G() = default;
  ~G() = default;
  static Try<G> f() {
    f();
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 'f' returns a 'Try<G>' which should not be ignored [alpha-mesos-unused-return-value]
    return f();
  }
};

void g() {
  auto ff = []() -> Try<int> { return 0; };
  ff();
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 'operator()' returns a 'Try<int>' which should not be ignored [alpha-mesos-unused-return-value]
}

Try<int> j() {
  Try<int> foo = j();
  return j();
}

int k() {
    k();
    return k();
}
