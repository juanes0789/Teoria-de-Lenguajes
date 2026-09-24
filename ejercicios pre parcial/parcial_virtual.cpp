// =====================================================
// PARCIAL (25%) - TEORIA DE LENGUAJES
// Sistema de seguimiento de recursos UdeA
//
// Punto 1 -> Expresion regular        (codigo BP-XXXXX)
// Punto 2 -> Expresion regular        (movimiento BP72000)
// Punto 3 -> AFD  (+ AFND de apoyo)   (secuencia BP72000GM11000...)
// Punto 4 -> Automata de pila         (estructuras {BP[GM(CT)]})
// Punto 5 -> Gramatica -> funciones   (reporte BP GM CT)
// Punto 6 -> Integracion en el main
// =====================================================

#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <vector>
#include <cctype>

using namespace std;

// =====================================================
// PUNTO 1. IDENTIFICACION DE LOS RECURSOS
// -----------------------------------------------------
// Lenguaje: exactamente BP, GM o CT + '-' + 5 digitos.
// Es un lenguaje REGULAR, finito y con forma fija
// -> la herramienta natural es una EXPRESION REGULAR.
//
//   ^            inicio de la cadena
//   (BP|GM|CT)   una de las tres categorias
//   -            el guion literal
//   [0-9]{5}     exactamente 5 digitos
//   $            fin de la cadena
// =====================================================

regex patronIdentificador("^(BP|GM|CT)-[0-9]{5}$");

bool validarIdentificador(const string& codigo) {
    return regex_match(codigo, patronIdentificador);
}

// =====================================================
// PUNTO 2. CLASIFICACION DE LOS MOVIMIENTOS
// -----------------------------------------------------
// Lenguaje: (BP|GM|CT) seguido de UNO O MAS digitos.
// "independientemente de la cantidad de caracteres
// numericos" -> se usa + (uno o mas) en vez de {5}.
// Sigue siendo regular -> expresion regular.
//
// La categoria son siempre los 2 primeros caracteres,
// asi que si la cadena es valida basta con substr(0, 2).
// =====================================================

regex patronMovimiento("^(BP|GM|CT)[0-9]+$");

bool validarMovimiento(const string& movimiento) {
    return regex_match(movimiento, patronMovimiento);
}

string categoriaMovimiento(const string& movimiento) {
    if (!validarMovimiento(movimiento))
        return "";

    return movimiento.substr(0, 2); // "BP", "GM" o "CT"
}

// =====================================================
// PUNTO 3. SEGUIMIENTO DE LA DISTRIBUCION  (AFD)
// -----------------------------------------------------
// Lenguaje: una secuencia de movimientos pegados, que
// DEBE comenzar con BP, y luego puede tener cualquier
// cantidad de movimientos BP / GM / CT, cada uno con
// uno o mas digitos.
//
//   L = BP[0-9]+ ( (BP|GM|CT)[0-9]+ )*
//
// Solo hay que recordar "en que parte del movimiento
// voy" (no hay que contar ni emparejar nada) -> memoria
// finita -> AFD.
//
// Estados:
//   q0: inicio
//   q1: leyo la B inicial
//   q2: leyo una categoria completa, espera digito
//   q3: leyendo digitos             (ACEPTACION)
//   q4: leyo B de un nuevo movimiento
//   q5: leyo G de un nuevo movimiento
//   q6: leyo C de un nuevo movimiento
//   q7: error (sumidero, ya no sale de aqui)
//
// Tabla de transiciones (D = digito, lo demas -> q7):
//        B    P    G    M    C    T    D
//   q0   q1   -    -    -    -    -    -
//   q1   -    q2   -    -    -    -    -
//   q2   -    -    -    -    -    -    q3
//   q3   q4   -    q5   -    q6   -    q3
//   q4   -    q2   -    -    -    -    -
//   q5   -    -    -    q2   -    -    -
//   q6   -    -    -    -    -    q2   -
// =====================================================

string nombreEstado(int estado) {
    switch (estado) {
        case 0: return "q0 (inicio)";
        case 1: return "q1 (leyo B inicial)";
        case 2: return "q2 (categoria completa, espera digito)";
        case 3: return "q3 (leyendo digitos) [ACEPTACION]";
        case 4: return "q4 (leyo B)";
        case 5: return "q5 (leyo G)";
        case 6: return "q6 (leyo C)";
        default: return "q7 (ERROR)";
    }
}

