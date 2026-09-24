#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stack>


using namespace std;

regex patronPalabra("^[A-Z][a-z]+[A-Z]$");
regex patron("^[A-Za-z]+( [A-Za-z]+)*$"); 

int numero;
string palabra;
vector<int> numerosPares;

// metodo booleano que valida si el numero ingresado es par
bool validarNumeroPar(int numero) {
    if (numero % 2 == 0) {
        return true;
    } else {
        return false;
    }
}


// calcular el promedio de numeros en el vector que son mayores a un numero par ingresado por el usuario, si no hay numeros mayores a ese numero par, mostrar un mensaje indicando que no hay numeros mayores.

string calcularPromedioMayores(const vector<int>& numeros, int numeroPar) {
    int suma = 0;
    int contador = 0;

    for (int num : numeros) {
        if (num > numeroPar) {
            suma += num;
            contador++;
        }
    }

    if (contador > 0) {
        double promedio = static_cast<double>(suma) / contador;
        return "El promedio de los numeros mayores a " + to_string(numeroPar) + " es: " + to_string(promedio);
    } else {
        return "No hay numeros mayores a " + to_string(numeroPar);
    }
}

string mostrarNumeros(const vector<int>& numeros) {
    string resultado = "Numeros ingresados: ";
    for (int num : numeros) {
        resultado += to_string(num) + " ";
    }
    return resultado;
}
void mostrarNumerosAlReves(const vector<int>& numeros){
    cout << "Numeros ingresados al reves: ";
    for (int i = numeros.size() - 1; i >= 0; i--) {
        cout << numeros[i] << " ";
    }
}

string pedirDato(
    const string& mensaje,
    const regex& patron,
    const string& mensajeError
) {
    string dato;

    while (true) {
        cout << mensaje;
        getline(cin, dato);

        if (regex_match(dato, patron)) {
            cout << "Valido" << endl;
            return dato;
        }

        cout << mensajeError << endl;
    }
}

// =====================================================
// VERIFICAR SI ES OPERADOR
// =====================================================

bool operador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// =====================================================
// VERIFICAR SI ES APERTURA
// =====================================================

bool apertura(char c) {
    return c == '(' || c == '[' || c == '{';
}

// =====================================================
// VERIFICAR SI ES CIERRE
// =====================================================

bool cierre(char c) {
    return c == ')' || c == ']' || c == '}';
}

// =====================================================
// VERIFICAR QUE LOS SIMBOLOS COINCIDAN
// =====================================================

bool pareja(char abierto, char cerrado) {

    if (abierto == '(' && cerrado == ')')
        return true;

    if (abierto == '[' && cerrado == ']')
        return true;

    if (abierto == '{' && cerrado == '}')
        return true;

    return false;
}

// =====================================================
// 1. REVISAR CARACTERES
// =====================================================

bool caracteresCorrectos(const string& expresion) {

    for (int i = 0; i < expresion.size(); i++) {

        char c = expresion[i];

        if (isspace(c))
            continue;

        if (isdigit(c))
            continue;

        if (operador(c))
            continue;

        if (apertura(c) || cierre(c))
            continue;

        cout << "\nCaracter no permitido: " << c;
        cout << "\nPosicion: " << i + 1 << endl;

        return false;
    }

    return true;
}

// =====================================================
// 2. REVISAR SIMBOLOS CON PILA
// =====================================================

bool simbolosCorrectos(const string& expresion) {

    stack<char> pila;

    for (int i = 0; i < expresion.size(); i++) {

        char c = expresion[i];

        // Guardar aperturas
        if (apertura(c)) {
            pila.push(c);
        }

        // Revisar cierres
        else if (cierre(c)) {

            // No hay nada en la pila
            if (pila.empty()) {

                cout << "\nError en posicion " << i + 1;
                cout << ": cierre sin apertura.\n";

                return false;
            }

            // Sacar el elemento de arriba
            char arriba = pila.top();

            // Comprobar que sea su pareja
            if (!pareja(arriba, c)) {

                cout << "\nError en posicion " << i + 1;
                cout << ": los simbolos no coinciden.\n";

                return false;
            }

            pila.pop();
        }
    }

    // Si todavía hay aperturas,
    // faltan cierres
    if (!pila.empty()) {

        cout << "\nError: faltan simbolos de cierre.\n";

        return false;
    }

    return true;
}

// =====================================================
// 3. REVISAR ESTRUCTURA DE LA EXPRESION
// =====================================================

