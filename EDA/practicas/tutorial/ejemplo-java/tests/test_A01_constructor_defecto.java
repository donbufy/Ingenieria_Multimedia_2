public class test_A01_constructor_defecto {
    public static void main(String[] args) {
        Counter c = new Counter();
        Check.equals(0, c.getValue(), "A01: el valor inicial debe ser 0");
        Check.end();
    }
}
