class Main {
  public static void main(String[] args) {
    A a = new A();
    B b = new B();
    assert (!(a instanceof B));
    assert (b instanceof B);
  }
}
;

class A {}

class B extends A {}
