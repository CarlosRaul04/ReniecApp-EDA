#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include "Persona.h"  // Incluir el archivo de encabezado para la clase Persona

using namespace std;
using namespace std::chrono;

// Tabla de Cuckoo Hash modificada solo para almacenar DNIs
template <typename K>
class CuckooHash {
private:
    vector<K> table1;
    vector<K> table2;
    size_t size1, size2;
    float loadFactor;
    size_t totalRegistros;

    // Función de hash para la primera tabla
    size_t hash1(K key) {
        return key % size1;
    }

    // Función de hash para la segunda tabla
    size_t hash2(K key) {
        return (key / size2) % size2;
    }

    // Rehashing para la primera tabla
    void rehash1(K key) {
        size_t index = hash1(key);
        if (table1[index] != 0) {
            auto temp = table1[index];
            table1[index] = key;
            rehash2(temp);
        } else {
            table1[index] = key;
        }
        totalRegistros++;
        verificarFactorDeCarga();
    }

    // Rehashing para la segunda tabla
    void rehash2(K key) {
        size_t index = hash2(key);
        if (table2[index] != 0) {
            auto temp = table2[index];
            table2[index] = key;
            rehash1(temp);
        } else {
            table2[index] = key;
        }
        totalRegistros++;
        verificarFactorDeCarga();
    }

    // Verificar el factor de carga y redimensionar las tablas si es necesario
    void verificarFactorDeCarga() {
        float actualLoadFactor = float(totalRegistros) / (size1 + size2);
        if (actualLoadFactor > loadFactor) {
            cout << "El factor de carga ha superado el umbral. Redimensionando tablas..." << endl;
            redimensionarTablas();
        }
    }

    // Redimensionar las tablas cuando se supera el factor de carga
    void redimensionarTablas() {
        size_t nuevaSize1 = ceil(size1 * 1.5);
        size_t nuevaSize2 = ceil(size2 * 1.5);

        vector<K> nuevaTable1(nuevaSize1, 0);
        vector<K> nuevaTable2(nuevaSize2, 0);

        for (size_t i = 0; i < size1; ++i) {
            if (table1[i] != 0) {
                rehash1(table1[i]);
            }
        }
        for (size_t i = 0; i < size2; ++i) {
            if (table2[i] != 0) {
                rehash2(table2[i]);
            }
        }

        size1 = nuevaSize1;
        size2 = nuevaSize2;
        table1 = move(nuevaTable1);
        table2 = move(nuevaTable2);
    }

public:
    CuckooHash(size_t s1, size_t s2, float lf = 0.7) : size1(s1), size2(s2), loadFactor(lf), totalRegistros(0) {
        table1.resize(size1, 0);
        table2.resize(size2, 0);
    }

    void insert(K key) {
        rehash1(key);
    }

    bool search(K key) {
        // Calcular índices directamente
        size_t index1 = hash1(key);
        size_t index2 = hash2(key);

        // Comprobar ambas posiciones en las tablas
        return table1[index1] == key || table2[index2] == key;
    }

    void remove(K key) {
        size_t index1 = hash1(key);
        if (table1[index1] == key) {
            table1[index1] = 0;
            totalRegistros--;
            return;
        }

        size_t index2 = hash2(key);
        if (table2[index2] == key) {
            table2[index2] = 0;
            totalRegistros--;
            return;
        }
    }
};

// Función para realizar búsqueda binaria en el archivo binario de personas
bool buscarPersonaBinaria(const string& nombreArchivo, uint32_t dni, Persona& persona) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    archivo.seekg(0, ios::end);
    size_t totalRegistros = archivo.tellg() / sizeof(Persona);
    archivo.seekg(0, ios::beg);

    size_t inicio = 0, fin = totalRegistros - 1;
    while (inicio <= fin) {
        size_t medio = inicio + (fin - inicio) / 2;
        archivo.seekg(medio * sizeof(Persona), ios::beg);
        archivo.read(reinterpret_cast<char*>(&persona), sizeof(Persona));

        if (persona.dni == dni) {
            if (persona.activo) {
                return true; // Persona encontrada y activa
            } else {
                cout << "La persona no existe o ya ha sido eliminada." << endl;
                return false;
            }
        } else if (persona.dni < dni) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return false; // Persona no encontrada o inactiva
}


// Función para mostrar la información de una persona
void mostrarPersona(const Persona& persona) {
    cout << "DNI: " << persona.dni << endl;
    cout << "Nombres: " << persona.nombres[0].data() << " " << persona.nombres[1].data() << endl;
    cout << "Apellidos: " << persona.apellidos[0].data() << " " << persona.apellidos[1].data() << endl;
    cout << "Nacionalidad: " << persona.nacionalidad.data() << endl;
    cout << "Lugar de Nacimiento: " << persona.lugarNacimiento.data() << endl;
    cout << "Direccion: " << persona.direccion.data() << endl;
    cout << "Telefono: " << persona.telefono.data() << endl;
    cout << "Correo: " << persona.correo.data() << endl;
    cout << "Estado Civil: " << persona.estadoCivil.data() << endl;
}

