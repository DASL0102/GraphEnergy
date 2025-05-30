# AYDA-Project-Red-Electrica



## Instalación

### Requisitos previos

- **Git** (versión 2.34.1+ recomendada)
- **CMake** (versión 3.22.1)
- **Compilador C++** compatible con C++17:
  - g++ 11.4.0
  - Clang 14.0.0-1ubuntu1.1
- **SFML** (versión 2.5)
- **Graphviz** (opcional, para visualización de grafos)

## Instalación en Linux (Ubuntu/Debian)

### Instalar dependencias:

```bash
sudo apt update
sudo apt install -y git cmake g++ libsfml-dev graphviz
sudo apt-get install nlohmann-json3-dev
```

### Clonar el repositorio:
```bash
git clone https://gitlab.com/daniel_exercise/ayda-proyecto-red-electrica.git

cd ayda-proyecto-red-electrica
```



### Compilar AYDA-Project-Red-Electrica:

```bash
make
```

Se generara un ejecutable

```bash
./RedElectrica 
```


## Descripción del Proyecto

#### Este proyecto simula una red eléctrica donde:

🏭 Estaciones generadoras suministran energía a ⚡ subestaciones.

🏘️ Subestaciones distribuyen energía a viviendas y edificios.

📏 Se optimiza la red para reducir costos de cableado y mejorar eficiencia mediante algoritmos de grafos.



# Manual de Instrucciones - Simulador de Red Eléctrica

## 🏠 Botones de Inserción de Elementos

### 🏡 Casa

1. Haz clic izquierdo en el botón "Casa"
2. Selecciona una ubicación en el mapa con clic izquierdo
3. Se insertará una casa en la posición seleccionada



### ⚡ SubEstacion

1. Haz clic izquierdo en el botón "Subestación"
2. Selecciona una ubicación en el mapa con clic izquierdo
3. Se insertará una subestación en la posición seleccionada


### 🏭 Estación

1. Haz clic izquierdo en el botón "Estación"
2. Selecciona una ubicación en el mapa con clic izquierdo
3. Se insertará una estación en la posición seleccionada


##  🔌 Botones de Conexión

1. Posiciona el cursor sobre un elemento (casa/subestación/estación)
2. Haz clic derecho para seleccionar el origen
3. Mueve el cursor a otro elemento destino
4. Haz clic derecho para crear la conexión

 📌 Reglas de conexión:
   - Estación → Subestación
   - Subestación → Casa
   - Casa → Casa


## 📏 C. Distancia (Conexión por Distancia)
Requisitos previos:
- Tener estaciones y subestaciones conectadas
- Tener casas sin conexión

Acción:
- Haz clic izquierdo para generar conexiones automáticas
- Optimiza según distancia mínima

## ⚖️ C. Carga (Conexión por Carga)

Requisitos previos:
- Tener estaciones y subestaciones conectadas
- Tener casas sin conexión

Acción:
- Haz clic izquierdo para generar conexiones automáticas
- Optimiza según capacidad de carga de las subestaciones


## Botones de Gestión

### 💾 Guardar
- Crea un punto de restauración del estado actual
- Almacena toda la configuración de la red

### ↩️ Volver
- Restaura el último estado guardado
- Requiere crear nuevo "Guardar" para persistir cambios

## Datos de Pruebas

#### Si necesitas datos de prueba puedes cargar los archivos test1, test2, test3 y test4, que viene por defecto con el programa:




## 🖱️ Resumen de Controles

| Acción           | Tipo de Clic | Procedimiento                              |
|------------------|--------------|--------------------------------------------|
| **Insertar**     | Clic Izq.    | 1. Clic en botón (Casa/Subestación/Estación)<br>2. Clic en mapa |
| **Conexión Manual** | Clic Der.   | 1. Clic derecho en origen<br>2. Clic derecho en destino |
| **Conexión Auto**  | Clic Izq.    | 1. Clic en botón (Óptima/Sugerida)<br>2. Sistema calcula conexiones |
| **Fix**         | Clic Izq.     | 1. Clic en botón Fix<br>2. Sistema repara conexiones |
| **Guardar**     | Clic Izq.     | Clic único para crear punto de guardado     |
| **Volver**      | Clic Izq.     | Clic único para restaurar último guardado  |


## 🎥 Video Tutorial

[![Cómo usar el programa - Tutorial completo](https://img.youtube.com/vi/m8LvUYv171g/0.jpg)](https://www.youtube.com/watch?v=m8LvUYv171g)

**Haz clic en la imagen para ver el tutorial en YouTube.**  
En este video aprenderás a usar todas las funciones del programa.