bool estructuraCorrecta(const string& expresion) {

    bool numero = true;
    bool existeNumero = false;

    for (int i = 0; i < expresion.size(); i++) {

        char c = expresion[i];

        if (isspace(c))
            continue;

        // =============================================
        // NUMERO
        // =============================================

        if (isdigit(c)) {

            if (!numero) {

                cout << "\nError de estructura:";
                cout << "\nHay numeros consecutivos.\n";

                return false;
            }

            existeNumero = true;
            numero = false;

            // Saltar todos los digitos
            while (i + 1 < expresion.size() &&
                   isdigit(expresion[i + 1])) {

                i++;
            }
        }

        // =============================================
        // OPERADOR
        // =============================================

        else if (operador(c)) {

            if (numero) {

                cout << "\nError de estructura:";
                cout << "\nOperador inesperado en posicion ";
                cout << i + 1 << ".\n";

                return false;
            }

            numero = true;
        }
    }

    // No hubo numeros
    if (!existeNumero) {

        cout << "\nError: no hay numeros en la expresion.\n";

        return false;
    }

    // La expresion terminó con operador
    if (numero) {

        cout << "\nError: la expresion termina con un operador.\n";

        return false;
    }

    return true;
}

// =====================================================
// MOSTRAR RESULTADO
// =====================================================

void resultado(bool caracteres,
               bool simbolos,
               bool estructura) {

    cout << "\n\n====================================";
    cout << "\n        RESULTADO DEL ANALISIS";
    cout << "\n====================================\n";

    cout << "Caracteres   : "
         << (caracteres ? "CORRECTOS" : "INCORRECTOS")
         << endl;

    cout << "Simbolos     : "
         << (simbolos ? "CORRECTOS" : "INCORRECTOS")
         << endl;

    cout << "Estructura   : "
         << (estructura ? "CORRECTA" : "INCORRECTA")
         << endl;

    cout << "\nResultado final: ";

    if (caracteres && simbolos && estructura)
        cout << "EXPRESION ACEPTADA";
    else
        cout << "EXPRESION RECHAZADA";

    cout << "\n====================================\n";
}


int main() {
    // Ingresar 5 numeros que sean pares y mostrarlos en pantalla
    for (int i = 0; i < 5; i++) {
        cout << "Ingrese un numero par: ";
        cin >> numero;
        if (validarNumeroPar(numero)) {
            numerosPares.push_back(numero);
        } else {
            cout << "El numero ingresado no es par. Por favor, ingrese un numero par." << endl;
            i--; // Decrementar el contador para repetir la iteración
        }
           
    }

    cout << mostrarNumeros(numerosPares) << endl;

    while (true) {
        cout << "Ingrese un numero par para calcular el promedio de los numeros mayores: ";
        cin >> numero;
        if (validarNumeroPar(numero)) {
            string promedioResultado = calcularPromedioMayores(numerosPares, numero);
            cout << promedioResultado << endl;
            break;
        } else {
            cout << "Entrada invalida. Por favor, ingrese un numero par." << endl;
        }
    }

    cout << mostrarNumerosAlReves(numerosPares) << endl;

    cout << "Ingrese una palabra que comience con mayuscula y termine con mayuscula: ";
    cin >> palabra;
    while (!regex_match(palabra, patronPalabra)) {
        cout << "La palabra ingresada no es valida. Debe comenzar con una letra mayuscula y terminar con una letra mayuscula." << endl;
        cout << "Ingrese una palabra que comience con mayuscula y termine con mayuscula: ";
        cin >> palabra;
    }

    string expresion;

    cout << "====================================\n";
    cout << "     VALIDADOR DE EXPRESIONES\n";
    cout << "====================================\n";

    cout << "\nDigite una expresion: ";
    getline(cin, expresion);

    // ---------------------------------------------
    // PRIMERA REVISION
    // ---------------------------------------------

    bool caracteres = caracteresCorrectos(expresion);

    if (!caracteres) {

        resultado(false, false, false);

        return 0;
    }

    // ---------------------------------------------
    // SEGUNDA REVISION
    // ---------------------------------------------

    bool simbolos = simbolosCorrectos(expresion);

    if (!simbolos) {

        resultado(true, false, false);

        return 0;
    }

    // ---------------------------------------------
    // TERCERA REVISION
    // ---------------------------------------------

    bool estructura = estructuraCorrecta(expresion);

    // ---------------------------------------------
    // RESULTADO
    // ---------------------------------------------

    resultado(caracteres, simbolos, estructura);

   
    
    return 0;
}