#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <tuple>

using namespace std;

//Funciones

bool validarUsuario(const string& usuario)
{
    regex patron("^[a-z][a-z0-9]{3,}$");

    return regex_match(usuario, patron);}

bool validarCodigo(const string& codigo)
{
    regex patron("^[0-9]{7}$");

    return regex_match(codigo, patron);

}

bool validarCorreo(const string& correo)
{
    regex patron("^[a-z0-9]+@gmail\\.com$");

    return regex_match(correo, patron);
}

bool validarTelefono(const string& telefono)
{
    regex patron("^[0-9]{10}$");

    return regex_match(telefono, patron);
}


void mostrarResultados(const vector<tuple<string, string, string, bool>>& resultados) //procedimiento que no retorna, no es una func 
{
    cout << "\n================================\n";
    cout << "        Resultados de validacion\n";
    cout << "================================\n\n";

    for (const auto& resultado : resultados) {
        const string& campo = get<0>(resultado);
        const string& valor = get<1>(resultado);
        const string& patron = get<2>(resultado);
        bool esValido = get<3>(resultado);
        

        cout << "Campo: " << campo << endl;
        cout << "Valor ingresado: " << valor << endl;
        cout << "Patron: " << patron << endl;
        cout << "Resultado: " << (esValido ? "Valido" : "Invalido") << endl;
        cout << "--------------------------------\n";
    }
}



int main()
{
    string usuario, codigo, correo, telefono;

    cout << "================================\n";
    cout << "    REGISTRO DE ESTUDIANTE \n";
    cout << "================================\n\n";

    cout << "Ingrese su usuario: ";
    cin >> usuario;

    cout << "Ingrese su codigo: ";
    cin >> codigo;


    cout << "Ingrese su correo: ";
    cin >> correo;

    cout << "Ingrese su telefono: ";
    cin >> telefono;

    // VECTOR CON TUPLAS:

    vector<tuple<string, string, string, bool>> resultados;

    //usuario
    resultados.push_back({
        "Usuario",
        usuario,
        "^[a-z][a-z0-9]{3,}$",
        validarUsuario(usuario)
    });

    //codigo
    resultados.push_back({
        "Codigo",
        codigo,
        "^[0-9]{7}$",
        validarCodigo(codigo)
    }); 

    //correo
    resultados.push_back({
        "Correo",
        correo,
        "^[a-z0-9]+@gmail\\.com$",
        validarCorreo(correo)
    }); 

    //telefono
    resultados.push_back({
        "Telefono",  
        telefono,
        "^[0-9]{10}$",
        validarTelefono(telefono)
    });
    
    mostrarResultados(resultados);

    bool todosValidos = true;
    for (const auto& resultado : resultados) {
        if (!get<3>(resultado)) {
            todosValidos = false;
            break;  
        }
    };

    cout << "\n" << (todosValidos ? "Todos los campos son validos." : "Algunos campos son invalidos.") << endl;
}