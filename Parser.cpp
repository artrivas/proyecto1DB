//
// Created by artrivas on 9/26/23.
//
#include "Parser.h"
// match and consume next token
bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}
bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}
bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp =current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Parse error, unrecognised character: " << current->lexema << endl;
            exit(0);
        }
        return true;
    }
    return false;
}
bool Parser::isAtEnd() {
    return (current->type == Token::END);
}
Parser::Parser() {
    previous = current = NULL;
    atributos["playerid"] = 1;
    atributos["fname"] = 2;
    atributos["lname"] = 3;
    atributos["position"] = 4;
    atributos["height"] = 5;
    atributos["weight"] = 6;
    atributos["birthday"] = 7;
    atributos["country"] = 8;
    atributos["school"] = 9;
    atributos["draft_year"] = 10;
    atributos["draft_round"] = 11;
    atributos["draft_number"] = 12;
}
void Parser::changeScanner(Scanner * sc){
    this->scanner = sc;
}


void Parser::icreate(){
    vector<int> order = {2,13,1,3,6,7,13,8,9}; //Orden de la llamada
    current = scanner->nextToken();
    int i = 0;
    string tablename;
    string rutacsv;
    string tipoindex;
    string atributo;
    while(current->type != Token::END){
        if(current->type == order[i]){
            if(order[i] == 1 && current->lexema != "from"){
                cout << "Ingrese una expresion valida\n";
            }else if(order[i] == 13 && i == 1){
                tablename = current->lexema;
            }else if(order[i] == 6){
                rutacsv = current->lexema;
            }else if(order[i] == 8){
                tipoindex = current->lexema;
            }else if(order[i] == 9){
                atributo = current->lexema;
                if(!atributos[atributo]){
                    cout << "Ingrese una expresion valida\n";
                    return;
                }
            }
            cout << current->type << ' ' << current->lexema << '\n';
        }else{
            cout << "Ingrese una expresion valida\n";
            return;
        }
        current = scanner->nextToken();
        i++;
    }
    cout <<tablename << ' ' << rutacsv << ' '<<tipoindex << ' ' << atributo <<'\n';
    tablas[tablename] = tipoindex;
    get_attribute[tablename] = atributo;
    if(tipoindex == "avl"){
        bool pk = atributo == "playerid";
        string indexfile = tablename;
        indexfile+='_';
        indexfile+=atributo;
        indexfile+=".bin";
        string heapfile = "heap_";
        heapfile += tablename;
        heapfile+='_';
        heapfile+=atributo;
        heapfile+=".bin";
        if(atributo == "fname" || atributo == "lname" || atributo == "position" || atributo == "height" || atributo == "birthday" || atributo == "country" || atributo == "school"){
            auto compare_function = [](char a[],char b[]) -> int{
                return strcmp(a,b);
            };
            AVLFile<char[20]> * index = new AVLFile<char[20]>(indexfile,heapfile,compare_function,pk,atributos[atributo]-1);
            vector<Record> ans = CsvToBin(rutacsv);
            for(auto lrecord: ans){
                index->insert(lrecord);
            }
            avl20[atributo] = index;
        }else if(atributo == "height" || atributo == "birthday" || atributo == "country"){
            auto compare_function = [](char a[],char b[]) -> int{
                return strcmp(a,b);
            };
            AVLFile<char[15]> * index = new AVLFile<char[15]>(indexfile,heapfile,compare_function,pk,atributos[atributo]-1);
            vector<Record> ans = CsvToBin(rutacsv);
            for(auto lrecord: ans){
                index->insert(lrecord);
            }
            avl15[atributo] = index;
        }else if(atributo == "playerid"){
            auto compare_function = [](long  a,long b) -> int{
                if(a > b) return 1;
                else if(a < b) return -1;
                else return 0;
            };
            AVLFile<long> * index = new AVLFile<long>(indexfile,heapfile,compare_function,pk,atributos[atributo]-1);
            vector<Record> ans = CsvToBin(rutacsv);
            for(auto lrecord: ans){
                index->insert(lrecord);
            }
            avl8[atributo] = index;
        }else if(atributo == "weight" || atributo == "draft_year" || atributo == "draft_round" || atributo == "draft_number"){
            auto compare_function = [](int  a,int b) -> int{
                if(a > b) return 1;
                else if(a < b) return -1;
                else return 0;
            };
            AVLFile<int> * index = new AVLFile<int>(indexfile,heapfile,compare_function,pk,atributos[atributo]-1);
            vector<Record> ans = CsvToBin(rutacsv);
            for(auto lrecord: ans){
                index->insert(lrecord);
            }
            avl4[atributo] = index;
        }
    }else if(tipoindex == "hash"){
        
        ;
    }else if(tipoindex == "sequential"){
        ;
    }else{
        cout << "Ingrese una expresion valida\n";
    }
}