int transicionAFD(int estado, char c) {
    switch (estado) {
        case 0:
            if (c == 'B') return 1;
            return 7;

        case 1:
            if (c == 'P') return 2;
            return 7;

        case 2:
            if (isdigit(c)) return 3;
            return 7;

        case 3:
            if (isdigit(c)) return 3;
            if (c == 'B') return 4;
            if (c == 'G') return 5;
            if (c == 'C') return 6;
            return 7;

        case 4:
            if (c == 'P') return 2;
            return 7;

        case 5:
            if (c == 'M') return 2;
            return 7;

        case 6:
            if (c == 'T') return 2;
            return 7;

        default:
            return 7; // sumidero
    }
}

bool validarSecuencia(const string& cadena, bool mostrarTraza) {
    int estado = 0;

    if (mostrarTraza) {
        cout << "\nCadena: " << cadena << "\n";
        cout << "\nProcesamiento:\n";
    }

    for (char c : cadena) {
        int anterior = estado;
        estado = transicionAFD(estado, c);

        if (mostrarTraza)
            cout << "  " << c << " : " << nombreEstado(anterior)
                 << " -> " << nombreEstado(estado) << "\n";
    }

    bool aceptada = (estado == 3);

    if (mostrarTraza)
        cout << "\nEstado final: " << nombreEstado(estado)
             << "\nResultado: " << (aceptada ? "ACEPTADA" : "RECHAZADA") << "\n";

    return aceptada;
}

// =====================================================
// PUNTO 3 (segunda parte). "reconocer si durante el
// recorrido de una cadena existe o no una determinada
// secuencia de simbolos"  ->  AFND
// -----------------------------------------------------
// "Contiene la subcadena X" es el ejemplo clasico de
// AFND: en q0 me quedo leyendo cualquier cosa y ADIVINO
// cuando empieza X. Se simula igual que en clase: con
// varios estados activos a la vez.
//
// Para el patron "GM" los estados son:
//   p0 -cualquiera-> p0      p0 -G-> p1
//   p1 -M-> p2               p2 -cualquiera-> p2 (final)
//
// Generalizado: el estado i significa "ya lei los i
// primeros simbolos del patron". vector<bool> guarda
// cuales estados estan activos (como q0, q1, q2 en
// afn.cpp pero sin fijar la cantidad).
// =====================================================

bool contieneSecuencia(const string& cadena, const string& patron, bool mostrarTraza) {
    int n = patron.size();
    vector<bool> activos(n + 1, false);
    activos[0] = true; // estado inicial

    if (mostrarTraza)
        cout << "\nBuscando \"" << patron << "\" en " << cadena << "\n";

    for (char c : cadena) {
        vector<bool> nuevos(n + 1, false);

        for (int i = 0; i <= n; i++) {
            if (!activos[i])
                continue;

            if (i == 0)
                nuevos[0] = true;           // p0 se queda en p0 con cualquier simbolo

            if (i == n)
                nuevos[n] = true;           // el final se queda en el final

            if (i < n && c == patron[i])
                nuevos[i + 1] = true;       // avanza un simbolo del patron
        }

        activos = nuevos;

        if (mostrarTraza) {
            cout << "  " << c << " -> activos: { ";
            for (int i = 0; i <= n; i++)
                if (activos[i]) cout << "p" << i << " ";
            cout << "}\n";
        }
    }

    if (mostrarTraza)
        cout << "Resultado: " << (activos[n] ? "CONTIENE" : "NO CONTIENE") << "\n";

    return activos[n];
}

// =====================================================
// PUNTO 4. ESTRUCTURAS DE DISTRIBUCION  (AUTOMATA DE PILA)
// -----------------------------------------------------
// Hay anidamiento SIN profundidad maxima -> hay que
// recordar TODAS las aperturas pendientes en orden.
// Eso no lo puede hacer un AFD (memoria finita).
// -> se necesita una PILA.
//
//   apertura  -> push
//   cierre    -> la pila no puede estar vacia y el tope
//                debe ser su pareja -> pop
//   al final  -> la pila debe quedar vacia
//
// Decision extra: las letras entre delimitadores deben
// formar una categoria valida (BP, GM o CT) y debe
// haber al menos un par de delimitadores.
// =====================================================

