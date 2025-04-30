# 🆔 Proyecto RENIEC - Registro Nacional de Ciudadanos (C++)

Este proyecto simula una versión básica del sistema RENIEC del Perú, usando estructuras eficientes como **Cuckoo Hashing** para almacenar millones de ciudadanos por su DNI.

📌 **Objetivo principal**:  
Almacenar y gestionar hasta **33 millones de personas** con rapidez y eficiencia.

---

## 🚀 ¿Qué hace este proyecto?

- Genera millones de ciudadanos falsos con datos como nombres, apellidos, dirección, correo, teléfono, etc.
- Almacena los datos en un archivo binario (`registros.bin`) para optimizar el uso de memoria.
- Usa una estructura **Cuckoo Hash** para acelerar las búsquedas, inserciones y eliminaciones (¡en milisegundos!).
- Incorpora un sistema de caché en memoria para mejorar aún más el rendimiento.

---
