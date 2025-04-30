# 🆔 Proyecto RENIEC - Registro Nacional de Ciudadanos (C++)

Este proyecto simula una versión básica del sistema RENIEC del Perú, usando estructuras eficientes como **Cuckoo Hashing** para almacenar millones de ciudadanos por su DNI.

📌 **Objetivo principal**:  
Almacenar y gestionar hasta **33 millones de personas** con rapidez y eficiencia.

---

## 🚀 ¿Qué hace este proyecto?

- Genera millones de ciudadanos falsos con datos como nombres, apellidos, dirección, correo, teléfono, etc.
- Almacena los datos en un archivo binario (`registros.bin`) para optimizar el uso de memoria.
- Usa una estructura **Cuckoo Hash** para acelerar las búsquedas, inserciones y eliminaciones.

---

## 💡 ¿Qué tan rápido es?

El sistema puede:

- Buscar un ciudadano por su DNI en milisegundos.
- Redimensionar automáticamente su estructura interna cuando sea necesario.
- Incorpora un sistema de caché en memoria para mejorar aún más el rendimiento.
---

## 🤖 ¿Qué estructura usa?

- ✅ **Cuckoo Hashing**: para manejar colisiones y mantener alta velocidad.
- ✅ **Archivos binarios**: para almacenamiento persistente y acceso rápido.
- ✅ **Cache en memoria (`unordered_map`)**: para consultas frecuentes inmediatas.

---

## 🛠️ ¿Cómo ejecutar el proyecto?

Puedes abrir y compilar este proyecto fácilmente desde un entorno gráfico como:

- ✅ **Zinjai** (liviano y fácil de usar)
- ✅ **Code::Blocks** (más completo y versátil)

### Pasos recomendados:

1. **Abre los archivos `data_generator.cpp` y `reniec_system.cpp` en tu IDE**.
2. **Compila y ejecuta primero `data_generator.cpp`** para generar el archivo `registros.bin` con ciudadanos de prueba.
   
   ![image](https://github.com/user-attachments/assets/a0bfc765-be31-4572-8a57-c75578580cf0)

3. Luego, **ejecuta `reniec_system.cpp`** para interactuar con el sistema: podrás buscar personas, agregarlas o eliminarlas por su DNI.
5. El sistema mostrará un menú amigable donde puedes realizar todas las operaciones fácilmente.

---

## 🧪 Menú interactivo del sistema

Cuando ejecutes `reniec_system.cpp`, verás algo como esto:





