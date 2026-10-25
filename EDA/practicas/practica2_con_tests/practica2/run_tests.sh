#!/bin/bash
# ==========================================================
# run_tests.sh — Compila y ejecuta las pruebas (versión 2)
#
# Uso:
#   ./run_tests.sh                  Ejecuta todas las pruebas
#   ./run_tests.sh A                Ejecuta todas las pruebas del grupo A
#   ./run_tests.sh A01              Ejecuta solo la prueba A01
#   ./run_tests.sh Z                Ejecuta las pruebas del alumno (grupo Z)
#   ./run_tests.sh X                Ejecuta las pruebas del examen (grupo X)
#                                   y calcula la nota sobre ellas
#   ./run_tests.sh --src=DIR        Usa la carpeta DIR en lugar de src/ para
#                                   el código a probar (combinable con el resto)
#   ./run_tests.sh X --eval         Igual que lo anterior, pero sin barra de
#                                   progreso: el informe se guarda en
#                                   informe.txt y la nota (solo el número)
#                                   en nota.txt
#
# Los grupos se leen de tests/tests.csv (formato: grupo;descripcion).
# El grupo Z contiene pruebas creadas por el alumno. No se ejecuta
# por defecto ni cuenta para la nota.
# La nota solo se calcula al ejecutar exactamente el grupo X (examen);
# el denominador es el número de pruebas test_X* encontradas, sin
# necesidad de declarar un total en tests.csv.
#
# Novedades de la versión 2:
#   - Soporte para pruebas en Java (ficheros tests/test_*.java)
#   - Límite de tiempo por prueba (resultado TIMEOUT)
#   - Detección de fallos de ejecución (resultado CRASH)
#   - Nota calculada sobre las pruebas del grupo X (examen)
#   - Verificación de integridad de las pruebas (tests/.checksums)
# ==========================================================

cd "$(dirname "$0")"
make clean

set -u

SRC_DIR="src"
FILTRO=""
EVAL_MODE=0
INFORME_FILE="informe.txt"
NOTA_FILE="nota.txt"
TIMEOUT_S="${TEST_TIMEOUT:-10}"

for arg in "$@"; do
    case "$arg" in
        --eval) EVAL_MODE=1 ;;
        --src=*) SRC_DIR="${arg#--src=}" ;;
        *)      FILTRO="$arg" ;;
    esac
done

# En modo --eval se parte de cero: si queda un informe.txt o nota.txt
# de una ejecución anterior (p. ej. de otro grupo), no debe confundirse
# con los de esta ejecución.
if [ "$EVAL_MODE" -eq 1 ]; then
    rm -f "$INFORME_FILE" "$NOTA_FILE"
fi

# Colores (desactivados si no hay terminal o si se vuelca a fichero con --eval)
if [ -t 1 ] && [ "$EVAL_MODE" -ne 1 ]; then
    VERDE='\033[0;32m'
    ROJO='\033[0;31m'
    MORADO='\033[0;35m'
    AMARILLO='\033[0;33m'
    CIAN='\033[0;36m'
    AZUL='\033[0;34m'
    NC='\033[0m'
else
    VERDE=''
    ROJO=''
    MORADO=''
    AMARILLO=''
    CIAN=''
    AZUL=''
    NC=''
fi

CXX="g++"
CXXFLAGS="-std=c++11 -Wall -Wextra -g -O0"
JAVAC="${JAVAC:-javac}"
JAVA="${JAVA:-java}"
TEST_DIR="tests"
OUT_DIR="tests/output"
BIN_DIR="tests/output/bin"
OBJ_DIR="tests/output/obj"
CLASSES_DIR="tests/output/classes"
DESC_FILE="$TEST_DIR/tests.csv"

mkdir -p "$OUT_DIR" "$BIN_DIR" "$OBJ_DIR"

# ----------------------------------------------------------
# Detección de valgrind (comprobación de errores de memoria)
#   Si no está instalado se avisa y se espera a que el usuario
#   pulse Enter antes de continuar sin esta comprobación.
# ----------------------------------------------------------
VALGRIND_TIMEOUT=$((TIMEOUT_S * 4))
if command -v valgrind >/dev/null 2>&1; then
    VALGRIND_OK=1
