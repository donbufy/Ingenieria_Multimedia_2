public class test_A02_constructor_parametros {
    public static void main(String[] args) {
        Counter c = new Counter(5);
        Check.equals(5, c.getValue(), "A02: el constructor debe guardar el valor recibido");
        Check.end();
    }
}
