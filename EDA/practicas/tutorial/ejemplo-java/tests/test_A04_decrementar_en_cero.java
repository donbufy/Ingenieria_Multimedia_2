public class test_A04_decrementar_en_cero {
    public static void main(String[] args) {
        Counter c = new Counter(0);
        Check.isFalse(c.decrement(), "A04: decrement en 0 debe devolver false");
        Check.equals(0, c.getValue(), "A04: el valor no debe bajar de 0");
        Check.end();
    }
}