else
    VALGRIND_OK=0
    echo ""
    echo "  AVISO: valgrind no está instalado. No se comprobarán errores de memoria."
    echo "  Instálalo (p. ej. 'sudo apt install valgrind') para obtener esa comprobación."
    echo ""
    read -r -p "  Pulsa Enter para continuar sin comprobación de memoria..." _
fi

declare -A MEMCHECK

# ----------------------------------------------------------
# Precompilación de doctest.h
#   Todas las pruebas .cc empiezan por:
#     #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#     #include "common/doctest.h"
#   tests/common/doctest_pch.h reproduce ese estado para poder
#   precompilarlo una sola vez en doctest_pch.h.gch. Al compilar
#   cada prueba se fuerza su inclusión con -include; el guard de
#   doctest.h hace que su propio #include sea un no-op, pero
#   gracias al .gch el compilador no vuelve a parsear ni generar
#   código para las ~7000 líneas de la librería en cada prueba.
# ----------------------------------------------------------
DOCTEST_PCH="$TEST_DIR/common/doctest_pch.h"
DOCTEST_GCH="$DOCTEST_PCH.gch"

if [ ! -f "$DOCTEST_GCH" ] || [ "$DOCTEST_PCH" -nt "$DOCTEST_GCH" ] \
   || [ "$TEST_DIR/common/doctest.h" -nt "$DOCTEST_GCH" ]; then
    $CXX $CXXFLAGS -I "$SRC_DIR" -I "$TEST_DIR" -x c++-header \
        "$DOCTEST_PCH" -o "$DOCTEST_GCH" >/dev/null 2>&1 || rm -f "$DOCTEST_GCH"
fi

# ----------------------------------------------------------
# Verificación de integridad de las pruebas (opcional)
# Si existe tests/.checksums se comprueba que las pruebas no
# han sido modificadas. Se usa durante los exámenes.
# ----------------------------------------------------------
if [ -f "$TEST_DIR/.checksums" ]; then
    if ! (cd "$TEST_DIR" && sha256sum --quiet -c .checksums >/dev/null 2>&1); then
        echo ""
        echo "  ERROR: las pruebas han sido modificadas o falta algún fichero."
        echo "  Restaura la carpeta tests/ original antes de continuar."
        echo ""
        exit 2
    fi
fi

# ----------------------------------------------------------
# Leer grupos y total declarado desde tests.csv
# ----------------------------------------------------------
if [ ! -f "$DESC_FILE" ]; then
    echo ""
    echo "  Error: no se encontró $DESC_FILE"
    echo ""
    exit 1
fi

GRUPOS_LETRA=()
GRUPOS_DESC=()

