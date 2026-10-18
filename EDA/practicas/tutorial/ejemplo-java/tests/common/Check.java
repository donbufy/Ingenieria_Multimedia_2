import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

/**
 * Mini-framework de pruebas para las prácticas en Java.
 *
 * Uso en una prueba:
 *
 *   public class test_A01_ejemplo {
 *       public static void main(String[] args) {
 *           Check.equals(0, new Counter().getValue(), "valor inicial");
 *           Check.end();   // imprime el resumen y fija el código de salida
 *       }
 *   }
 *
 * Una prueba pasa si todas las comprobaciones son correctas
 * (el proceso termina con código 0).
 */
public final class Check {

    private static int total = 0;
    private static int fallos = 0;

    private Check() { }

    /** Comprueba que expected y actual son iguales (usando equals). */
    public static void equals(Object expected, Object actual, String msg) {
        total++;
        boolean ok = (expected == null) ? actual == null : expected.equals(actual);
        if (!ok) {
            fallos++;
            System.out.println("ERROR: " + msg);
            System.out.println("  esperado: " + repr(expected));
            System.out.println("  obtenido: " + repr(actual));
        }
    }

    /** Comprueba que una condición es verdadera. */
    public static void isTrue(boolean cond, String msg) {
        total++;
        if (!cond) {
            fallos++;
            System.out.println("ERROR: " + msg);
            System.out.println("  la condición esperada es falsa");
        }
    }

    /** Comprueba que una condición es falsa. */
    public static void isFalse(boolean cond, String msg) {
        isTrue(!cond, msg);
    }

    /** Comprueba que la cadena actual contiene el fragmento esperado. */
    public static void contains(String fragmento, String actual, String msg) {
        total++;
        boolean ok = actual != null && fragmento != null && actual.contains(fragmento);
        if (!ok) {
            fallos++;
            System.out.println("ERROR: " + msg);
            System.out.println("  se esperaba encontrar: " + repr(fragmento));
            System.out.println("  en la salida:");
            System.out.println(indent(actual));
        }
    }

    /**
     * Ejecuta un fragmento de código capturando su salida estándar
     * y la devuelve como cadena. Útil para probar métodos main().
     */
    public static String captureOutput(Runnable r) {
        PrintStream original = System.out;
        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        try {
            System.setOut(new PrintStream(buffer));
            r.run();
        } finally {
            System.setOut(original);
        }
        return buffer.toString();
    }

    /** Imprime el resumen y termina con código 0 (todo OK) o 1 (fallos). */
    public static void end() {
        System.out.println("===============================================================================");
        System.out.println("comprobaciones: " + total + " | correctas: " + (total - fallos)
                + " | fallidas: " + fallos);
        System.out.println(fallos == 0 ? "Estado: SUCCESS!" : "Estado: FAILURE!");
        System.exit(fallos == 0 ? 0 : 1);
    }

    private static String repr(Object o) {
        if (o == null) return "null";
        if (o instanceof String) return "\"" + o + "\"";
        return o.toString();
    }

    private static String indent(String s) {
        if (s == null) return "    null";
        StringBuilder sb = new StringBuilder();
        for (String line : s.split("\n", -1)) {
            sb.append("    ").append(line).append("\n");
        }
        return sb.toString();
    }
}