// Función para eliminar espacios en blanco al principio y al final de una cadena
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

Record create_record(string texto) {
    Record nuevoRecord;
    // Divide el texto en partes usando comas como delimitador
    size_t pos = 0;
    vector<string> partes;
    while ((pos = texto.find(',')) != string::npos) {
        string token = texto.substr(0, pos);
        // Elimina espacios en blanco al principio y al final
        token = trim(token);

        // Elimina las comillas simples solo si rodean una cadena
        if (!token.empty() && token.front() == '\'' && token.back() == '\'') {
            token = token.substr(1, token.length() - 2);
        }

        partes.push_back(token);
        texto.erase(0, pos + 1);
    }
    // Elimina espacios en blanco al principio y al final de la última parte
    texto = trim(texto);
    // Elimina las comillas simples solo si rodean una cadena en la última parte
    if (!texto.empty() && texto.front() == '\'' && texto.back() == '\'') {
        texto = texto.substr(1, texto.length() - 2);
    }
    partes.push_back(texto);

    // Asigna los valores a los campos de la estructura Record
    if (partes.size() == 12) {
        nuevoRecord.playerid = stol(partes[0]);

        // Verifica que el campo fname no exceda los 20 caracteres
        if (partes[1].size() <= sizeof(nuevoRecord.fname)) {
            strncpy(nuevoRecord.fname, partes[1].c_str(), sizeof(nuevoRecord.fname));
        } else {
            cerr << "Error: El campo fname excede los " << sizeof(nuevoRecord.fname) << " caracteres permitidos." << endl;
        }

        // Repite el proceso para los otros campos...

        // Verifica que el campo lname no exceda los 20 caracteres
        if (partes[2].size() <= sizeof(nuevoRecord.lname)) {
            strncpy(nuevoRecord.lname, partes[2].c_str(), sizeof(nuevoRecord.lname));
        } else {
            cerr << "Error: El campo lname excede los " << sizeof(nuevoRecord.lname) << " caracteres permitidos." << endl;
        }

        // Verifica que el campo position no exceda los 20 caracteres
        if (partes[3].size() <= sizeof(nuevoRecord.position)) {
            strncpy(nuevoRecord.position, partes[3].c_str(), sizeof(nuevoRecord.position));
        } else {
            cerr << "Error: El campo position excede los " << sizeof(nuevoRecord.position) << " caracteres permitidos." << endl;
        }

        // Verifica que el campo height no exceda los 5 caracteres
        if (partes[4].size() <= sizeof(nuevoRecord.height)) {
            strncpy(nuevoRecord.height, partes[4].c_str(), sizeof(nuevoRecord.height));
        } else {
            cerr << "Error: El campo height excede los " << sizeof(nuevoRecord.height) << " caracteres permitidos." << endl;
        }

        nuevoRecord.weight = stoi(partes[5]);

        // Verifica que el campo birthday no exceda los 10 caracteres
        if (partes[6].size() <= sizeof(nuevoRecord.birthday)) {
            strncpy(nuevoRecord.birthday, partes[6].c_str(), sizeof(nuevoRecord.birthday));
        } else {
            cerr << "Error: El campo birthday excede los " << sizeof(nuevoRecord.birthday) << " caracteres permitidos." << endl;
        }

        // Verifica que el campo country no exceda los 15 caracteres
        if (partes[7].size() <= sizeof(nuevoRecord.country)) {
            strncpy(nuevoRecord.country, partes[7].c_str(), sizeof(nuevoRecord.country));
        } else {
            cerr << "Error: El campo country excede los " << sizeof(nuevoRecord.country) << " caracteres permitidos." << endl;
        }

        // Verifica que el campo school no exceda los 25 caracteres
        if (partes[8].size() <= sizeof(nuevoRecord.school)) {
            strncpy(nuevoRecord.school, partes[8].c_str(), sizeof(nuevoRecord.school));
        } else {
            cerr << "Error: El campo school excede los " << sizeof(nuevoRecord.school) << " caracteres permitidos." << endl;
        }

        nuevoRecord.draft_year = stoi(partes[9]);
        nuevoRecord.draft_round = stoi(partes[10]);
        nuevoRecord.draft_number = stoi(partes[11]);
    } else {
        cerr << "Error: El texto no contiene los 12 valores necesarios." << endl;
    }
    nuevoRecord.print_vals();
    return nuevoRecord;
}
void Parser::iinsert(){
    vector<int> order = {1,13,12}; //Orden de la llamada
    current = scanner->nextToken();
    int i = 0;
    Record a;
    string tablename;
    while(current->type != Token::END){
        if(order[i] == current->type){
            if(order[i] == 13 && tablas[current->lexema] == ""){
                cout << "Ingrese una expresion valida\n";
                return;
            }else if(order[i] == 12){
                a = create_record(current->lexema);
            }else if(order[i] == 13){
                tablename = current->lexema;
            }
            cout << current->type << ' ' << current->lexema << '\n';
        }
        current = scanner->nextToken();
        i++;
    }
    cout << tablename << '\n';
    a.print_vals();
    string index = tablas[tablename];
    string atributo = get_attribute[tablename];
    if(index == "avl"){
        if(avl20.find(atributo) != avl20.end()){
            avl20[atributo]->insert(a);
        }else if(avl15.find(atributo) != avl15.end()){
            avl15[atributo]->insert(a);
        }else if(avl25.find(atributo) != avl25.end()){
            avl25[atributo]->insert(a);
        }else if(avl4.find(atributo) != avl4.end()){
            avl4[atributo]->insert(a);
        }else if(avl8.find(atributo) != avl8.end()){
            avl8[atributo]->insert(a);
        } else{
            cout << "Programming error\n";
        }
    }else if(index == "hash"){
        ;
    }else if(index == "sequential"){
        ;
    }else{
        cout << "Ingrese una cadena valida\n";
    }

}
void Parser::idelete(){
    vector<int> order = {1,13,1,13,10,15}; //Orden de la llamada
    current = scanner->nextToken();
    int i = 0;
    string tablename;
    string atributo;
    string key;
    while(current->type != Token::END){
        if(order[i] == current->type){
            if(i && order[i] == 1 && current->lexema != "where"){
                cout << "Ingrese una expresion valida1\n";
                return;
            }else if(!i && order[i] == 1 && current->lexema != "from"){
                cout << "Ingrese una expresion valida2\n";
                return;
            }else if(order[i] == 13 && i == 1 && tablas[current->lexema] == ""){
                cout << "Ingrese una expresion valida3\n";
                return;
            }else if(order[i] == 13 && i == 1){
                tablename = current->lexema;
            }else if(order[i] == 13 && atributos[current->lexema]){
                atributo = current->lexema;
            }else if(order[i] == 13 && !atributos[current->lexema]){
                cout << "Ingrese una expresion valida4\n";
                return;
            }else if(order[i] == 15){
                key = current->lexema;
            }
            cout << current->type << ' ' << current->lexema << '\n';
        }
        current = scanner->nextToken();
        i++;
    }
    cout << tablename << ' ' << atributo << ' ' << key << '\n';
    string index = tablas[tablename];
    if(index == "avl"){
        if(avl20.find(atributo) != avl20.end()){
            char key1[20];
            strcpy(key1,key.c_str());
            avl20[atributo]->remove(key1);
        }else if(avl15.find(atributo) != avl15.end()){
            char key1[15];
            strcpy(key1,key.c_str());
            avl15[atributo]->remove(key1);
        }else if(avl25.find(atributo) != avl25.end()){
            char key1[25];
            strcpy(key1,key.c_str());
            avl25[atributo]->remove(key1);
        }else if(avl4.find(atributo) != avl4.end()){
            int key1 = stoi(key);
            avl4[atributo]->remove(key1);
        }else if(avl8.find(atributo) != avl8.end()){
            long key1 = stoi(key);
            avl8[atributo]->remove(key1);
        } else{
            cout << "Programming error\n";
        }
    }else if(index == "sequential"){
        ;
    }else if(index == "hash"){
        ;
    }
}
void Parser::iselect() {
    vector<int> order = {9, 1, 13,1,13}; //Orden de la llamada
    //9 1 13 1 13 10 15
    //9 1 13 1 13 11 15 14 15
    current = scanner->nextToken();
    int i = 0;
    string atributo;
    string table;
    while (i < 5) {
        if(order[i] == current->type){
            if(i == 3 && order[i] == 1 && current->lexema != "where"){
                cout << "Ingrese una expresion valida1\n";
                return;
            }else if(i == 1 && order[i] == 1 && current->lexema != "from"){
                cout << "Ingrese una expresion valida2\n";
                return;
            }else if(i == 4 && order[i] == 13 && atributos[current->lexema]){
                atributo = current->lexema;
            }else if(i == 4 && order[i] == 13 && !atributos[current->lexema]) {
                cout << "Ingrese una expresion valida4\n";
                return;
            }else if(i == 2 && order[i] == 13 && tablas[current->lexema]!= ""){
                table = current->lexema;
            }else if(i == 2 && order[i] == 13 && tablas[current->lexema] == "") {
                cout << current->lexema << ' ' << tablas[current->lexema] << '\n';
                cout << "Ingrese una expresion valida5\n";
                return;
            }
            cout << current->type << ' ' << current->lexema << '\n';
        }
        current = scanner->nextToken();
        i++;
    }
    if(current->type == Token::EQUAL){
        current = scanner->nextToken();
        if(current->type == Token::COMPARE){
            string key;
            string old_key;
            old_key = current->lexema;
            if(old_key[0] == '\'' && old_key[old_key.size()-1] == '\''){
                for(int itr = 1; itr< (int)old_key.size()-1;itr++){
                    key+=old_key[itr];
                }
            }

            cout <<table << ' ' <<atributo << ' ' << key << '\n';
            string index = tablas[table];
            if(index == "avl"){
                vector<Record> ans;
                if(avl20.find(atributo) != avl20.end()){
                    char key1[20];
                    strcpy(key1,key.c_str());
                    ans = avl20[atributo]->find(key1);
                }else if(avl15.find(atributo) != avl15.end()){
                    char key1[15];
                    strcpy(key1,key.c_str());
                    ans = avl15[atributo]->find(key1);
                }else if(avl25.find(atributo) != avl25.end()){
                    char key1[25];
                    strcpy(key1,key.c_str());
                    ans = avl25[atributo]->find(key1);
                }else if(avl4.find(atributo) != avl4.end()){
                    int key1 = stoi(key);
                    ans = avl4[atributo]->find(key1);
                }else if(avl8.find(atributo) != avl8.end()){
                    long key1 = stoi(key);
                    ans = avl8[atributo]->find(key1);
                } else{
                    cout << "Programming error\n";
                }
                for(auto rec: ans){
                    rec.print_vals();
                    cout << "\n------------------------------\n";
                }
            }else if(index == "sequential"){
                ;
            }else if(index == "hash"){
                ;
            }
        }else{
            cout << "Ingrese una expresion valida\n";
            return;
        }
    }else if(current->type == Token::BETWEEN) {
        vector<int> suborder1 = {15,14,15};
        current = scanner->nextToken();
        int j = 0;
        string key1;
        string key2;
        while(current->type != Token::END) {
            if(current->type == suborder1[j] && j == 0){
                key1 = current->lexema;
            }else if(current->type == suborder1[j] && j == 2){
                key2 = current->lexema;
            }
            current = scanner->nextToken();
            j++;
        }
        string index = tablas[table];
        string old_key1,old_key2;
        old_key1 = key1;
        old_key2 = key2;
        key1 = "";
        key2 = "";
        if(old_key1[0] == '\'' && old_key1[old_key1.size()-1] == '\''){
            for(int itr = 1; itr< (int)old_key1.size()-1;itr++){
                key1+=old_key1[itr];
            }
        }
        if(old_key2[0] == '\'' && old_key2[old_key1.size()-1] == '\''){
            for(int itr = 1; itr< (int)old_key2.size()-1;itr++){
                key2+=old_key2[itr];
            }
        }
        cout <<table <<  ' ' << atributo << ' ' << key1 << ' ' << key2 ;
        if(index == "avl"){
            vector<Record> ans;
            if(avl20.find(atributo) != avl20.end()){
                char keya[20];
                strcpy(keya,key1.c_str());
                char keyb[20];
                strcpy(keyb,key2.c_str());
                cout << keya << ' ' << keyb << '\n';
                ans = avl20[atributo]->rangeSearch(keya,keyb);
            }else if(avl15.find(atributo) != avl15.end()){
                char keya[15];
                char keyb[15];
                strcpy(keya,key1.c_str());
                strcpy(keyb,key2.c_str());
                ans = avl15[atributo]->rangeSearch(keya,keyb);
            }else if(avl25.find(atributo) != avl25.end()){
                char keya[25];
                char keyb[25];
                strcpy(keya,key1.c_str());
                strcpy(keyb,key2.c_str());
                ans = avl25[atributo]->rangeSearch(keya,keyb);
            }else if(avl4.find(atributo) != avl4.end()){
                int keya = stoi(key1);
                int keyb = stoi(key2);
                ans = avl4[atributo]->rangeSearch(keya,keyb);
            }else if(avl8.find(atributo) != avl8.end()){
                long keya = stoi(key1);
                long keyb = stoi(key2);
                ans = avl8[atributo]->rangeSearch(keya,keyb);
            } else{
                cout << "Programming error\n";
            }
            for(auto rec: ans){
                rec.print_vals();
                cout <<"\n--------------------------------\n";
            }
        }else if(index == "sequential"){
            ;
        }else if(index == "hash"){
            ;
        }
    }else {
        cout << "Ingrese una expresion valida\n";
        return;
    }
}

void Parser::parse() {
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }else if (current->type == Token::END)
        cout << "Esperaba fin-de-input"<< endl;
    else if(current->type == Token::INSTRUCTION){
        if(current->lexema == "create"){
            icreate();
        }else if(current->lexema == "select"){
            iselect();
        }else if(current->lexema == "insert"){
            iinsert();
        }else if(current->lexema == "delete"){
            idelete();
        }
    }else{
        cout << "Ingrese una instruction valida\n";
    }
    if (current) delete current;
}
