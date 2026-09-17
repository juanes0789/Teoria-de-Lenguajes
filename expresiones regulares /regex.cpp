// 

#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

using namespace std;

string pedirDato(
    const string& mensaje,
    const regex& patron,
    const string& mensajeError
) {
    string dato;

    while (true) {
        cout << mensaje;
        cin >> dato;

        if (regex_match(dato, patron)) {
            cout << "Valido" << endl;
            return dato;
        }

        cout << mensajeError << endl;
    }
}

regex patronNombre("^[a-zA-Z]{3,}$");
regex patronApellido("^[a-zA-Z]{2,}$");
regex patronCorreo("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z0-9]+$");
regex patronDocumento("^[0-9]+$");
regex patronDependencia("^[a-zA-Z ]+$");
regex patronPregrado("^[a-zA-Z ]+$");
regex patronCampoOtro("^[a-zA-Z0-9 ]+$");
regex patronCampus("^[a-zA-Z ]+$");

string pedirLista(const string& titulo, const vector<string>& opciones) {
    while (true) {
        cout << titulo << endl;
        for (size_t i = 0; i < opciones.size(); ++i) {
            cout << (i + 1) << ". " << opciones[i] << endl;
        }
        cout << "Opcion: ";

        string entrada;
        cin >> entrada;

        if (regex_match(entrada, regex("^[1-9][0-9]*$"))) {
            int indice = stoi(entrada) - 1;

            if (indice >= 0 && indice < static_cast<int>(opciones.size())) {
                cout << "Valido" << endl;
                return opciones[indice];
            }
        }

        cout << "Opcion invalida. Debe seleccionar un numero dentro de la lista." << endl;
    }
}

int main() {

  
    

    cout << "================================\n";
    cout << "    REGISTRO CURSO DE GESTION\n";
    cout << "================================\n\n";

    while (true) {

        int opcion;

        cout << "\nMENU DE OPCIONES\n";
        cout << "1. Registrarme al curso\n";
        cout << "2. Salir\n";
        cout << "Seleccione una opcion: ";

        while (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcion invalida. Ingrese solo un numero (1 o 2).\n";
            cout << "Seleccione una opcion: ";
        }

        switch (opcion) {

        case 1: {

            string nombre = pedirDato(
                "Nombre: ",
                patronNombre,
                "Nombre invalido. Debe comenzar con una letra y tener al menos 3 caracteres alfanumericos."
            );

            string apellido = pedirDato(
                "Apellido: ",
                patronApellido,
                "Apellido invalido. Debe comenzar con una letra y tener al menos 2 caracteres alfanumericos."
            );

            string correo = pedirDato(
                "Correo: ",
                patronCorreo,
                "Correo invalido. Debe tener el formato usuario@correo.com"
            );

            vector<string> opcionesTipoDocumento = {"CC", "TI", "PA"};
            string tipoDocumento = pedirLista(
                "Seleccione el tipo de documento:",
                opcionesTipoDocumento
            );

            string documento = pedirDato(
                "Documento: ",
                patronDocumento,
                "Documento invalido. Debe ser un numero."
            );

            vector<string> opcionesVinculacion = {"Docente vinculado", "Externo", "Administrativo"};
            string vinculacion = pedirLista(
                "Seleccione la vinculacion:",
                opcionesVinculacion
            );

            vector<string> opcionesDependencia = {"Facultad de Artes", "Facultad de Educación", "Facultad de Ingenieria", "Otro"};
            string dependencia = pedirLista(
                "Seleccione la dependencia:",
                opcionesDependencia
            );

            if (dependencia == "Otro") {
                dependencia = pedirDato(
                    "Si no encuentra su dependencia en el listado, puede indicarnos aquí: ",
                    patronCampoOtro,
                    "Campo invalido. Debe contener solo caracteres alfanumericos y espacios."
                );
            }

            vector<string> opcionesPregrado = {"Ingenieria de Sistemas", "Arte dramatico", "Licenciatura en Educacion", "Otro"};
            string pregrado = pedirLista(
                "Seleccione el pregrado:",
                opcionesPregrado
            );


            string otroPregrado = "Ninguno";
            if (pregrado == "Otro") {
                otroPregrado = pedirDato(
                    "Si no encuentra su pregrado en el listado, puede indicarnos aquí: ",
                    patronCampoOtro,
                    "Campo invalido. Debe contener solo caracteres alfanumericos y espacios."
                );
            }

            string otroPosgrado = "Ninguno";
            if (pregrado == "Otro") {
                otroPosgrado = pedirDato(
                    "Si no encuentra su posgrado en el listado, puede indicarnos aquí: ",
                    patronCampoOtro,
                    "Campo invalido. Debe contener solo caracteres alfanumericos y espacios."
                );
            }

            vector<string> opcionesCampus = {"Ciudad Universitaria (Medellín)", "Uraba", "Posgrados"};
            string campus = pedirLista(
                "Seleccione el campus:",
                opcionesCampus
            );

           

            cout << "\n================================\n";
            cout << "        Datos registrados\n";
            cout << "================================\n\n";

            cout << "Nombre: " << nombre << endl;
            cout << "Apellido: " << apellido << endl;
            cout << "Correo: " << correo << endl;
            cout << "Tipo de documento: " << tipoDocumento << endl;
            cout << "Documento: " << documento << endl;
            cout << "Vinculacion: " << vinculacion << endl;
            cout << "Pregrado: " << pregrado << endl;
            cout << "Otro pregrado: " << otroPregrado << endl;
            cout << "Otro posgrado: " << otroPosgrado << endl;
            cout << "Dependencia: " << dependencia << endl;
            cout << "Campus: " << campus << endl;
            cout << "\nRegistro exitoso." << endl;

            break;
        }

        case 2:

            cout << "\nSaliendo del programa..." << endl;

            return 0;


        default:

            cout << "\nOpcion invalida. Seleccione 1 o 2." << endl;

            break;
        }
    }

    return 0;
}