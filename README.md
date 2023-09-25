# BASE DE DATOS 2 - PROYECTO 1
![image](https://github.com/artrivas/proyecto1DB/assets/87825436/b57c62e9-cc40-46d5-b31e-83a93daf1089)

## INTEGRANTES:

* Dimael Rivas [@artrivas](https://github.com/artrivas)
* Cahuana Condori, Kelvin AndreI [@andrewkc](https://github.com/andrewkc) 
* Chu
* Johan Callinapa
* Stuart Arteaga Montes [@SDAM26](https://github.com/SDAM26)

## 1. OBJETIVO
El objetivo de nuestro proyecto es implementar y comparar tres estructuras de organización de archivos para una recuperación y manipulación eficiente de datos. Las estructuras elegidas son:
* AVL file
* Sequential File
* Hash
Nuestras estructuras tendran las siguientes funciones :
* Buscar: dada una clave, devolveremos los registros respectivos.
* Búsqueda de rango: dado un rango de claves, devuelve todos los registros cuyas claves estén dentro del rango.
* Agregar: agrega un nuevo registro al archivo.
* Eliminar: eliminar un registro del archivo.
## 2. DATOS 
Datos sobre todos 548 jugadores de la NBA activos durante la temporada 2022-2023 obtenidos en [Kaggle](https://www.kaggle.com/datasets/szymonjwiak/nba-active-players-data-images)  que contiene los siguientes atributos:
| Atributo | Significado |
|-----------|-----------|
| playerid    | Id del jugador   |
|   fname  | Nombres    |
| lname    | Apellidos    |
| position    | Posicion de juego   |
| height     | Altura    |
| weight    | Peso    |
| birthday   | Cumpleaños    |
| Country    | Pais   |
| School    | Colegio    |
| Draft year    | Año de fichaje    |
## 3. Desempeño de las técnicas de indexación 
### AVL FILE:
AVL ofrece una mayor eficiencia en la búsqueda y manipulación de registros individuales, ya que es completamente dinámico. Esto lo hace más adecuado para aplicaciones donde se necesitan búsquedas de registros frecuentes y eficientes. Para aplicaciones que requieren búsquedas por rango de valores en la clave de índice, es particularmente útil.Aún así, reconocemos que ISAM puede ser más eficiente en términos de uso de memoria ya que no necesita almacenar toda la estructura del árbol como lo hace el archivo AVL.

![image](![image](https://github.com/artrivas/proyecto1DB/assets/87825436/44403560-f8f6-42e8-bfe4-b3bdda6d0240)

### Sequential File:
Un archivo secuencial es un archivo de datos donde los registros o entradas se almacenan de manera consecutiva, uno detrás del otro, en un solo flujo o secuencia de datos. Cada registro en el archivo tiene una longitud fija o variable. Lamentablemente debido a su naturaleza secuencial, el acceso a registros específicos dentro de un archivo secuencial puede ser lento, especialmente si se necesita acceder a un registro cercano al final del archivo.

![image](https://github.com/artrivas/proyecto1DB/assets/87825436/c27ab987-089a-4418-b1d4-0ff72e27d851)
#### Insercion
Si el archivo auxiliar (auxfile) llega a tener una cantidad k de registros, es esencial llevar a cabo un rebuild(). De no ser así, se inicia verificando el último SequentialBlock del datafile. Si la llave del registro actual es menor que la del registro a insertar, este último se añade al final del datafile, y se actualizan los punteros correspondientes. Si no es el caso, se procede a ubicar el espacio adecuado entre el datafile y el auxfile. El nuevo registro se anexa al final del auxfile y, si ya existe un registro con la misma llave, se retorna False. En cuanto a su complejidad, la inserción puede demandar un tiempo promedio de O(n/2).
#### Búsqueda 

El proceso de búsqueda puede variar. Para una búsqueda específica, se podría dividir el Archivo Secuencial en segmentos para identificar la llave, resultando en una complejidad de O(logn) + O(k), donde k es la longitud del auxfile. En búsquedas por rango, se localizan ambos extremos, inferior y superior, para obtener todos los registros entre esos valores. La técnica emplea tanto la búsqueda binaria en el datafile como la búsqueda lineal en el auxfile.


#### Eliminación

Para eliminar un registro, se busca la llave específica navegando tanto el datafile como el auxfile. Una vez identificada, se ajustan los punteros para reflejar la eliminación y se actualizan los registros afectados.


### Extendible Hashing 
Extensible Hashing es una técnica de organización de datos que se utiliza para indexar y buscar información en una estructura de datos jerárquica. Se basa en una función de hash que asigna datos a buckets, y cada bucket contiene un conjunto de registros relacionados. La característica distintiva del Extensible Hashing es que permite que el número de buckets aumente o disminuya dinámicamente a medida que se agregan o eliminan registros, lo que lo hace eficiente y escalable.

![image](https://github.com/artrivas/proyecto1DB/assets/87825436/1cde3731-62ba-4a11-a005-b63601966188)

### 3.1.1. Inserción
### 3.1.2. Búsqueda
### 3.1.3. Eliminación
## 4. Interfaz gráfica
## 5. Video Demostrativo
## 6. Conclusiones
## 7. Referencias