while IFS=';' read -r letra desc; do
    [[ -z "$letra" || "$letra" == \#* ]] && continue
    GRUPOS_LETRA+=("$letra")
    GRUPOS_DESC+=("$desc")
done < "$DESC_FILE"

if [ ${#GRUPOS_LETRA[@]} -eq 0 ]; then
    echo ""
    echo "  Error: $DESC_FILE no contiene ningún grupo"
    echo ""
    exit 1
fi

# ----------------------------------------------------------
# Recopilar ficheros de prueba según filtro (.cc y .java)
# ----------------------------------------------------------
total=0
pass=0
fail=0
error=0
crash=0
tout=0
mem_err=0

resultados=()
ficheros=()

if [ -n "$FILTRO" ]; then
    patron=$(echo "$FILTRO" | tr '[:lower:]' '[:upper:]')

    letras_validas=$(printf '%s' "${GRUPOS_LETRA[@]}")
    if [[ "$patron" =~ ^([$letras_validas])([0-9])$ ]]; then
        patron="${BASH_REMATCH[1]}0${BASH_REMATCH[2]}"
    fi

    encontrado=0

    for f in "$TEST_DIR"/test_${patron}_*.cc "$TEST_DIR"/test_${patron}.cc \
             "$TEST_DIR"/test_${patron}_*.java "$TEST_DIR"/test_${patron}.java; do
        [ -f "$f" ] || continue
        ficheros+=("$f")
        encontrado=1
    done

    if [ $encontrado -eq 0 ]; then
        for f in "$TEST_DIR"/test_${patron}*.cc "$TEST_DIR"/test_${patron}*.java; do
            [ -f "$f" ] || continue
            ficheros+=("$f")
            encontrado=1
        done
    fi

    if [ $encontrado -eq 0 ]; then
        echo ""
        echo "  No se encontró ninguna prueba que coincida con '$FILTRO'"
        echo "  Ejemplo de uso: ./run_tests.sh A (grupo), ./run_tests.sh A01 (individual)"
        echo ""
        exit 1
    fi
else
    for ((i=0; i<${#GRUPOS_LETRA[@]}; i++)); do
        letra="${GRUPOS_LETRA[$i]}"
        [ "$letra" = "Z" ] && continue
        for f in "$TEST_DIR"/test_${letra}*.cc "$TEST_DIR"/test_${letra}*.java; do
            [ -f "$f" ] && ficheros+=("$f")
        done
    done
fi

total_esperado=${#ficheros[@]}
progreso_actual=0

# ----------------------------------------------------------
# mostrar_progreso <nombre_prueba>
# ----------------------------------------------------------
previous_length=0
mostrar_progreso() {
    [ "$EVAL_MODE" -eq 1 ] && return
    local nombre="$1"
    progreso_actual=$((progreso_actual + 1))
    local pct=0
    if [ $total_esperado -gt 0 ]; then
        pct=$((progreso_actual * 100 / total_esperado))
    fi
    local barra_len=30
    local lleno=$((pct * barra_len / 100))
    local vacio=$((barra_len - lleno))
    local barra=""
    local espacios=""
    [ $lleno -gt 0 ] && barra=$(printf '%0.s█' $(seq 1 $lleno 2>/dev/null))
    [ $vacio -gt 0 ] && espacios=$(printf '%0.s░' $(seq 1 $vacio 2>/dev/null))
    printf '\r%*s' "$previous_length" ''
    line=$(printf '\r  [%s%s] %3d%% (%d/%d) %s   ' "$barra" "$espacios" "$pct" "$progreso_actual" "$total_esperado" "$nombre")
    previous_length=${#line}
    printf '\r%s' "$line"
}

# ----------------------------------------------------------
# registrar_resultado <rc> <nombre> <salida>
#   Clasifica el resultado de la ejecución de una prueba:
#   0 = PASS, 124 = TIMEOUT, >=126 = CRASH, resto = FAIL
# ----------------------------------------------------------
registrar_resultado() {
    local rc="$1"
    local nombre="$2"
    local salida="$3"

    if [ "$rc" -eq 0 ]; then
        resultados+=("PASS|$nombre")
        pass=$((pass + 1))
    elif [ "$rc" -eq 124 ]; then
        {
            echo "La prueba superó el límite de tiempo (${TIMEOUT_S}s)."
            echo "Posibles causas: bucle infinito, espera de entrada que no llega"
            echo "o un algoritmo mucho más lento de lo esperado."
        } >> "$salida"
        resultados+=("TIMEOUT|$nombre")
        tout=$((tout + 1))
    elif [ "$rc" -ge 126 ]; then
        {
            echo ""
            echo "El programa terminó de forma abrupta (código $rc)."
            echo "Posibles causas: acceso a memoria inválida (segmentation fault),"
            echo "doble liberación de memoria o uso de punteros no inicializados."
        } >> "$salida"
        resultados+=("CRASH|$nombre")
        crash=$((crash + 1))
    else
        resultados+=("FAIL|$nombre")
        fail=$((fail + 1))
    fi
}

# ----------------------------------------------------------
# comprobar_memoria <binario> <nombre>
#   Ejecuta el binario bajo valgrind y clasifica el resultado en
#   MEMCHECK[nombre]: OK, FAIL (errores detectados) o TIMEOUT.
# ----------------------------------------------------------
comprobar_memoria() {
    local binario="$1"
    local nombre="$2"
    local vglog="$OUT_DIR/${nombre}.valgrind.txt"

    # Se limpian logs de una ejecución anterior: --trace-children=yes genera
    # un fichero por proceso (uno por PID), y los PID cambian en cada run.
    rm -f "$vglog" "$vglog".*

    # --trace-children=yes es necesario porque algunos tests lanzan el
    # binario bajo prueba (p. ej. ./catalog) con system(); sin esta opción
    # Valgrind solo instrumenta el proceso del test y ese binario corre sin
    # comprobar. Como varios procesos (test, shell, binario) escriben logs
    # a la vez, se usa "%p" para que cada uno tenga su propio fichero en
    # vez de compartir uno solo (la salida se intercalaría y rompería el
    # grep de más abajo).
    { timeout "$VALGRIND_TIMEOUT" valgrind --leak-check=full --error-exitcode=99 \
        --trace-children=yes --log-file="$vglog.%p" "$binario" \
        --no-intro=true --no-version=true \
        >/dev/null 2>&1; } 2>/dev/null
    local vg_rc=$?

    if [ $vg_rc -eq 124 ]; then
        MEMCHECK["$nombre"]="TIMEOUT"
        return
    fi

    # El exit code de valgrind no refleja errores detectados en los hijos
    # cuando se usa --trace-children=yes, así que se agregan los logs de
    # todos los procesos (test + shell + binario lanzado con system()).
    # Los logs sin errores se descartan para no llenar tests/output/ con
    # un fichero por cada proceso (test, shell, binario) de cada test.
    local errores_total=0
    local logf errores
    for logf in "$vglog".*; do
        [ -f "$logf" ] || continue
        errores=$(grep -oE 'ERROR SUMMARY: [0-9]+' "$logf" | grep -oE '[0-9]+')
        if [ "${errores:-0}" -eq 0 ]; then
            rm -f "$logf"
        else
            errores_total=$((errores_total + errores))
        fi
    done

    if [ "$errores_total" -eq 0 ]; then
        MEMCHECK["$nombre"]="OK"
    else
        MEMCHECK["$nombre"]="FAIL"
    fi
}

# ----------------------------------------------------------
# compilar_objeto_cc <src.cc>
#   Compila un .cc de src/ a su .o en $OBJ_DIR, reutilizando el
#   objeto si ya está actualizado. La frescura se determina con
#   ficheros de dependencias (-MMD -MP), que incluyen tanto el
#   .cc como las cabeceras que arrastra, así que un cambio en un
#   .h también invalida el objeto correspondiente.
#   Salida: ruta del .o por stdout (éxito) o el log de error
#   (fallo). Código de retorno 0/1.
# ----------------------------------------------------------
compilar_objeto_cc() {
    local cc="$1"
    local base
    # Nombre del objeto derivado de la ruta relativa a $SRC_DIR (con / -> _),
    # para que dos ficheros con el mismo nombre en carpetas distintas
    # (p. ej. D01_x/Persona.cc y D05_y/Persona.cc) no compartan .o.
    base="${cc#"$SRC_DIR"/}"
    base="${base%.cc}"
    base="${base//\//_}"
    local obj="$OBJ_DIR/$base.o"
    local dep="$OBJ_DIR/$base.d"

    local necesita=1
    if [ -f "$obj" ] && [ -f "$dep" ]; then
        necesita=0
        while IFS= read -r d; do
            [ -z "$d" ] && continue
            if [ "$d" -nt "$obj" ]; then
                necesita=1
                break
            fi
        done < <(sed -e 's/^[^:]*: //' -e 's/\\$//' "$dep" | tr -s ' \t' '\n' | sed '/^$/d')
    fi

    if [ $necesita -eq 1 ]; then
        local out
        out=$($CXX $CXXFLAGS -MMD -MP -I "$SRC_DIR" -I "$TEST_DIR" -c "$cc" -o "$obj" 2>&1)
        if [ $? -ne 0 ]; then
            echo "$out"
            return 1
        fi
    fi

    echo "$obj"
    return 0
}

# ----------------------------------------------------------
# compilar_y_ejecutar_cc <test_src> <extra_srcs>
# ----------------------------------------------------------
compilar_y_ejecutar_cc() {
    local test_src="$1"
    local extra_srcs="$2"

    local nombre
    nombre=$(basename "$test_src" .cc)
    local binario="$BIN_DIR/$nombre"
    local salida="$OUT_DIR/${nombre}.test.txt"

    total=$((total + 1))

    local objs=()
    local compile_out=""
    local compile_rc=0

    if [ -n "$extra_srcs" ]; then
        for cc in $extra_srcs; do
            local obj_out
            obj_out=$(compilar_objeto_cc "$cc")
            if [ $? -ne 0 ]; then
                compile_out="$obj_out"
                compile_rc=1
                break
            fi
            objs+=("$obj_out")
        done
    fi

    local pch_flag=()
    [ -f "$DOCTEST_GCH" ] && pch_flag=(-include "$DOCTEST_PCH")

    if [ $compile_rc -eq 0 ]; then
        compile_out=$($CXX $CXXFLAGS -I "$SRC_DIR" -I "$TEST_DIR" "${pch_flag[@]+"${pch_flag[@]}"}" "$test_src" "${objs[@]+"${objs[@]}"}" -o "$binario" 2>&1)
        compile_rc=$?
    fi

    if [ $compile_rc -ne 0 ]; then
        echo "$compile_out" > "$salida"
        resultados+=("ERROR|$nombre")
        error=$((error + 1))
        MEMCHECK["$nombre"]="N/A"
        mostrar_progreso "$nombre"
        return
    fi

    # El informe de doctest va a $salida (--out); stdout del código bajo
    # prueba va a .cout.txt; stderr se añade al final del informe.
    local cout="$OUT_DIR/${nombre}.cout.txt"
    local cerr="$OUT_DIR/${nombre}.cerr.tmp"
    : > "$salida"
    timeout "$TIMEOUT_S" "$binario" --no-intro=true --no-version=true \
        --out="$salida" >"$cout" 2>"$cerr"
    local test_rc=$?
    [ -s "$cerr" ] && cat "$cerr" >> "$salida"
    rm -f "$cerr"
    registrar_resultado "$test_rc" "$nombre" "$salida"

    if [ $VALGRIND_OK -eq 1 ]; then
        comprobar_memoria "$binario" "$nombre"
        case "${MEMCHECK[$nombre]}" in
            FAIL|TIMEOUT)
                mem_err=$((mem_err + 1))
                # Un error de memoria invalida la prueba aunque el
                # resultado funcional haya sido PASS.
                if [ "$test_rc" -eq 0 ]; then
                    pass=$((pass - 1))
                    fail=$((fail + 1))
                fi
                ;;
        esac
    fi

    mostrar_progreso "$nombre"
}

# ----------------------------------------------------------
# Compilación (una sola vez) de las clases Java de src/ y
# tests/common/ en tests/output/classes
# ----------------------------------------------------------
java_src_done=0
java_src_ok=0
java_src_out=""
java_src_recompiled=0
JAVA_SRC_STAMP="$CLASSES_DIR/.src_stamp"

compilar_java_src() {
    [ $java_src_done -eq 1 ] && return
    java_src_done=1
    mkdir -p "$CLASSES_DIR"
    local fuentes=()
    for f in "$SRC_DIR"/*.java "$TEST_DIR"/common/*.java; do
        [ -f "$f" ] && fuentes+=("$f")
    done

    if [ ${#fuentes[@]} -eq 0 ]; then
        java_src_ok=1
        return
    fi

    # Reutiliza las clases ya compiladas si ninguna fuente es más
    # reciente que la última compilación (marca en $JAVA_SRC_STAMP).
    local necesita=1
    if [ -f "$JAVA_SRC_STAMP" ]; then
        necesita=0
        for f in "${fuentes[@]}"; do
            if [ "$f" -nt "$JAVA_SRC_STAMP" ]; then
                necesita=1
                break
            fi
        done
    fi

    if [ $necesita -eq 0 ]; then
        java_src_ok=1
        return
    fi

    java_src_out=$($JAVAC -encoding UTF-8 -d "$CLASSES_DIR" "${fuentes[@]}" 2>&1)
    if [ $? -eq 0 ]; then
        java_src_ok=1
        java_src_recompiled=1
        touch "$JAVA_SRC_STAMP"
    fi
}

# ----------------------------------------------------------
# compilar_y_ejecutar_java <test_src>
# ----------------------------------------------------------
compilar_y_ejecutar_java() {
    local test_src="$1"

    local nombre
    nombre=$(basename "$test_src" .java)
    local salida="$OUT_DIR/${nombre}.test.txt"

    total=$((total + 1))

    MEMCHECK["$nombre"]="N/A"

    compilar_java_src
    if [ $java_src_ok -ne 1 ]; then
        echo "$java_src_out" > "$salida"
        resultados+=("ERROR|$nombre")
        error=$((error + 1))
        mostrar_progreso "$nombre"
        return
    fi

    # Reutiliza el .class del test si ya está actualizado y las
    # clases de src/ no se han recompilado en esta ejecución.
    local clase="$CLASSES_DIR/${nombre}.class"
    if [ $java_src_recompiled -eq 1 ] || [ ! -f "$clase" ] || [ "$test_src" -nt "$clase" ]; then
        local compile_out
        compile_out=$($JAVAC -encoding UTF-8 -cp "$CLASSES_DIR" -d "$CLASSES_DIR" "$test_src" 2>&1)
        if [ $? -ne 0 ]; then
            echo "$compile_out" > "$salida"
            resultados+=("ERROR|$nombre")
            error=$((error + 1))
            mostrar_progreso "$nombre"
            return
        fi
    fi

    local test_out
    test_out=$(timeout "$TIMEOUT_S" $JAVA -cp "$CLASSES_DIR" "$nombre" 2>&1 > >(tee "$OUT_DIR/${nombre}.cout.txt"))
    local test_rc=$?
    echo "$test_out" > "$salida"
    registrar_resultado "$test_rc" "$nombre" "$salida"
    mostrar_progreso "$nombre"
}

# ----------------------------------------------------------
# Cabecera
# ----------------------------------------------------------
echo ""
echo "  Compilando y ejecutando $total_esperado prueba(s) [$SRC_DIR/]..."
echo ""

# ----------------------------------------------------------
# resolver_fuentes <test_src>
#   Analiza los #include del test (C++) para determinar qué
#   ficheros .cc de src/ necesita. Si el test no incluye
#   ninguna cabecera de src/, devuelve "make" (necesita el
#   binario completo de la aplicación).
# ----------------------------------------------------------
resolver_fuentes() {
    local test_src="$1"
    local srcs=""
    local visitados=""
    local directo=0
    local cola="$test_src"

    while [ -n "$cola" ]; do
        local actual="${cola%% *}"
        if [ "$cola" = "$actual" ]; then
            cola=""
        else
            cola="${cola#* }"
        fi

        local headers
        headers=$(grep -oE '#include "[^"]+"' "$actual" 2>/dev/null \
            | sed 's/#include "//;s/"//') || true

        local dir_actual
        dir_actual=$(dirname "$actual")
        for h in $headers; do
            case "$h" in common/*) continue ;; esac
            # Un #include relativo a la carpeta del fichero que lo contiene
            # (p. ej. "Persona.h" dentro de src/D06_x/Agenda.h) se
            # normaliza a su ruta relativa a $SRC_DIR.
            if [ ! -f "$SRC_DIR/$h" ] && [ -f "$dir_actual/$h" ]; then
                case "$dir_actual" in
                    "$SRC_DIR"/*) h="${dir_actual#"$SRC_DIR"/}/$h" ;;
                esac
            fi
            case " $visitados " in *" $h "*) continue ;; esac
            visitados="$visitados $h"

            # Un .cc de src/ incluido directamente ya forma parte de la
            # unidad de compilación del test: no se compila aparte.
            if [ "${h##*.}" = "cc" ]; then
                [ -f "$SRC_DIR/$h" ] && directo=1
            else
                local cc="${h%.h}.cc"
                if [ -f "$SRC_DIR/$cc" ]; then
                    srcs="$srcs $SRC_DIR/$cc"
                fi
            fi

            if [ -f "$SRC_DIR/$h" ]; then
                cola="$cola $SRC_DIR/$h"
            fi
        done
    done

    if [ -n "$srcs" ]; then
        echo "$srcs"
    elif [ $directo -eq 1 ]; then
        echo "-"
    else
        echo "make"
    fi
}

# ----------------------------------------------------------
# Ejecutar pruebas grupo por grupo
# ----------------------------------------------------------
make_done=0

for ((i=0; i<${#GRUPOS_LETRA[@]}; i++)); do
    letra="${GRUPOS_LETRA[$i]}"

    grupo_files=()
    for f in "${ficheros[@]+"${ficheros[@]}"}"; do
        case "$(basename "$f")" in
            test_${letra}*) grupo_files+=("$f") ;;
        esac
    done
    [ ${#grupo_files[@]} -eq 0 ] && continue

    for f in "${grupo_files[@]}"; do
        case "$f" in
            *.java)
                compilar_y_ejecutar_java "$f"
                continue
                ;;
        esac

        fuentes=$(resolver_fuentes "$f")

        if [ "$fuentes" = "make" ]; then
            if [ $make_done -eq 0 ]; then
                app_compile_out=$(make 2>&1)
                make_ok=$?
                make_done=1
            fi
            if [ ${make_ok:-1} -ne 0 ]; then
                nombre=$(basename "$f" .cc)
                total=$((total + 1))
                error=$((error + 1))
                resultados+=("ERROR|$nombre")
                MEMCHECK["$nombre"]="N/A"
                echo "$app_compile_out" > "$OUT_DIR/${nombre}.test.txt"
                mostrar_progreso "$nombre"
            else
                compilar_y_ejecutar_cc "$f" ""
            fi
        elif [ "$fuentes" = "-" ]; then
            compilar_y_ejecutar_cc "$f" ""
        else
            compilar_y_ejecutar_cc "$f" "$fuentes"
        fi
    done
done

printf '\r%80s\r' ' '
echo ""

# ----------------------------------------------------------
# Informe de resultados
#   Con --eval, todo el informe se vuelca a $INFORME_FILE (sin
#   barra de progreso) en lugar de mostrarse por pantalla.
# ----------------------------------------------------------
if [ "$EVAL_MODE" -eq 1 ]; then
    exec 3>&1
    exec > "$INFORME_FILE" 2>&1
fi

echo "==========================================="
echo "  INFORME DE PRUEBAS"
echo "==========================================="

imprimir_grupo() {
    local prefijo="$1"
    local titulo="$2"
    local tiene=0
    for r in "${resultados[@]+"${resultados[@]}"}"; do
        nombre="${r#*|}"
        case "$nombre" in ${prefijo}*) tiene=1 ;; esac
    done
    [ $tiene -eq 0 ] && return
    echo ""
    echo "  --- $titulo ---"
    for r in "${resultados[@]}"; do
        estado="${r%%|*}"
        nombre="${r#*|}"
        case "$nombre" in ${prefijo}*)
            local mem="${MEMCHECK[$nombre]:-N/A}"
            case "$mem" in
                FAIL|TIMEOUT)
                    # Hay error de memoria: la primera columna pasa a ser
                    # MEMORY y la segunda muestra si la prueba en sí pasó.
                    local linea
                    linea=$(printf "  ${AZUL}MEMORY${NC}   %s" "$nombre")
                    local test_col=""
                    case "$estado" in
                        PASS)    test_col=$(printf "${VERDE}test: PASS${NC}") ;;
                        FAIL)    test_col=$(printf "${ROJO}test: FAIL${NC}") ;;
                        ERROR)   test_col=$(printf "${MORADO}test: ERROR${NC}") ;;
                        CRASH)   test_col=$(printf "${CIAN}test: CRASH${NC}") ;;
                        TIMEOUT) test_col=$(printf "${AMARILLO}test: TIMEOUT${NC}") ;;
                    esac
                    printf "%-51s  %s\n" "$linea" "$test_col"
                    ;;
                *)
                    case "$estado" in
                        PASS)    printf "  ${VERDE}PASS${NC}     %s\n" "$nombre" ;;
                        FAIL)    printf "  ${ROJO}FAIL${NC}     %s\n" "$nombre" ;;
                        ERROR)   printf "  ${MORADO}ERROR${NC}    %s\n" "$nombre" ;;
                        CRASH)   printf "  ${CIAN}CRASH${NC}    %s\n" "$nombre" ;;
                        TIMEOUT) printf "  ${AMARILLO}TIMEOUT${NC}  %s\n" "$nombre" ;;
                    esac
                    ;;
            esac
        ;; esac
    done
}

for ((i=0; i<${#GRUPOS_LETRA[@]}; i++)); do
    imprimir_grupo "test_${GRUPOS_LETRA[$i]}" "Grupo ${GRUPOS_LETRA[$i]}: ${GRUPOS_DESC[$i]}"
done

echo ""
echo "==========================================="

# ----------------------------------------------------------
# Resumen y nota
#   El grupo Z (pruebas del alumno) nunca cuenta para la nota.
#   La nota solo se calcula al ejecutar exactamente el grupo X
#   (pruebas del examen), y se calcula sobre el número de
#   pruebas test_X* encontradas. Si no hay ninguna, no se
#   muestra nota.
# ----------------------------------------------------------
z_total=0
z_pass=0
x_total=0
x_pass=0
for r in "${resultados[@]+"${resultados[@]}"}"; do
    nombre="${r#*|}"
    estado="${r%%|*}"
    aprobada=0
    if [ "$estado" = "PASS" ]; then
        case "${MEMCHECK[$nombre]:-N/A}" in
            FAIL|TIMEOUT) aprobada=0 ;;
            *)            aprobada=1 ;;
        esac
    fi
    case "$nombre" in
        test_Z*)
            z_total=$((z_total + 1))
            [ $aprobada -eq 1 ] && z_pass=$((z_pass + 1))
            ;;
        test_X*)
            x_total=$((x_total + 1))
            [ $aprobada -eq 1 ] && x_pass=$((x_pass + 1))
            ;;
    esac
done

graded_total=$((total - z_total))
graded_pass=$((pass - z_pass))

if [ $graded_total -gt 0 ]; then
    echo ""
    echo "  Aprobadas: $graded_pass  |  Fallidas: $fail  |  Errores compilación: $error  |  Crash: $crash  |  Timeout: $tout  |  Total: $graded_total"
    if [ $VALGRIND_OK -eq 1 ]; then
        echo "  Errores de memoria (valgrind): $mem_err"
    fi
fi

# La nota solo se calcula al ejecutar el grupo X (examen)
FILTRO_MAYUS=$(echo "$FILTRO" | tr '[:lower:]' '[:upper:]')
if [ "$FILTRO_MAYUS" = "X" ] && [ $x_total -gt 0 ]; then
    nota=$(awk "BEGIN { n = ($x_pass / $x_total) * 10; if (n == int(n)) printf \"%d\", n; else printf \"%.2f\", n }")
    echo ""
    echo "  NOTA (examen, grupo X): $nota/10"
    if [ "$EVAL_MODE" -eq 1 ]; then
        echo "$nota" > "$NOTA_FILE"
    fi
fi

if [ $z_total -gt 0 ]; then
    echo ""
    echo "  Aprobadas: $z_pass  |  Total: $z_total  (Grupo Z, no cuenta para la nota)"
fi

echo ""
echo "  Salida de cada prueba en: $OUT_DIR/"
echo ""

if [ "$EVAL_MODE" -eq 1 ]; then
    exec 1>&3 3>&-
    echo ""
    echo "  Informe guardado en: $INFORME_FILE"
    [ -f "$NOTA_FILE" ] && echo "  Nota guardada en: $NOTA_FILE"
    echo ""
fi