bool apertura(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool cierre(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool pareja(char abierto, char cerrado) {
    return (abierto == '(' && cerrado == ')') ||
           (abierto == '[' && cerrado == ']') ||
           (abierto == '{' && cerrado == '}');
}

bool esCategoria(const string& palabra) {
    return palabra == "BP" || palabra == "GM" || palabra == "CT";
}

bool validarEstructura(const string& estructura, bool mostrarErrores) {
    stack<char> pila;
    string palabra = "";  // letras acumuladas entre delimitadores
    bool hayDelimitador = false;

    for (int i = 0; i < (int)estructura.size(); i++) {
        char c = estructura[i];

        if (isupper(c)) {
            palabra += c;
            continue;
        }

        if (!apertura(c) && !cierre(c)) {
            if (mostrarErrores)
                cout << "  Error: caracter no permitido '" << c << "' en posicion " << i + 1 << "\n";
            return false;
        }

        // Llego un delimitador: revisar la palabra acumulada
        if (palabra != "" && !esCategoria(palabra)) {
            if (mostrarErrores)
                cout << "  Error: \"" << palabra << "\" no es una categoria (BP, GM, CT)\n";
            return false;
        }
        palabra = "";
        hayDelimitador = true;

        if (apertura(c)) {
            pila.push(c);
        }
        else { // cierre
            if (pila.empty()) {
                if (mostrarErrores)
                    cout << "  Error en posicion " << i + 1 << ": cierre sin apertura\n";
                return false;
            }

            if (!pareja(pila.top(), c)) {
                if (mostrarErrores)
                    cout << "  Error en posicion " << i + 1 << ": '" << pila.top()
                         << "' se cierra con '" << c << "'\n";
                return false;
            }

            pila.pop();
        }
    }

    // Letras sueltas al final, ej: "{BP}GM" o "BP"
    if (palabra != "") {
        if (mostrarErrores)
            cout << "  Error: \"" << palabra << "\" esta por fuera de toda estructura\n";
        return false;
    }

    if (!hayDelimitador) {
        if (mostrarErrores)
            cout << "  Error: no hay ninguna estructura\n";
        return false;
    }

    if (!pila.empty()) {
        if (mostrarErrores)
            cout << "  Error: faltan " << pila.size() << " simbolo(s) de cierre\n";
        return false;
    }

    return true;
}

// =====================================================
// PUNTO 5. REGLAS PARA CONSTRUIR UN REPORTE (GRAMATICA)
// -----------------------------------------------------
//   R -> M | M R
//   M -> BP | GM | CT
//
// Cada NO TERMINAL se vuelve una FUNCION (analizador
// descendente recursivo):
//   M(): revisa que el token actual sea BP, GM o CT.
//   R(): llama a M(); si era el ultimo token aplica
//        R -> M; si quedan tokens aplica R -> M R y se
//        llama a si misma (recursion = la regla R -> M R).
//
// La recursion usa la pila de llamadas del programa, que
// es justamente la pila de un automata de pila.
// (Nota: esta gramatica en particular tambien es regular:
//  ^(BP|GM|CT)( (BP|GM|CT))*$ ; se implementa con
//  funciones porque el enunciado pide traducir las reglas.)
// =====================================================

// Separa la cadena por espacios (sin librerias nuevas)
vector<string> separarTokens(const string& cadena) {
    vector<string> tokens;
    string actual = "";

    for (char c : cadena) {
        if (c == ' ') {
            if (actual != "") {
                tokens.push_back(actual);
                actual = "";
            }
        }
        else {
            actual += c;
        }
    }

    if (actual != "")
        tokens.push_back(actual);

    return tokens;
}

// M -> BP | GM | CT
bool reglaM(const vector<string>& tokens, int pos, bool mostrarTraza) {
    bool ok = esCategoria(tokens[pos]);

    if (mostrarTraza)
        cout << "    M -> " << tokens[pos] << (ok ? "   ok" : "   NO ES BP/GM/CT") << "\n";

    return ok;
}

// R -> M | M R
bool reglaR(const vector<string>& tokens, int pos, bool mostrarTraza) {
    bool esUltimo = (pos == (int)tokens.size() - 1);

    if (mostrarTraza)
        cout << "  R -> " << (esUltimo ? "M" : "M R") << "\n";

    if (!reglaM(tokens, pos, mostrarTraza))
        return false;

    if (esUltimo)
        return true;                                  // R -> M

    return reglaR(tokens, pos + 1, mostrarTraza);     // R -> M R
}

bool validarReporte(const string& reporte, bool mostrarTraza) {
    vector<string> tokens = separarTokens(reporte);

    if (tokens.empty())   // R necesita al menos un M
        return false;

    if (mostrarTraza)
        cout << "\nDerivacion:\n";

    return reglaR(tokens, 0, mostrarTraza);
}

// =====================================================
// UTILIDADES DE PRESENTACION
// =====================================================

string textoResultado(bool ok) {
    return ok ? "ACEPTADO" : "RECHAZADO";
}

string leerLinea(const string& mensaje) {
    string dato;
    cout << mensaje;
    getline(cin, dato);
    return dato;
}

// =====================================================
// PRUEBAS AUTOMATICAS  ("Realice diferentes pruebas")
// =====================================================

void probar(const string& nombre, const string& cadena, bool obtenido, bool esperado) {
    cout << "  " << (obtenido == esperado ? "[OK]   " : "[FALLA]") << " "
         << nombre << " \"" << cadena << "\" -> " << textoResultado(obtenido) << "\n";
}

void pruebasAutomaticas() {
    cout << "\n--- Punto 1: Identificador ---\n";
    probar("P1", "BP-72000", validarIdentificador("BP-72000"), true);
    probar("P1", "GM-11000", validarIdentificador("GM-11000"), true);
    probar("P1", "CT-04000", validarIdentificador("CT-04000"), true);
    probar("P1", "BP72000",  validarIdentificador("BP72000"),  false);
    probar("P1", "BP-7200",  validarIdentificador("BP-7200"),  false);
    probar("P1", "XX-12345", validarIdentificador("XX-12345"), false);
    probar("P1", "bp-72000", validarIdentificador("bp-72000"), false);

    cout << "\n--- Punto 2: Movimiento ---\n";
    probar("P2", "BP72000",   validarMovimiento("BP72000"),   true);
    probar("P2", "GM1",       validarMovimiento("GM1"),       true);
    probar("P2", "CT0400099", validarMovimiento("CT0400099"), true);
    probar("P2", "BP",        validarMovimiento("BP"),        false);
    probar("P2", "BP-72000",  validarMovimiento("BP-72000"),  false);
    probar("P2", "PB72000",   validarMovimiento("PB72000"),   false);

    cout << "\n--- Punto 3: Secuencia (AFD) ---\n";
    probar("P3", "BP72000",               validarSecuencia("BP72000", false),               true);
    probar("P3", "BP72000GM11000CT04000", validarSecuencia("BP72000GM11000CT04000", false), true);
    probar("P3", "BP1BP2",                validarSecuencia("BP1BP2", false),                true);
    probar("P3", "GM11000",               validarSecuencia("GM11000", false),               false);
    probar("P3", "BP72000GM",             validarSecuencia("BP72000GM", false),             false);
    probar("P3", "BPGM11000",             validarSecuencia("BPGM11000", false),             false);
    probar("P3", "contiene GM: BP1GM2",   contieneSecuencia("BP1GM2", "GM", false),         true);
    probar("P3", "contiene GM: BP1CT2",   contieneSecuencia("BP1CT2", "GM", false),         false);

    cout << "\n--- Punto 4: Estructura (pila) ---\n";
    vector<string> buenas = {"{BP}", "{{BP}}", "{BP{GM}}", "{{BP}{GM}{CT}}",
                             "{{{BP}{GM}}{CT}}", "{BP[GM]}", "{BP[GM(CT)]}"};
    vector<string> malas  = {"{BP", "BP}", "{{BP}", "{BP}}", "{BP[GM}]", "{XY}", "BP", "{BP}GM"};
    for (const string& s : buenas) probar("P4", s, validarEstructura(s, false), true);
    for (const string& s : malas)  probar("P4", s, validarEstructura(s, false), false);

    cout << "\n--- Punto 5: Reporte (gramatica) ---\n";
    probar("P5", "BP",          validarReporte("BP", false),          true);
    probar("P5", "BP GM",       validarReporte("BP GM", false),       true);
    probar("P5", "BP GM CT",    validarReporte("BP GM CT", false),    true);
    probar("P5", "CT BP GM BP", validarReporte("CT BP GM BP", false), true);
    probar("P5", "",            validarReporte("", false),            false);
    probar("P5", "BP XX",       validarReporte("BP XX", false),       false);
    probar("P5", "BPGM",        validarReporte("BPGM", false),        false);
}

// =====================================================
// PUNTO 6. INTEGRACION DEL SISTEMA
// =====================================================

void sistemaIntegrado() {
    cout << "\n--- Ingrese la informacion del registro ---\n";
    string identificador = leerLinea("1. Identificador (ej. BP-72000): ");
    string movimiento    = leerLinea("2. Movimiento (ej. BP72000): ");
    string secuencia     = leerLinea("3. Secuencia (ej. BP72000GM11000CT04000): ");
    string estructura    = leerLinea("4. Estructura (ej. {BP[GM(CT)]}): ");
    string reporte       = leerLinea("5. Reporte (ej. BP GM CT): ");

    bool okId  = validarIdentificador(identificador);
    bool okMov = validarMovimiento(movimiento);
    bool okSec = validarSecuencia(secuencia, false);
    bool okEst = validarEstructura(estructura, false);
    bool okRep = validarReporte(reporte, false);

    cout << "\n==================================================\n";
    cout << "        SISTEMA DE SEGUIMIENTO DE RECURSOS\n";
    cout << "==================================================\n\n";

    cout << "Identificador: " << identificador << "\n";
    cout << "Resultado: " << textoResultado(okId) << "\n\n";

    cout << "Movimiento: " << movimiento << "\n";
    if (okMov)
        cout << "Categoria: " << categoriaMovimiento(movimiento) << "\n";
    cout << "Resultado: " << textoResultado(okMov) << "\n\n";

    cout << "Secuencia:\n" << secuencia << "\n";
    cout << "Resultado: " << textoResultado(okSec) << "\n\n";

    cout << "Estructura:\n" << estructura << "\n";
    cout << "Resultado: " << textoResultado(okEst) << "\n\n";

    cout << "Reporte:\n" << reporte << "\n";
    cout << "Resultado: " << textoResultado(okRep) << "\n\n";

    bool general = okId && okMov && okSec && okEst && okRep;

    cout << "--------------------------------------------------\n";
    cout << "REGISTRO GENERAL: " << textoResultado(general) << "\n";
    cout << "--------------------------------------------------\n";
}

// =====================================================
// MAIN: solo pide datos, llama funciones y muestra
// =====================================================

int main() {
    while (true) {
        cout << "\n==================================================\n";
        cout << "   PARCIAL TEORIA DE LENGUAJES - RECURSOS UdeA\n";
        cout << "==================================================\n";
        cout << "1. Punto 1 - Validar identificador\n";
        cout << "2. Punto 2 - Clasificar movimiento\n";
        cout << "3. Punto 3 - Procesar secuencia (AFD con traza)\n";
        cout << "4. Punto 3 - Buscar una secuencia de simbolos (AFND)\n";
        cout << "5. Punto 4 - Validar estructura (pila)\n";
        cout << "6. Punto 5 - Validar reporte (gramatica)\n";
        cout << "7. Punto 6 - Sistema integrado\n";
        cout << "8. Ejecutar pruebas automaticas\n";
        cout << "0. Salir\n";

        string opcion = leerLinea("Opcion: ");

        if (opcion == "0") {
            cout << "Saliendo...\n";
            break;
        }
        else if (opcion == "1") {
            string codigo = leerLinea("Codigo: ");
            cout << "Identificador " << (validarIdentificador(codigo) ? "VALIDO" : "INVALIDO") << "\n";
        }
        else if (opcion == "2") {
            string mov = leerLinea("Movimiento: ");
            if (validarMovimiento(mov))
                cout << "Movimiento valido\nCategoria: " << categoriaMovimiento(mov) << "\n";
            else
                cout << "Movimiento invalido\n";
        }
        else if (opcion == "3") {
            validarSecuencia(leerLinea("Secuencia: "), true);
        }
        else if (opcion == "4") {
            string cadena = leerLinea("Cadena: ");
            string patron = leerLinea("Secuencia a buscar (ej. GM): ");
            if (patron == "")
                cout << "Debe ingresar una secuencia a buscar\n";
            else
                contieneSecuencia(cadena, patron, true);
        }
        else if (opcion == "5") {
            bool ok = validarEstructura(leerLinea("Estructura: "), true);
            cout << "Estructura " << (ok ? "VALIDA" : "INVALIDA") << "\n";
        }
        else if (opcion == "6") {
            bool ok = validarReporte(leerLinea("Reporte (separado por espacios): "), true);
            cout << "Reporte " << (ok ? "VALIDO" : "INVALIDO") << "\n";
        }
        else if (opcion == "7") {
            sistemaIntegrado();
        }
        else if (opcion == "8") {
            pruebasAutomaticas();
        }
        else {
            cout << "Opcion invalida\n";
        }
    }

    return 0;
}
