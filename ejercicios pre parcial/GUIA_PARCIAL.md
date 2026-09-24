# Guía del parcial: Teoría de Lenguajes en C++

La solución completa está en `parcial_virtual.cpp`. Compilar y ejecutar:

```bash
g++ -std=c++17 parcial_virtual.cpp -o parcial && ./parcial
```

Con la opción **8** del menú se corren las pruebas automáticas (todas deberían salir `[OK]`).

---

## 0. La pregunta clave: ¿qué herramienta uso?

El parcial **no dice** qué concepto usar. Hazte estas preguntas en orden:

| Pregunta sobre el lenguaje | Si la respuesta es SÍ, usa | Palabras que lo delatan |
|---|---|---|
| ¿Es un **formato fijo** (letras, dígitos, longitudes) que se valida de un solo golpe? | **Regex** (`regex_match`) | "código", "formato", "estructura XX-00000", "correo", "teléfono", "rechazar los que no cumplan" |
| ¿Hay que procesarlo **símbolo por símbolo** y mostrar el recorrido, pero solo necesito recordar *en qué parte voy*? | **AFD** (`int estado` + `switch`) | "símbolo por símbolo", "estado de aceptación", "mostrar el procesamiento", "termina en", "comienza con", "orden" |
| ¿Tengo que **adivinar** dónde empieza algo, o busco si **contiene** una subcadena? | **AFND** (varios `bool` activos a la vez) | "existe o no una secuencia", "contiene", "en algún momento aparece" |
| ¿Hay **anidamiento sin límite** o cosas que se **abren y cierran**, o hay que **contar/emparejar**? | **Autómata de pila** (`stack<char>`) | "puede contener otras que contienen otras", "sin profundidad máxima", "correspondencia apertura/cierre", aⁿbⁿ |
| ¿Te dan **reglas tipo `A → ...`**? | **Gramática → una función por no terminal** | "reglas", "R → M R", "se puede generar" |

> **Regla de oro:** si necesitas **memoria ilimitada** (contar, emparejar, anidar), es **pila**. Si la memoria es **finita** (un número fijo de situaciones), es **AFD/AFND/regex**.

---

## Punto 1: Identificador `BP-72000` → Regex

```cpp
regex patronIdentificador("^(BP|GM|CT)-[0-9]{5}$");
bool validarIdentificador(const string& codigo) {
    return regex_match(codigo, patronIdentificador);
}
```

**Justificación:** el lenguaje es regular y de forma fija (prefijo de 3 opciones, guion, exactamente 5 dígitos), así que una expresión regular lo describe completo en una línea.

**Chuleta de regex:**

