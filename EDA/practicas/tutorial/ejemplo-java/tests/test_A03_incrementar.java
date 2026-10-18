public class test_A03_incrementar {
    public static void main(String[] args) {
        Counter c = new Counter();
        c.increment();
        Check.equals(1, c.getValue(), "A03: increment debe sumar 1");
        Check.end();
    }
}
