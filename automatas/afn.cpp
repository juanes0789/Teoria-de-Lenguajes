// Cadenas que contienen al menos una “a”, en el alfabeto {a,b}
#include <iostream>
#include <string>

using namespace std;

int main()
{
    char pregunta = 'S';

    while (true)
    {
        string cadena;

        cout << "Ingrese una cadena: ";
        cin >> cadena;

        // Estados actuales
        bool q0 = true; // Estado inicial
        bool q1 = false;

        // Recorrer la cadena
        for (char simbolo : cadena)
        {
            // Guardamos los nuevos estados
            bool nuevo_q0 = false;
            bool nuevo_q1 = false;

            // Transiciones desde q0
            if (q0)
            {
                if (simbolo == 'a')
                {
                    nuevo_q0 = true;
                    nuevo_q1 = true;
                }
                else if (simbolo == 'b')
                {
                    nuevo_q0 = true;
                }
            }

            // Transiciones desde q1
            if (q1)
            {
                if (simbolo == 'a')
                {
                    nuevo_q1 = true;
                }
                else if (simbolo == 'b')
                {
                    nuevo_q1 = true;
                }
            }

            // Actualizamos los estados
            q0 = nuevo_q0;
            q1 = nuevo_q1;
        }

        // q1 estado final
        if (q1)
            cout << "Cadena ACEPTADA" << endl;
        else
            cout << "Cadena RECHAZADA" << endl;

        cout << "¿Desea ingresar otra cadena? (S/N): ";
        cin >> pregunta;
        if (pregunta == 'N')
            break;
    }
    return 0;
}

// Cadenas que contienen al menos una “a”, en el alfabeto {a,b}
#include <iostream>
#include <string>

using namespace std;

int main()
{
   char continuar = 'p';
   

   while(continuar == 'p'){
    string cadena;
    cout << "Ingrese una cadena:";
    cin>> cadena;
    bool q0 = true;
    bool q1 = false;
    bool q2 = false;

    for(char simbolo : cadena){
        bool nuevo_q0=false;
        bool nuevo_q1=false;
        bool nuevo_q2=false;

        if(q0){
            if(simbolo== 'a'){
                nuevo_q0=true;
                nuevo_q1=true;
            }
            else if(simbolo == 'b'){
                nuevo_q0= true;

            }
        }
        if(q1){
            if(simbolo == 'a'){
                nuevo_q1 = true;
            }else if(simbolo == 'b'){
                nuevo_q2 = true;
            }
        }
        if(q2){
            nuevo_q2 = true;
        }

        q0 = nuevo_q0;
        q1 = nuevo_q1;
        q2 = nuevo_q2;

        

    }
   
    if (q2){
        cout<< "cadena aceptada"<<'\n';
    }else{
        cout << "Cadena rechazada";
    }

    cout << "Pulse 'p' para continuar, de lo contrario otra cosa.";
    cin>> continuar;
   

    return 0;
   }
}

// Cadenas que contienen al menos una “a”, en el alfabeto {a,b}
#include <iostream>
#include <string>

using namespace std;

int main()
{
    char continuar = 'S';
    while(continuar == 'S'){

        string cadena;
        cout<<"Ingrese la cadena: ";
        cin>>cadena;

        bool q0 = true; // Estado inicial
        bool q1 = false;
        bool q2 = false;

        for(char simbolo : cadena){
            bool nuevo_q0 = false;
            bool nuevo_q1 = false;
            bool nuevo_q2 = false;

            if(q0){ 
                if(simbolo == 'a' ){
                    nuevo_q0 = true;
                    nuevo_q1 = true;
                }
                else if(simbolo == 'b'){
                    nuevo_q2 = true;
                }
                else if(simbolo == 'c'){
                    nuevo_q0 = true;
                    break;
                }
            }

             if(q1){
                nuevo_q1 = true;
            }

             if(q2) {
                nuevo_q2 = true;
            }

            q0 = nuevo_q0;
            q1 = nuevo_q1;
            q2 = nuevo_q2;
        }

        if(q2 || q1){
                cout<<"Cadena aceptada";
            }
        else {
                cout<<"Rechazada";
            }

        cout << "\n\n¿Desea ingresar otra cadena? (S/N): ";
        cin >> continuar;
            if (continuar == 'N')
                break;
    }
     return 0;
}