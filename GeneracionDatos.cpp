#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <chrono>
#include "Persona.h"

using namespace std;
using namespace std::chrono;

void exportarABinario(const vector<Persona>& personas, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary | ios::app);
    if (!archivo) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return;
    }
    for (const auto& persona : personas) {
        archivo.write(reinterpret_cast<const char*>(&persona), sizeof(Persona));
    }
    archivo.close();
}

void generarYExportarDatos(const string& nombreArchivo, int cantidad, int tamanoLote = 100000) {
    uint32_t dni_inicial = 10000000;
    vector<Persona> personas;
    personas.reserve(tamanoLote);

    for (int i = 0; i < cantidad; ++i) {
        Persona persona;
        persona.dni = dni_inicial + i;
        Persona::generarNombres(persona.nombres, i);
        Persona::generarApellidos(persona.apellidos);
        Persona::copiarStringAArray("Peruº", persona.nacionalidad);
        Persona::generarLugarNacimiento(persona.lugarNacimiento);
        Persona::generarDireccion(persona.direccion);
        Persona::generarTelefono(persona.telefono);  // AquÃ­ estamos llamando correctamente a la funciÃ³n
        Persona::generarCorreo(persona.nombres, persona.correo);
        Persona::generarEstadoCivil(persona.estadoCivil);
        persona.activo = true;

        personas.push_back(persona);

        if ((i + 1) % 1000000 == 0) {
            cout << "Generados " << (i + 1) << " registros..." << endl;
        }

        if (personas.size() == tamanoLote || i == cantidad - 1) {
            exportarABinario(personas, nombreArchivo);
            personas.clear();
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int cantidad = 33000000; //Cantidad de ciudadanos a generar

    auto start = high_resolution_clock::now();


    cout << "Generando " << cantidad << " registros, por favor espere...\n";
    generarYExportarDatos("registros.bin", cantidad);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);

    cout << "Datos generados y exportados correctamente." << endl;
    cout << "Tiempo de ejecucion: " << duration.count() << " segundos" << endl;

    return 0;
}
