// FDA - Finite Deterministic Automaton termina en 'a'
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string cadena;
    cout << "Ingrese una cadena de texto: ";
    cin >> cadena;

    int estado = 0; // Estado inicial: q0

    for (char simbolo : cadena)
    {
        switch (estado)
        {
            case 0: // Estado q0
                if (simbolo == 'a')
                    estado = 1; // Transición a q1
                else 
                    estado = 0; // Permanece en q0
                break;

            case 1: // Estado q1
                if (simbolo == 'a')
                    estado = 1; // Permanece en q1
               
                else
                    estado = 0; // Transición a q0
                break;
        }
    }

    // Verificar estado final
    if (estado == 1)
        cout << "Cadena aceptada\n";
    else
        cout << "Cadena no aceptada\n";

    return 0;
}



// FDA - Finite Deterministic Automaton par o impar 
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string cadena;
    cout << "Ingrese un numero: ";
    cin >> cadena;

    int estado = 0; // Estado inicial: q0 


    for (char simbolo : cadena)
    {
        switch (estado)
        {
            case 0: // Estado q0
                if (simbolo == '0')
                    estado = 1; // Transición a q1
                else 
                    estado = 0; // Permanece en q0
                break;

            case 1: // Estado q1
                if (simbolo == '0')
                    estado = 0; // Permanece en q1
             
                else
                    estado = 1; // Transición a q0
                break;
        }
    }

    // Verificar estado final
    if (estado == 0)
        cout << "Cadena aceptada\n";
    else
        cout << "Cadena no aceptada\n";

    return 0;
}

//TALLER  PUNTO 1
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    string cadena;

    cout << "Ingrese el registro de asistencia: ";
    cin >> cadena;
    
    for (char &caracter : cadena) {
        caracter = toupper(caracter);

    }

    int estado = 0; // q0: no ha aparecido P

    for (char asistencia : cadena) {

        switch (estado) {

            case 0: // q0: no ha aparecido P

                if (asistencia == 'P')
                    estado = 1;

                else if (asistencia == 'A')
                    estado = 0;

                else
                    cout << "Ingrese una cadena solo con el alfabeto {A, P}";

                break;

            case 1: // q1: ya apareció P

                if (asistencia == 'A')
                    estado = 1;

                else if (asistencia == 'P')
                    estado = 1;

                else
                    cout << "Ingrese una cadena solo con el alfabeto {A, P}";

                break;
        }
    }

    if (estado == 1)
        cout << "Registro valido: Hubo al menos una asistencia.";
    else
        cout << "Registro invalido: No hubo asistencia.";

    return 0;
}

//TALLER PUNTO 2
#include <iostream>
#include <string>

using namespace std;

int main() {
    string codigo;

    cout << "Ingrese el codigo de acceso: ";
    cin >> codigo;
    

    int estado = 0; // q0: no ha aparecido 0

    for (char c : codigo) {

        switch (estado) {

            case 0: // q0: no ha aparecido 0

                if (c == '0')
                    estado = 1;
                else if (c == '1')
                    estado = 0;
                else
                    cout << "Ingrese un codigo solo con el alfabeto {1, 0}";
                break;

            case 1: // q1: ya apareció 0, esperando 1

                if (c == '0')
                    estado = 1;
                else if (c == '1')
                    estado = 2;
                else
                    cout << "Ingrese un codigo solo con el alfabeto {1, 0}";
                break;
            
            case 2: //q2: Termina en 01
            
                if (c=='0')
                    estado =1;
                else if (c =='1')
                    estado=0;
                else
                    cout << "Ingrese un codigo solo con el alfabeto {1, 0}";
                break;
        }
    }

    if (estado == 2)
        cout << "Registro valido: El codigo termina en 01.";
    else
        cout << "Registro invalido.";

    return 0;
}