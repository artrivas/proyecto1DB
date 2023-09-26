# BASE DE DATOS 2 - PROYECTO 1
![image](https://github.com/artrivas/proyecto1DB/assets/87825436/b57c62e9-cc40-46d5-b31e-83a93daf1089)

## INTEGRANTES:

* Dimael Rivas [@artrivas](https://github.com/artrivas)
* Cahuana Condori, Kelvin AndreI [@andrewkc](https://github.com/andrewkc) 
* Johan Callinapa [@fnction](https://github.com/johancalli)
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

![image](https://github.com/artrivas/proyecto1DB/assets/87825436/44403560-f8f6-42e8-bfe4-b3bdda6d0240)

#### Insercion
Antes de insertar el nodo en el avl, primero insertamos el récord en el HeapFile, de esta manera encontramos la posición física del registro, la cual servirá para la creación del nodo del avl. Siguiendo con esto, tenemos 3 casos:
  1. El archivo está vacío: Insertamos el nodo
  2. El root_node se encuentra eliminado: Reemplazamos el valor por el nodo a insertar
  3. Recorremos por medio de una recursión comparando un nodo comparativo y la llave
Enfocándonos en el 3er caso, tenemos que si el nodo no ha sido insertado anteriormente, se inserta normalmente. En caso de que no lo sea, se realiza un push_front en una lista enlazada anexada al nodo repetido (en el caso donde el atributo no sea primary_key). Luego por un proceso recursivo actualizamos la altura y balanceamos los nodos.

```cpp
    bool insert(Record record) {
        setup_files();
        ofstream file(this->heapfilename,ios::binary | ios::app);
        file.write((char *) &record, sizeof(Record));
        unsigned long nodepos = file.tellp()/sizeof(Record);
        file.close();
        return add(nodepos);
    }
    void insert_node_by_pos(AVLNode<KeyType> & node, long position){
        setup_files();
        writefile.seekp((position-1)*sizeof(AVLNode<KeyType>));
        writefile.write((char*) &node,sizeof(node));
    }
```

#### Busqueda
Por medio de una recursión encontramos el nodo con la llave deseada, y por medio de un bucle obtenemos todas los nodos con la misma llave por medio del atributo list pointer, luego los resultados se reemplazan por sus posiciones físicas en el heapfile (por medio del atributo file_position).
```cpp

    vector<AVLNode<KeyType>> find(long pos_node, KeyType & key){
        if(pos_node<0) return vector<AVLNode<KeyType>>{};
        readfile.seekg((pos_node-1)*sizeof(AVLNode<KeyType>));
        AVLNode<KeyType> cmp_node;
        readfile.read((char*) &cmp_node,sizeof(AVLNode<KeyType>));
        int result = this->compare(key,cmp_node.key);
        if(result > 0){
            return find(cmp_node.right,key);
        }else if(result <0){
            return find(cmp_node.left,key);
        }else{
            //Tendria que hacer una busqueda de arriba y abajo para obtener todos los records que cumplen
            vector<AVLNode<KeyType>> ans;
            ans.push_back(cmp_node);
            AVLNode<KeyType> cnt = get_node(cmp_node.list_pointer);
            while(cnt.file_position != -1){
                ans.push_back(cnt);
                cnt = get_node(cnt.list_pointer);
            }
            return ans;
        }
    }

```

#### Remove
Utilizamos un lacy method para marcar nodos eliminados, cambiando el atributo file position a -1, y es debido a eso que por medio de comparaciones recursivas eliminamos el nodo en 3 casos:
  1. El nodo es hoja: Se retorna un -1 al puntero del padre
  2. El nodo tiene un hijo: Se retorna la posición del hijo al puntero del padre
  3. El nodo tiene 2 hijos: Se intercambian los valores entre el sucesor y el nodo a eliminar y se repite el proceso en el sub árbol derecho con la llave del sucesor.
Una vez ejecutado, se procede a actualizar las alturas y balancear los nodos. Note que no es necesario eliminar los nodos repetidos, ya que al eliminar al principal, las demás ocurrencias se pierden
```cpp
    bool remove(KeyType & key) {
        AVLNode<KeyType> root = get_node(this->pos_root);
        long ans = _remove(key,root,this->pos_root);
        if(ans == -2) return false;
        return true;
    }
long _remove(KeyType key,AVLNode<KeyType> cmp_node,long pos){
        if(cmp_node.file_position == -1) return -2;
        int result1 = this->compare(cmp_node.key,key);
        if(result1 > 0){
            AVLNode<KeyType> left_node = get_node(cmp_node.left);
            int result2 = this->compare(left_node.key,key);
            if(!result2){
                cmp_node.left = _remove(key,left_node,cmp_node.left);
                insert_node_by_pos(cmp_node,pos);
            }else{
                _remove(key,left_node,cmp_node.left);
            }
        }else if(result1 < 0){
            AVLNode<KeyType> right_node = get_node(cmp_node.right);
            int result3 = this->compare(right_node.key,key);
            if(!result3){
                cmp_node.right = _remove(key,right_node,cmp_node.right);
                insert_node_by_pos(cmp_node,pos);
            }else{
                _remove(key,right_node,cmp_node.right);
            }
        }else{
            if(cmp_node.left == -1 || cmp_node.right == -1){ //El nodo tiene 1 hijo o es nodo hoja
                cmp_node.file_position = -1;
                insert_node_by_pos(cmp_node,pos);
                return (cmp_node.left == -1 ? cmp_node.right : cmp_node.left); //Si cmp_node.right es -1, no tiene hijos |
            }else{ // Nodo con 2 hijos
                AVLNode<KeyType> right_subtree = get_node(cmp_node.right);
                long pos_succesor = find_succesor(right_subtree,cmp_node.right);
                AVLNode<KeyType> succesor = get_node(pos_succesor);
                memcpy((char*) & cmp_node.key, (char*) & succesor.key, sizeof(cmp_node.key));
                cmp_node.right = _remove(succesor.key,right_subtree,cmp_node.right);
                insert_node_by_pos(cmp_node,pos);
            }
        }
        //Actualizar alturas
        update_height(cmp_node,pos);

        long balance = get_balance(pos);

        if (balance > 1 && get_balance(cmp_node.left) >= 0) // Left Left
            right_rotation(cmp_node,pos);
        else if (balance > 1 && get_balance(cmp_node.left) < 0){ // Left Right
            AVLNode<KeyType> left_node = get_node(cmp_node.left);
            left_rotation(left_node,cmp_node.left);
            right_rotation(cmp_node,pos);
        }else if (balance < -1 && get_balance(cmp_node.right) <= 0) //Right Right
            left_rotation(cmp_node,pos);
        else if (balance < -1 && get_balance(cmp_node.right) > 0){// Right Left
            AVLNode<KeyType> right_node = get_node(cmp_node.right);
            right_rotation(right_node,cmp_node.right);
            left_rotation(cmp_node,pos);
        }

        return pos;
    }
```

#### Range Search
Se acota las búsquedas recursivas por medio de ambos limitadores, luego se procede a verificar si los nodos dentro del rango poseen repeticiones, para así extraerlos y reemplazarlos por sus valores físicos almacenados en el heapfile.
```cpp

void _rangeSearch(AVLNode<KeyType> cmp_node, KeyType & bkey, KeyType & ekey,vector<AVLNode<KeyType>> & ans){
        if(cmp_node.file_position == -1)
            return;
        int result1 = this->compare(bkey,cmp_node.key); //bkey <= cmp.key
        int result2 = this->compare(cmp_node.key,ekey); //cmp.key <= ekey
        if(result1<=0){
            AVLNode<KeyType> new_cmp_node;
            readfile.seekg((cmp_node.left-1)*sizeof(AVLNode<KeyType>));
            readfile.read((char*) &new_cmp_node,sizeof(AVLNode<KeyType>));
            _rangeSearch(new_cmp_node,bkey,ekey,ans);
        }
        if(result1 <= 0 && result2 <=0){
            //Iprimir;
            ans.push_back(cmp_node);
            AVLNode<KeyType> cnt = get_node(cmp_node.list_pointer);
            while(cnt.file_position != -1){
                ans.push_back(cnt);
                cnt = get_node(cnt.list_pointer);
            }
        }
        if(result2 <=0){
            AVLNode<KeyType> new_cmp_node;
            readfile.seekg((cmp_node.right-1)* sizeof(AVLNode<KeyType>));
            readfile.read((char*)&new_cmp_node,sizeof(AVLNode<KeyType>));
            _rangeSearch(new_cmp_node,bkey,ekey,ans);
        }
    }

```

### Sequential File:
Un archivo secuencial es un archivo de datos donde los registros o entradas se almacenan de manera consecutiva, uno detrás del otro, en un solo flujo o secuencia de datos. Cada registro en el archivo tiene una longitud fija o variable. Lamentablemente debido a su naturaleza secuencial, el acceso a registros específicos dentro de un archivo secuencial puede ser lento, especialmente si se necesita acceder a un registro cercano al final del archivo.
![image](https://github.com/artrivas/proyecto1DB/assets/87825436/c27ab987-089a-4418-b1d4-0ff72e27d851)

#### Insercion
Si el archivo auxiliar (auxfile) llega a tener una cantidad k de registros, es esencial llevar a cabo un rebuild(). De no ser así, se inicia verificando el último SequentialBlock del datafile. Si la llave del registro actual es menor que la del registro a insertar, este último se añade al final del datafile, y se actualizan los punteros correspondientes. Si no es el caso, se procede a ubicar el espacio adecuado entre el datafile y el auxfile. El nuevo registro se anexa al final del auxfile y, si ya existe un registro con la misma llave, se retorna False. En cuanto a su complejidad, la inserción puede demandar un tiempo promedio de O(n/2).

```c++

bool add(Record registro) {
        if (this->numAuxEntries == K) {
            rebuild();
        }

        fstream datafile(datafile_name, ios::in | ios::out | ios::binary);
        fstream auxfile(auxfile_name, ios::in | ios::out | ios::binary);

        if (addToDataIfGreaterThanLast(datafile, registro)) {
            datafile.close();
            auxfile.close();
            return true;
        }

        bool added = addToAux(datafile, auxfile, registro);

        datafile.close();
        auxfile.close();

        return added;
    }
bool addToDataIfGreaterThanLast(fstream& datafile, const Record& registro) {
        RecordBlock last;

        datafile.seekg(-sizeof(RecordBlock), ios::end);
        datafile.read((char*)&last, sizeof(RecordBlock));
        if (last.record.get_key() < registro.get_key()) {
            RecordBlock new_block{ last.next, last.next_file, registro};

            datafile.seekp(0, ios::end);
            datafile.write((char*)&new_block, sizeof(RecordBlock));

            last.next = (long) datafile.tellp() - sizeof(RecordBlock);
            last.next_file = DATA_FILE;

            datafile.seekp(-2 * sizeof(RecordBlock), ios::cur);
            datafile.write((char*)&last, sizeof(RecordBlock));

            return true;
        }

        return false;
    }

    bool addToAux(fstream& datafile, fstream& auxfile, const Record& registro) {
        long current_pos = 0;
        char current_file = DATA_FILE;
        RecordBlock current;

        datafile.seekg(0, ios::beg);
        datafile.read((char*)&current, sizeof(RecordBlock));

        RecordBlock next;
        while (current.next != -1) {
            fstream& currentStream = (current.next_file == DATA_FILE) ? datafile : auxfile;
            currentStream.seekg(current.next, ios::beg);
            currentStream.read((char*)&next, sizeof(RecordBlock));

            if (next.record.get_key() == registro.get_key()) {
                return false;
            } else if (next.record.get_key() > registro.get_key()) {
                break;
            } else {
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
        }

        RecordBlock block{ current.next, current.next_file, registro };

        auxfile.seekp(0, ios::end);
        long pos = auxfile.tellp();
        auxfile.write((char*)&block, sizeof(RecordBlock));

        current.next = pos;
        current.next_file = AUX_FILE;

        fstream& targetStream = (current_file == DATA_FILE) ? datafile : auxfile;
        targetStream.seekp(current_pos, ios::beg);
        targetStream.write((char*)&current, sizeof(RecordBlock));

        numAuxEntries++;
        return true;
    }
```



#### Búsqueda 

El proceso de búsqueda puede variar. Para una búsqueda específica, se podría dividir el Archivo Secuencial en segmentos para identificar la llave, resultando en una complejidad de O(logn) + O(k), donde k es la longitud del auxfile. En búsquedas por rango, se localizan ambos extremos, inferior y superior, para obtener todos los registros entre esos valores. La técnica emplea tanto la búsqueda binaria en el datafile como la búsqueda lineal en el auxfile.

```c++
template<typename T>
    vector<Record> search(T key) {
        vector<Record> result;

        if (binarySearchInDatafile(key, result)) {
            return result;
        }

        linearSearchInAuxfile(key, result);

        return result;
    }

template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key) {
        vector<Record> result;

        fstream datafile(datafile_name, ios::in | ios::binary);
        searchInFile(datafile, begin_key, end_key, result);
        datafile.close();

        fstream auxfile(auxfile_name, ios::in | ios::binary);
        searchAuxFile(auxfile, begin_key, end_key, result);
        auxfile.close();

        return result;
    }
```


#### Eliminación

Para eliminar un registro, se busca la llave específica navegando tanto el datafile como el auxfile. Una vez identificada, se ajustan los punteros para reflejar la eliminación y se actualizan los registros afectados.

```c++
template<typename T>
    bool remove(T key){
        fstream auxfile(auxfile_name, ios::in | ios::out | ios::binary);
        fstream datafile(datafile_name, ios::in | ios::out | ios::binary);
        long current_pos = 0;
        RecordBlock current, next;

        datafile.seekg(0, ios::beg);
        datafile.read((char*)&current, sizeof(RecordBlock));

        bool keyFound = false;
        char current_file = DATA_FILE;

        while (current.next != -1) {
            fstream &activeFile = (current.next_file == DATA_FILE) ? datafile : auxfile;
            activeFile.seekg(current.next, ios::beg);
            activeFile.read((char*)&next, sizeof(RecordBlock));
            if (next.record.get_key() == key) {
                keyFound = true;
                long temp_pos = next.next;
                next.next = -2;
                activeFile.seekg(current.next, ios::beg);
                activeFile.write((char*)&next, sizeof(RecordBlock));
                current.next = temp_pos;
                current.next_file = next.next_file;
                if (current_file == DATA_FILE) {
                    datafile.seekg(current_pos, ios::beg);
                    datafile.write((char*)&current, sizeof(RecordBlock));
                } else {
                    auxfile.seekg(current_pos, ios::beg);
                    auxfile.write((char*)&current, sizeof(RecordBlock));
                }
                break;
            } else if (next.record.get_key() > key) {
                break;
            }

            current_pos = current.next;
            current_file = current.next_file;
            current = next;
        }

        datafile.close();
        auxfile.close();

        if (keyFound) {
            return true;
        }
        return false;
    }


```


### Extendible Hashing 
Extensible Hashing es una técnica de organización de datos que se utiliza para indexar y buscar información en una estructura de datos jerárquica. Se basa en una función de hash que asigna datos a buckets, y cada bucket contiene un conjunto de registros relacionados. La característica distintiva del Extensible Hashing es que permite que el número de buckets aumente o disminuya dinámicamente a medida que se agregan o eliminan registros, lo que lo hace eficiente y escalable.

![image](https://github.com/artrivas/proyecto1DB/assets/87825436/1cde3731-62ba-4a11-a005-b63601966188)

#### Inserccion
```c++
template <typename TR, typename TK>
bool ExtHashFile<TR, TK>::add(TR record) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 
    
    std::fstream indexFileFL(indexfilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFileFL.is_open())
        throw std::runtime_error("Cannot open index_fl file"); 
    
    std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFileFL.is_open())
        throw std::runtime_error("Cannot open data_fl file"); 
    
    size_t num = hasher(record.get_key()) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    if (is_empty(indexFile)) {
        std::cout << "There are no records in the datafile\n";
        return false;
    }

    if (search(record.get_key()) != nullptr) {
        std::cout << "The record is already indexed, records with the same key are not allowed\n";
        return false;
    }

    IndexEntry node;
    int prev_pos_node{};
    int pos_node{}; 
    indexFile.read((char*) &count_index, sizeof(count_index));
    int depth{};

    find_node_pos(indexFile, binary, pos_node, prev_pos_node, depth);

    if (pos_node != -1) {
        indexFile.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);
        add_record(indexFile, dataFile, indexFileFL, dataFileFL, bucket, node, pos_node, record);
    
    } else {
        indexFile.seekg((prev_pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));
        
        if (node.is_leaf()) {
            Bucket<TR> bucket(metadata.block_factor);
            dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            bucket.read(dataFile);

            add_record(indexFile, dataFile, indexFileFL, dataFileFL, bucket, node, prev_pos_node, record);
            indexFile.close();
            dataFile.close();
            indexFileFL.close();
            dataFileFL.close();
            return true;
        }

        Bucket<TR> new_buck(metadata.block_factor);
        new_buck.metadata.local_depth = depth + 1;
        
        IndexEntry new_node_e;
        new_buck.add(record);

        int pos_add = 0;
        if (node.left == -1) {
            node.pos_bucket = -1;
            node.left = get_pos_add_node(indexFileFL);
            pos_add = node.left;
        } else {
            node.pos_bucket = -1;
            node.right = get_pos_add_node(indexFileFL); 
            pos_add = node.right;
        }
        count_index++;

        new_node_e.pos_bucket = get_pos_add_bucket(dataFileFL); 
        metadata.count++;

        // write bucket count and index count
        dataFile.seekp(0, std::ios::beg);
        dataFile.write((char*) &metadata.count, sizeof(int)); 
        indexFile.seekp(0, std::ios::beg);
        indexFile.write((char*) &count_index, sizeof(int)); 

        // write prev node updated
        indexFile.seekp(prev_pos_node * sizeof(IndexEntry) + sizeof(count_index), std::ios::beg);
        indexFile.write((char*) &node, sizeof(IndexEntry));

        // write new bucket
        dataFile.seekp(sizeof(metadata) + (new_node_e.pos_bucket * new_buck.size_of()), std::ios::beg);
        new_buck.write(dataFile);

        // write new node
        indexFile.seekp(sizeof(int) + (pos_add * sizeof(new_node_e)), std::ios::beg);
        indexFile.write((char*) &new_node_e, sizeof(IndexEntry));
    }

    indexFile.close();
    dataFile.close();
    indexFileFL.close();
    dataFileFL.close();
    return true;
}
```
#### BUSQUEDA

```c++
template <typename TR, typename TK>
TR* ExtHashFile<TR, TK>::find_record(TK key, int& pos_bucket, TR& record, int& pos_record, bool& found) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 

    size_t num = hasher(key) % int(std::pow(2, metadata.global_depth));
    std::string binary = numToBin(num, metadata.global_depth);
    std::reverse(binary.begin(), binary.end());

    if (is_empty(indexFile)) {
        std::cout << "There are no records in the datafile\n";
        return nullptr;
    }

    IndexEntry node;
    int prev_pos_node{};
    int pos_node{}; 
    indexFile.read((char*) &count_index, sizeof(count_index));
    int depth{};

    find_node_pos(indexFile, binary, pos_node, prev_pos_node, depth);

    if (pos_node == -1) {
        indexFile.seekg((prev_pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        if (node.is_leaf()) {
            Bucket<TR> bucket(metadata.block_factor);
            dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            bucket.read(dataFile);

            for (int i = 0; i < bucket.metadata.capacity; ++i) {
                if (bucket.next_del[i] == 0 && bucket.records[i].get_key() == key) {
                    TR* record_ptr = &bucket.records[i];
                    pos_bucket = node.pos_bucket;
                    record = bucket.records[i];
                    pos_record = i;
                    found = true;
                    return record_ptr; 
                }
            }
            // std::cout << "No record was found with that key" << std::endl;
            return nullptr;
       
        } else {
            // std::cout << "No record was found with that key" << std::endl;
            return nullptr;
            
        }
    } else {
        indexFile.seekg((pos_node * sizeof(IndexEntry)) + sizeof(int), std::ios::beg);
        indexFile.read((char*) &node, sizeof(IndexEntry));

        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((node.pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);

        for (int i = 0; i < bucket.metadata.capacity; ++i) {
            if (bucket.next_del[i] == 0 && bucket.records[i].get_key() == key) {
                TR* record_ptr = &bucket.records[i];
                pos_bucket = node.pos_bucket;
                record = bucket.records[i];
                pos_record = i;
                found = true;
                return record_ptr; 
            }
        }
        // std::cout << "No record was found with that key" << std::endl;
        return nullptr;
    }
    indexFile.close();
    dataFile.close();
    return nullptr;
}

```
#### ELIMINACIÓN 
```c++
template <typename TR, typename TK>
bool ExtHashFile<TR, TK>::remove(TK key) {
    std::fstream indexFile(indexfile, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFile.is_open())
        throw std::runtime_error("Cannot open indexfile"); 
    
    std::fstream dataFile(datafile, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFile.is_open())
        throw std::runtime_error("Cannot open datafile"); 
    
    std::fstream indexFileFL(indexfilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!indexFileFL.is_open())
        throw std::runtime_error("Cannot open index_fl file"); 
    
    std::fstream dataFileFL(datafilefl, std::ios::binary | std::ios::in | std::ios::out);
    if (!dataFileFL.is_open())
        throw std::runtime_error("Cannot open data_fl file"); 

    if (metadata.count == 0)
        return false;

    int pos_bucket{};
    static TR record;
    int pos_record{};
    bool found {false};
    auto result = find_record(key, pos_bucket, record, pos_record, found);

    std::vector<int> nodes_to_delete{};
    int desc_depth{};

    if (result != nullptr && found == true) {
        std::string bin;
        std::vector<int> path = get_path_traversal(indexFile, key, bin);
        Bucket<TR> bucket(metadata.block_factor);
        dataFile.seekg((pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
        bucket.read(dataFile);

        if (bucket.metadata.count == 1) {
            IndexEntry node;
            int i{};
            for (i = path.size() - 2; i >= 0; i--) {
                indexFile.seekg((path[i] * sizeof(node)) + sizeof(count_index), std::ios::beg);
                indexFile.read((char*) &node, sizeof(node));
                if (node.left != -1 && node.right != -1) {
                    if (bin[i] == '0') {
                        nodes_to_delete.push_back(node.left);
                        node.left = -1;
                    } else {
                        nodes_to_delete.push_back(node.right);
                        node.right = -1;
                    }
                    break;
                } else {
                    if (node.right == -1)
                        nodes_to_delete.push_back(node.left);
                    else
                        nodes_to_delete.push_back(node.right);
                    desc_depth++;
                }
            }
            
            if (bucket.metadata.local_depth == metadata.global_depth) {
                metadata.global_depth = metadata.global_depth - desc_depth;
            }
            metadata.count--;
            count_index = count_index - nodes_to_delete.size();

            // update father node
            indexFile.seekp((path[i] * sizeof(node)) + sizeof(count_index), std::ios::beg);
            indexFile.write((char*) &node, sizeof(node));
            
            // update metadata: global_depth, count
            dataFile.seekp(0, std::ios::beg);
            dataFile.write((char*) &metadata, sizeof(metadata));
            
            // update count_index 
            indexFile.seekp(0, std::ios::beg);
            indexFile.write((char*) &count_index, sizeof(count_index));

            // remove nodes
            remove_node(indexFileFL, nodes_to_delete);

            // remove bucket
            remove_bucket(dataFileFL, pos_bucket);

        } else {
            bucket.remove(pos_record);
            dataFile.seekp((pos_bucket * bucket.size_of()) + sizeof(metadata), std::ios::beg);
            dataFile.write((char*) &bucket.metadata, sizeof(bucket.metadata));
            for (int i = 0; i < bucket.metadata.capacity; ++i) {
                dataFile.write((char*) &bucket.next_del[i], sizeof(int));
            }
        }
        return true;
    }
    else {
        std::cout << "The record with that key does not exist or has already been deleted\n";
    }
    dataFile.close();
    indexFile.close();
    dataFileFL.close();
    indexFileFL.close();
    return false;
}
```


## Analisis de complejidades
### 3.1.1. Inserción
<div align="center">
<img src="graficos/grafico1.png" alt="GRAFICO 1" width="500" height="500">
<img src="graficos/grafico2.png" alt="GRAFICO 2" width="500" height="500">
</div>

### 3.1.2. Búsqueda
<div align="center">
<img src="graficos/grafico3.png" alt="GRAFICO 3" width="500" height="500">
<img src="graficos/grafico4.png" alt="GRAFICO 4" width="500" height="500">
</div>

## 4. Interfaz gráfica
<img src="graficos/in3.png" alt="GRAFICO 4" width="500" height="500">

## 5. Conclusiones
El uso de estructura de datos en archivos facilita el ingreso a los registros de manera mas eficiente y menos compleja que la forma comun.
Durante la realización de nuestro proyecto universitario, que involucró el uso de estructuras de datos como los árboles AVL, el hashing extensible y los archivos secuenciales para gestionar datos, hemos adquirido conocimientos valiosos y experiencia en varias áreas:
* Árboles AVL:
Aprendimos que los árboles AVL son estructuras de búsqueda equilibradas que son excelentes para mantener datos ordenados.
Comprendimos cómo las rotaciones y equilibrios mantienen la estructura en óptimas condiciones para búsquedas eficientes.
Experimentamos con operaciones de inserción, eliminación y búsqueda en árboles AVL.
* Hashing Extensible:
Descubrimos que el hashing extensible es eficiente para búsquedas rápidas en conjuntos de datos grandes.
Exploramos cómo el tamaño de las tablas de dispersión se expande y contrae dinámicamente según sea necesario.
Aprendimos a manejar colisiones y cómo dividir o fusionar tablas de dispersión.
* Archivos Secuenciales:
Adquirimos habilidades prácticas en la gestión de archivos secuenciales, lo que es esencial en la ciencia de datos y la gestión de bases de datos.
Aprendimos a organizar datos de manera eficiente en disco y a realizar operaciones de lectura y escritura en archivos secuenciales.
* Consideraciones de Rendimiento y Eficiencia:
Reconocimos la importancia de evaluar el rendimiento y la eficiencia de cada estructura de datos en diferentes situaciones.
Comparamos operaciones de inserción, búsqueda y eliminación en árboles AVL, hashing extensible , Sequential File.
## 7. Referencias
* Estructura de datos : Archivos ( files ). (s. f.). https://www.fceia.unr.edu.ar/estruc/2006/fileintr.htm
* IBM documentation. (s. f.). https://www.ibm.com/docs/es/iis/11.5?topic=sets-structure-data
* rincondelvago.com. (2017, 30 abril). Encuentra aquí información de Estructura de datos: Acceso en ficheros para tu escuela ¡Entra ya! | Rincón del Vago. https://html.rincondelvago.com/estructura-de-datos_acceso-en-ficheros_1.html

