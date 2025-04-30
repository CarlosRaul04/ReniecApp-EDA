#ifndef PERSONA_H
#define PERSONA_H
#include <array>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstdlib>

class Persona {
public:
    uint32_t dni;
    std::array<std::array<char, 15>, 2> nombres;  // Almacenar� dos nombres (nombre1, nombre2)
    std::array<std::array<char, 20>, 2> apellidos;  // Almacenar� dos apellidos (apellido1, apellido2)
    std::array<char, 100> lugarNacimiento;
    std::array<char, 100> direccion;
    std::array<char, 20> nacionalidad;
    std::array<char, 15> telefono;
    std::array<char, 50> correo;
    std::array<char, 10> estadoCivil;
    bool activo = true;

    // Generaci�n de nombres con sexo basado en �ndice (par = mujer, impar = hombre)
    static void generarNombres(std::array<std::array<char, 15>, 2>& nombres, int indice) {
        const char* nombresHombres[] = {"Carlos", "Luis", "Pedro", "Fernando", "Jose", "Miguel"};
        const char* nombresMujeres[] = {"Ana", "Maria", "Elena", "Laura", "Carla", "Paula"};
        const int numHombres = sizeof(nombresHombres) / sizeof(nombresHombres[0]);
        const int numMujeres = sizeof(nombresMujeres) / sizeof(nombresMujeres[0]);

        // Generar nombre masculino o femenino seg�n el �ndice (impar = hombre, par = mujer)
        std::string nombre1;
        if (indice % 2 == 0) {
            nombre1 = nombresMujeres[indice % numMujeres];  // Mujer
        } else {
            nombre1 = nombresHombres[indice % numHombres];  // Hombre
        }

        // Para el segundo nombre, se genera de manera similar
        std::string nombre2;
        if (indice % 2 == 0) {
            nombre2 = nombresMujeres[(indice + 1) % numMujeres];
        } else {
            nombre2 = nombresHombres[(indice + 1) % numHombres];
        }

        copiarStringAArray(nombre1.c_str(), nombres[0]);
        copiarStringAArray(nombre2.c_str(), nombres[1]);
    }

    // Generaci�n de apellidos (dos apellidos)
    static void generarApellidos(std::array<std::array<char, 20>, 2>& apellidos) {
        const char* apellidosPredefinidos[] = {"Garcia", "Lopez", "Martinez", "Perez", "Sanchez"};
        const int numApellidos = sizeof(apellidosPredefinidos) / sizeof(apellidosPredefinidos[0]);

        std::string apellido1 = apellidosPredefinidos[rand() % numApellidos];
        std::string apellido2 = apellidosPredefinidos[rand() % numApellidos];

        copiarStringAArray(apellido1.c_str(), apellidos[0]);
        copiarStringAArray(apellido2.c_str(), apellidos[1]);
    }

    static void generarLugarNacimiento(std::array<char, 100>& lugarNacimiento) {
        const char* lugares[] = {"Lima", "Arequipa", "Cusco", "Trujillo", "Piura"};
        const int numLugares = sizeof(lugares) / sizeof(lugares[0]);
        std::string lugar = lugares[rand() % numLugares];
        copiarStringAArray(lugar.c_str(), lugarNacimiento);
    }

    static void generarDireccion(std::array<char, 100>& direccion) {
        const std::array<const char*, 10> departamentos = {"Lima", "Cusco", "Arequipa", "Piura", "La Libertad", "Jun�n", "Ica", "Puno", "San Mart�n", "Loreto"};
        const std::array<const char*, 10> ciudades = {"Miraflores", "Wanchaq", "Yanahuara", "Bellavista", "V�ctor Larco", "El Tambo", "Parcona", "San Rom�n", "Morales", "Punchana"};
        const std::array<const char*, 10> calles = {"Calle Los �lamos", "Calle San Andr�s", "Avenida Los �lamos", "Calle La Palma", "Jir�n Los Pinos","Avenida Jos� Galvez", "Calle La Marina", "Calle Los Cedros", "Jir�n Grau", "Calle Los Rosales"};
        const std::array<const char*, 10> provincias = {"Lima", "Cusco", "Arequipa", "Sullana", "Trujillo", "Huancayo", "Ica", "Juliaca", "Tarapoto", "Iquitos"};
        const std::array<const char*, 10> distritos = {"Centro", "San Isidro", "Cercado", "Los Olivos", "Huanchaco", "Chilca", "Tate", "Cabana", "Moyobamba", "Bel�n"};

        std::string direccionCompleta =
        std::string(departamentos[rand() % departamentos.size()]) + ", " +
        std::string(provincias[rand() % provincias.size()]) + ", " +
        std::string(ciudades[rand() % ciudades.size()]) + ", " +
        std::string(distritos[rand() % distritos.size()]) + ", " +
        std::string(calles[rand() % calles.size()]) + " " + std::to_string(100 + rand() % 900);  // N�mero aleatorio entre 100 y 999

        copiarStringAArray(direccionCompleta.c_str(), direccion);
    }

    static void generarTelefono(std::array<char, 15>& telefono) {
        std::string tel = "+51" + std::to_string(900000000 + rand() % 100000000);
        copiarStringAArray(tel.c_str(), telefono);
    }

    static void generarCorreo(std::array<std::array<char, 15>, 2>& nombres, std::array<char, 50>& correo) {
        std::string nombreCompleto = std::string(nombres[0].data()) + std::string(nombres[1].data());
        std::string correoGenerado = nombreCompleto + "@gmail.com";
        copiarStringAArray(correoGenerado.c_str(), correo);
    }

    static void generarEstadoCivil(std::array<char, 10>& estadoCivil) {
        const char* estados[] = {"Soltero", "Casado", "Divorciado", "Viudo"};
        const int numEstados = sizeof(estados) / sizeof(estados[0]);
        std::string estado = estados[rand() % numEstados];
        copiarStringAArray(estado.c_str(), estadoCivil);
    }

    // Funci�n �nica para copiar cadenas a arrays de diferentes tama�os
    template <std::size_t N>
    static void copiarStringAArray(const char* str, std::array<char, N>& array) {
        size_t i = 0;
        for (; i < N && str[i] != '\0'; ++i) {
            array[i] = str[i];
        }
        array[i] = '\0';  // Asegura que el string en el array sea null-terminated
    }

};

#endif // PERSONA_H
