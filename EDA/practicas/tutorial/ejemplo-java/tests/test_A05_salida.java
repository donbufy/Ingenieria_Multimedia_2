public class test_A05_salida {
    public static void main(String[] args) {
        Counter c = new Counter(5);
        Check.equals("Counter: 5", c.toString(), "A05: formato de toString");
        Check.end();
    }
}
