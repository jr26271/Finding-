import org.sosy_lab.sv_benchmarks.Verifier;

public class Main {

    public static void main(String args[]) {
      final int N = Verifier.nondetInt();
      if(N > 60)
        assert N > 42;
  }

}