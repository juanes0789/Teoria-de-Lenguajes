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