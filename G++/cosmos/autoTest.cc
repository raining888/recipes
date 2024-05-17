void func(auto a = 1);  // error: auto不能用于函数参数

struct Foo {
  auto var1_ = 0;  // error: auto不能同于非静态成员变量
  static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main() {
  int arr[10] = {0};
  auto aa = arr;      // OK: aa -> int*
  auto rr[10] = arr;  // err: auto无法定义数组
  Bar<int> bar;
  Bar<auto> bb = bar;  // err: auto无法推导出模版参数

  return 0;
}
