class A {
  public void f() {}
}
;

class B extends A {
  public void f() {
    assert false;
  }
}
;

class Main {
  public static void main(String[] args) {
    A a = new A();
    B b = new B();
    a.f();
  }
}