// Función para agregar una nueva persona en el archivo binario
void insertarPersona(CuckooHash<uint32_t>& cuckooHash, const string& nombreArchivo, unordered_map<uint32_t, Persona>& cache) {
    Persona persona;

    cout << "Ingrese el DNI: ";
    cin >> persona.dni;
    cin.ignore();

    // Verificar si el DNI ya existe en la tabla hash
    if (cuckooHash.search(persona.dni)) {
        cout << "Error: El DNI ya existe en la base de datos." << endl;
        return;
    }

    // Pedir los datos restantes
    cout << "Ingrese los nombres: ";
    cin.getline(persona.nombres[0].data(), 15);
    cin.getline(persona.nombres[1].data(), 15);

    cout << "Ingrese los apellidos: ";
    cin.getline(persona.apellidos[0].data(), 20);
    cin.getline(persona.apellidos[1].data(), 20);

    cout << "Ingrese la nacionalidad: ";
    cin.getline(persona.nacionalidad.data(), 20);

    cout << "Ingrese el lugar de nacimiento: ";
    cin.getline(persona.lugarNacimiento.data(), 20);

    cout << "Ingrese la direccion: ";
    cin.getline(persona.direccion.data(), 100);

    cout << "Ingrese el telefono: ";
    cin >> persona.telefono.data();
    cin.ignore();

    cout << "Ingrese el correo: ";
    cin.getline(persona.correo.data(), 40);

    cout << "Ingrese el estado civil: ";
    cin.getline(persona.estadoCivil.data(), 20);

    // Escribir la persona en el archivo binario
    ofstream archivo(nombreArchivo, ios::binary | ios::app);
    if (!archivo) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return;
    }

    archivo.write(reinterpret_cast<const char*>(&persona), sizeof(Persona));
    cuckooHash.insert(persona.dni); // Insertar en la tabla hash
    cache[persona.dni] = persona;  // Agregar al caché
    cout << "Persona agregada correctamente." << endl;
}

// Función para eliminar una persona del archivo binario
bool eliminarPersonaEnArchivo(const string& nombreArchivo, uint32_t dni) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    Persona persona;
    bool encontrado = false;

    // Iterar sobre el archivo y buscar la persona
    while (archivo.read(reinterpret_cast<char*>(&persona), sizeof(Persona))) {
        if (persona.dni == dni) {
            persona.activo = false; // Marcar como inactiva
            // Mover el puntero del archivo al inicio de la posición actual para sobrescribir
            archivo.seekp(-static_cast<int>(sizeof(Persona)), ios::cur);
            archivo.write(reinterpret_cast<const char*>(&persona), sizeof(Persona));
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Persona no encontrada en el archivo." << endl;
    }

    return encontrado;
}



int main() {
    CuckooHash<uint32_t> cuckooHash(33000000, 22000000); // Iniciar tabla Cuckoo con tamaño inicial
    unordered_map<uint32_t, Persona> cache;  // Cache para personas
    const string nombreArchivo = "registros.bin";

    // Cargar DNIs en hash desde archivo
    Persona persona;
    ifstream archivo(nombreArchivo, ios::binary);
    while (archivo.read(reinterpret_cast<char*>(&persona), sizeof(Persona))) {
        cuckooHash.insert(persona.dni);
    }

    // Menú de opciones
    while (true) {
        cout << "\nMenu� de Opciones: " << endl;
        cout << "1. Buscar persona" << endl;
        cout << "2. Agregar persona" << endl;
        cout << "3. Eliminar persona" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opcion: ";
        int opcion;
        cin >> opcion;
        while (cin.fail()) {
            cin.clear(); // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inválida
            cout << "Entrada invalida. Por favor, ingrese una opcion valida: ";
            cin >> opcion; // Reintentar la entrada
        }

        if (opcion == 1) {
            uint32_t dni;
            cout << "Ingrese el DNI a buscar: ";
            cin >> dni;
            while (cin.fail()) {
                cin.clear(); // Limpiar el estado de error
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inválida
                cout << "Entrada invalida. Por favor, ingrese un DNI valido: ";
                cin >> dni; // Reintentar la entrada
            }

            auto start = high_resolution_clock::now();

            if (!cuckooHash.search(dni)) {
                cout << "El DNI no esta� registrado en el sistema. No se puede realizar la busqueda." << endl;
            } else {
                // Si el DNI está en el Cuckoo Hash, realizar la búsqueda
                if (cache.find(dni) != cache.end()) {
                    mostrarPersona(cache[dni]);
                } else {
                    if (buscarPersonaBinaria(nombreArchivo, dni, persona)) {
                        mostrarPersona(persona);
                        cache[dni] = persona;  // Almacenar en el cache
                    } else {
                        cout << "Persona no encontrada." << endl;
                    }
                }
            }

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "Tiempo de b�squeda: " << duration.count() << "ms" << endl;
        } else if (opcion == 2) {
            insertarPersona(cuckooHash, nombreArchivo, cache);
        } else if (opcion == 3) {
            uint32_t dni;
            cout << "Ingrese el DNI de la persona a eliminar: ";
            cin >> dni;
            while (cin.fail()) {
                cin.clear(); // Limpiar el estado de error
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inválida
                cout << "Entrada invalida. Por favor, ingrese un DNI valido: ";
                cin >> dni; // Reintentar la entrada
            }

            auto start = high_resolution_clock::now();

            if (eliminarPersonaEnArchivo(nombreArchivo, dni)) {
                cuckooHash.remove(dni);  // Eliminar del cuckoo hash
                cache.erase(dni);  // Eliminar del cache
                cout << "Persona eliminada correctamente." << endl;
            }

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << "Tiempo de Eliminacion: " << duration.count() << "ms" << endl;

        } else if (opcion == 4) {
            break;
        } else {
            cout << "Opci�n no v�lida." << endl;
        }
    }

    return 0;
}
