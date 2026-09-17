#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

using namespace std;

regex patronDano("^[A-Z][A-Za-z0-9]+(?: [A-Za-z0-9]{2,})*$");
regex patronDepartamento("^[A-Z][A-Za-z]+$");
regex partronMunicipio("^[A-Z][A-Za-z]+(?: [A-Za-z]{2,})*$");
regex patronDescripcion("^[A-Z][A-Za-z]+(?: [A-Za-z]+)*$");
regex patronDireccion("^[A-Z][A-Za-z0-9]*(?: [A-Za-z0-9]+)*$");
regex patronContador("^[0-9]+$");
regex patronNombre("^[A-Z][A-Za-z]*(?: [A-Za-z]{2,})*$");
regex partronTelefono("^[0-9]{10}$");

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

void mostrarResultados(const vector<tuple<string, string, string, bool>>& resultados) {
    cout << "\n================================\n";
    cout << "        Daño reportado\n";
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

int main() {

    cout << "\n================================\n";
    cout << "        Reporte de Daño\n";
    cout << "================================\n\n";
    
    string dano = pedirDato(
        "Ingrese el tipo de daño: ",
        patronDano,
        "Dano invalido. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string departamento = pedirDato(
        "Ingrese el departamento: ",
        patronDepartamento,
        "Departamento invalido. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string municipio = pedirDato(
        "Ingrese el municipio: ",
        partronMunicipio,
        "Municipio invalido. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string descripcion = pedirDato(
        "Ingrese la descripcion del daño: ",
        patronDescripcion,
        "Descripcion invalida. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string direccion = pedirDato(
        "Ingrese la direccion: ",
        patronDireccion,
        "Direccion invalida. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string detalle = pedirDato(
        "Ingrese el detalle de la dirección: ",
        patronDescripcion,
        "Detalle invalido. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string contador = pedirDato(
        "Ingrese el numero de la tapa del contador: ",
        patronContador,
        "Contador invalido. Debe ser un numero."
    );

    string instalacion = pedirDato(
        "Ingrese la instalacion: ",
        patronContador,
        "Instalacion invalida. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string nombre = pedirDato(
        "Ingrese el nombre completo del titular: ",
        patronNombre,
        "Nombre invalido. Debe comenzar con una letra mayuscula y contener solo caracteres alfanumericos."
    );

    string telefono = pedirDato(
        "Ingrese el telefono de contacto: ",
        partronTelefono,
        "Telefono invalido. Debe ser un numero de 10 digitos."
    );

    vector<tuple<string, string, string, bool>> resultados;

    resultados.push_back({
        "Daño",
        dano,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(dano, patronDano)
    });

    resultados.push_back({
        "Departamento",
        departamento,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(departamento, patronDepartamento)
    });

    resultados.push_back({
        "Municipio",
        municipio,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(municipio, partronMunicipio)
    });

    resultados.push_back({
        "Descripcion",
        descripcion,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(descripcion, patronDescripcion)
    });

    resultados.push_back({
        "Direccion",
        direccion,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(direccion, patronDireccion)
    });

    resultados.push_back({
        "Detalle direccion",
        detalle,
        "^[A-Z][A-Za-z0-9 ]*$",
        regex_match(detalle, patronDescripcion)
    });

    resultados.push_back({
        "Numero de contador",
        contador,
        "^[0-9]+$",
        regex_match(contador, patronContador)
    });

    resultados.push_back({
        "Instalacion",
        instalacion,
        "^[0-9]+$",
        regex_match(instalacion, patronContador)
    });

    resultados.push_back({
        "Nombre",
        nombre,
        "^[A-Z][A-Za-z ]*$",
        regex_match(nombre, patronNombre)
    });

    resultados.push_back({
        "Telefono",
        telefono,
        "^[0-9]{10}$",
        regex_match(telefono, partronTelefono)
    });

    mostrarResultados(resultados);







}

