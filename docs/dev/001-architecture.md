јрхитектура  
-----------

- [x] ¬есь код библиотеки расположен в пространстве имен `db`  
- [x] ¬с€ библиотека состоит:  
  - [x] »з нескольких глобальных функций:  
    - [ ] [availableDatabase(filename)][0]  
    - [ ] [existDatabase(filename)][1]  
    - [ ] [existTable(filename, table)][2]  
    - [ ] [existColumn(filename, table, column)][3]  
    - [ ] [dropDatabase(filename)][4]  
    - [ ] [dropTable(filename, table)][5]  
    - [ ] [dropColumn(filename, table, column)][6]  
  - [x] публичный класс db::connection.  
    - [ ] конструктор.  
    - [ ] деструктор.  
    - [ ] копирование/присвоение.  
    - [ ] operator<<  
    - [ ] existTable(name)  
    - [ ] existColumn(table, column)  
    - [ ] dropTable(name)  
    - [ ] dropColumn(table, column)  
  - [x] и нескольких служебных классов.  

[0]: #availableDatabase "true, если к база существует, и доступна"  
[1]: #existDatabase "true, если к база существует"  
[2]: #existTable "true, если таблица существует"  
[3]: #existColumn "true, если столбец существует"  

[4]: #dropDatabase "физически уничтожает базу данных"  
[5]: #dropTable "физически уничтожает таблицу"  
[6]: #dropColumn "физически уничтожает столбец"  



## availableDatabase
- [x] true, если база существует, и к ней можно успешно подключитьс€.  
- [x] может бросить исключение.  
- [x] assert(filename);  

```c++
bool availableDatabase(const char* filename);

templaye<class s> 
bool availableDatabase(const s& filename);
```

## existDatabase
- [x] если база существует, вернет true.  
- [x] может бросить исключение.  

```c++
bool existDatabase(const char*  filename);

templaye<class s> 
bool existDatabase(const s& filename);
```

## existColumn
```c++
bool existColumn(const chat* filename, const char* column);

templaye<class s1, class s2> 
bool existColumn(const s1& filename, const s2& column);
```
¬озвразают `true`, если в указанной базе данных существует указанный столбец.  


## dropDatabase  
## dropTable  
## dropColumn   





db::connection  
--------------
√лавный класс библиотеки.  
—лужит дл€ подключени€ к базе данных, 
и осуществлени€ различных запросов.  

  - [x] √арантирует Thread-Safe.  


