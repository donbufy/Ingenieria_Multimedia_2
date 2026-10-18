/**
 * Clase de ejemplo del tutorial (versión Java).
 * Contiene un error deliberado que deberás corregir
 * siguiendo los pasos del tutorial.
 */
public class Counter {

    private int value;

    public Counter() {
        this.value = 0;
    }

    public Counter(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public void increment() {
        value++;
    }

    public boolean decrement() {
        // BUG: no comprueba si value es 0
        value--;
        return true;
    }

    @Override
    public String toString() {
        return "Counter: " + value;
    }
}