| Símbolo | Significa | Ejemplo |
|---|---|---|
| `^` `$` | inicio / fin (obligan a que **toda** la cadena cumpla) | `^abc$` |
| `(A\|B)` | A o B | `(BP\|GM\|CT)` |
| `[0-9]` `[A-Z]` `[a-z]` | un carácter del rango | `[A-Z][a-z]+` |
| `{n}` `{n,}` `{n,m}` | exactamente n / al menos n / entre n y m | `[0-9]{5}` |
| `+` `*` `?` | 1 o más / 0 o más / 0 o 1 | `[0-9]+` |
| `\\.` `\\-` | carácter literal (en C++ el `\` va doble) | `@gmail\\.com` |
| `( ...)*` | repetir un grupo | `(BP\|GM)( (BP\|GM))*` |

**Casos parecidos:** placas `^[A-Z]{3}-[0-9]{3}$`, cédula `^[0-9]{6,10}$`, correo `^[a-z0-9]+@udea\\.edu\\.co$`.

---

## Punto 2: Movimiento `BP72000` → Regex + categoría

```cpp
regex patronMovimiento("^(BP|GM|CT)[0-9]+$");
string categoria = movimiento.substr(0, 2);   // solo si es válido
```

**Qué lo delata:** *"independientemente de la cantidad de caracteres numéricos"* → cambia `{5}` por `+`.

- Si el profesor acepta **cero** dígitos (`BP` solo), cambia `+` por `*`.
- Para saber el grupo: como la categoría siempre tiene 2 letras, basta con `substr(0, 2)`. (Otra opción es `smatch` con `regex_match(s, m, patron)` y `m[1]`, pero eso es código nuevo que tendrías que justificar.)

---

## Punto 3: Secuencia `BP72000GM11000CT04000` → AFD (+ AFND)

**Lenguaje:** `BP[0-9]+ ((BP|GM|CT)[0-9]+)*`, es decir, empieza con BP y luego vienen movimientos pegados.

**¿Por qué AFD?** Pide explícitamente *"procesar símbolo por símbolo"* y llegar a una *"condición de aceptación"*. Solo hay que recordar en qué parte del movimiento voy (7 situaciones), así que la memoria es finita.

### Diagrama de estados

```
          B        P        dígito
  (q0) ──────► q1 ─────► q2 ────────► ((q3)) ◄─┐ dígito
                          ▲             │  │  └─┘
                          │ P           │B │G  C
                          ├──── q4 ◄────┘  │   │
                          │ M              ▼   │
                          ├──── q5 ◄───────────┘ (G)
                          │ T
                          └──── q6 ◄──────────── (C)
  Cualquier otro símbolo → q7 (error, sumidero)
```

| | B | P | G | M | C | T | dígito |
|---|---|---|---|---|---|---|---|
| q0 | q1 | | | | | | |
| q1 | | q2 | | | | | |
| q2 | | | | | | | q3 |
| **q3*** | q4 | | q5 | | q6 | | q3 |
| q4 | | q2 | | | | | |
| q5 | | | | q2 | | | |
| q6 | | | | | | q2 | |

(celda vacía = q7 error; * = aceptación)

### Plantilla de AFD (sirve para cualquier AFD)

```cpp
int transicion(int estado, char c) {
    switch (estado) {
        case 0: if (c == 'B') return 1; return ERROR;
        ...
    }
}
bool validar(const string& s) {
    int estado = 0;
    for (char c : s) {
        int anterior = estado;
        estado = transicion(estado, c);
        cout << c << " : q" << anterior << " -> q" << estado << "\n"; // traza
    }
    return estado == ESTADO_FINAL;
}
```

**Pasos para diseñar cualquier AFD:**
1. Escribe 3 cadenas aceptadas y 3 rechazadas.
2. Pregúntate *"¿qué necesito recordar después de leer cada símbolo?"*. Cada respuesta distinta es un estado.
3. Llena la tabla estado × símbolo. Lo que no tenga sentido va al **estado de error (sumidero)**.
4. Marca los estados de aceptación (donde puede terminar la cadena).
5. Recorre a mano tus cadenas de prueba.

**Variaciones típicas:**
- *"Puede empezar con cualquier categoría":* desde q0 pon `B→q4`, `G→q5`, `C→q6` (q1 ya no hace falta).
- *"Debe terminar en CT":* agrega un estado final que solo se alcance después de un movimiento CT.
- *"Ignorar mayúsculas/minúsculas":* antes del ciclo, `for (char &c : s) c = toupper(c);` (como en tu taller de asistencia).

### Segunda parte: "¿existe o no una secuencia?" → AFND

*"Reconocer si durante el recorrido existe una determinada secuencia"* es el caso clásico de AFND con "contiene X": en p0 se queda leyendo cualquier cosa y **adivina** dónde empieza X. Se simula como en `afn.cpp`: se calculan los `nuevo_qi` a partir de los `qi` activos y luego se actualizan. En la solución está generalizado con `vector<bool>` para cualquier patrón:

```
p0 ─cualquiera─► p0     p0 ─G─► p1 ─M─► ((p2)) ─cualquiera─► p2
```

---

## Punto 4: Estructuras `{BP[GM(CT)]}` → Autómata de pila

**Qué lo delata:** *"puede contener otras que contienen otras"*, *"sin profundidad máxima"*, *"correspondencia entre apertura y cierre"*. Un AFD no puede recordar un número ilimitado de aperturas pendientes; una **pila** sí.

**Algoritmo (el mismo de `autoamatapila.cpp`):**
1. Si llega una apertura `( [ {`, hago **push**.
2. Si llega un cierre: si la pila está **vacía**, error ("cierre sin apertura"); si el **tope** no es su pareja, error; si todo bien, hago **pop**.
3. Al terminar, si la pila **no está vacía**, error ("faltan cierres").

| Cadena | Por qué falla |
|---|---|
| `{BP` | la pila queda con `{` |
| `BP}` | cierre con la pila vacía |
| `{BP[GM}]` | el tope es `[` y llega `}` |

**Decisiones extra que tomé (y debes poder justificar):** las letras entre delimitadores tienen que ser BP/GM/CT, y debe haber al menos un par de delimitadores. Si el profesor no lo exige, se pueden quitar esas validaciones.

**Casos parecidos:** paréntesis balanceados, etiquetas HTML `<a></a>`, `aⁿbⁿ` (push por cada `a`, pop por cada `b`), palíndromos con marca central `wcwᴿ`.

---

## Punto 5: Reporte `BP GM CT` → Gramática como funciones

```
R → M | M R
M → BP | GM | CT
```

**Cómo traducir cualquier gramática a C++ (descenso recursivo):**
- **Cada no terminal se convierte en una función** `bool reglaX(tokens, pos)`.
- **Cada terminal** se compara con el token actual (`tokens[pos] == "BP"`).
- Una **alternativa** (`|`) se vuelve un `if` que decide qué regla aplicar.
- Una **regla recursiva** (`R → M R`) se vuelve una función que se llama a sí misma con `pos + 1`.

```cpp
bool reglaR(tokens, pos) {
    if (!reglaM(tokens, pos)) return false;
    if (pos == ultimo) return true;          // R -> M
    return reglaR(tokens, pos + 1);          // R -> M R
}
```

**Frase para justificar:** la recursión usa la pila de llamadas del programa, así que se comporta como un autómata de pila que reconoce la gramática. (Esta gramática en particular es lineal por la derecha, o sea regular, así que también serviría `^(BP|GM|CT)( (BP|GM|CT))*$`. Si te preguntan, menciónalo: demuestra que entiendes la jerarquía.)

**Otras gramáticas típicas:**
- `S → a S b | ab` es aⁿbⁿ: la función `S` consume una `a`, se llama a sí misma y luego exige una `b`.
- `E → T + E | T`, `T → número`: expresiones aritméticas.

---

## Punto 6: Integración

- Cada validación está en **su propia función** que devuelve `bool`.
- `main()` solo **pide datos**, **llama funciones** y **muestra resultados**, tal como lo exige el enunciado.
- `REGISTRO GENERAL = okId && okMov && okSec && okEst && okRep`.
- Usa `getline(cin, x)` para **todas** las lecturas: el reporte lleva espacios y mezclar `cin >>` con `getline` deja un `\n` pendiente (el bug que tenías en `ejercicio.cpp`: el `getline` de la expresión se saltaba).

---

## Código "nuevo" que debes saber justificar (Condición especial)

| Código | ¿Por qué? | ¿Cómo funciona? |
|---|---|---|
| `substr(0, 2)` | obtener la categoría del movimiento | devuelve 2 caracteres desde la posición 0 |
| `vector<bool> activos` en el AFND | el número de estados depende del largo del patrón | `activos[i]` = "ya leí los i primeros símbolos del patrón"; en cada símbolo se calcula `nuevos` y se reemplaza (igual que `nuevo_q0`, `nuevo_q1` de clase) |
| `separarTokens` | partir el reporte por espacios | recorre carácter por carácter; al ver un espacio guarda la palabra acumulada |
| funciones recursivas (`reglaR`) | la regla `R → M R` es recursiva | la función se llama a sí misma con el siguiente token; termina cuando llega al último (`R → M`) |
| `isupper`, `isdigit` (`<cctype>`) | clasificar caracteres | devuelven verdadero si el carácter es mayúscula / dígito |

---

## Errores comunes que debes evitar

1. Olvidar `^` y `$` en la regex, o usar `regex_search` en vez de `regex_match`: se aceptaría `xxBP-72000yy`.
2. Escribir un solo `\` en C++: tiene que ser `"\\."`.
3. En el AFND, actualizar los estados **mientras** se calculan (hay que usar `nuevo_*` y asignarlos al final).
4. En la pila, hacer `top()` sin revisar antes `empty()`, lo que hace que el programa falle.
5. No revisar al final que la pila esté vacía.
6. Poner `return 0;` dentro del `while` (pasó en `afn.cpp`, que por eso se sale después de la primera cadena).
7. Probar solo los ejemplos del enunciado: siempre agrega casos límite como la cadena vacía, una sola letra, minúsculas o símbolos raros.
