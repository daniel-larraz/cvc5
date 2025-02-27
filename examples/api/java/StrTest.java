import io.github.cvc5.*;

public class StrTest {
  public static void main(String args[]) throws CVC5ApiException {
    TermManager tm = new TermManager();

    Term str = tm.mkString("\\u{200cb}", true);
  
    String value = str.getStringValue();
    String expected = new String(Character.toChars(0x200CB));

    assert value.equals(expected);
    System.out.println("Value: " + value);
  }
}