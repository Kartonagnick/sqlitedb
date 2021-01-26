
Список функций
--------------

- [ ] [availableDatabase](#availableDatabase "Проверяет доступность базы данных для использования")
- [ ] [existDatabase](#existDatabase "Проверяет существование базы данных")
- [ ] [existTable](#existTable "Проверяет существует ли указанная таблица")
- [ ] [existColumn](#existColumn "Проверяет существует ли указанный столбец")
- [ ] [dropDatabase](#dropDatabase "Удаляет базу данных")
- [ ] [dropTable](#dropTable "Удаляет таблицу из базы")
- [ ] [dropColumn](#dropColumn "Удаляет из таблицы столбец")

<br />
<br />





## availableDatabase
Проверяет доступность базы данных для использования.  
```c++
template <class s> 
bool availableDatabase(const s& filename);

bool availableDatabase(const char* filename);
```
- [ ] true, если база существует, и к ней можно подключиться.  
- [ ] может бросить исключение.  
- [ ] assert(filename);  

<br />
<br />





## existDatabase
Проверяет существование базы данных.  
```c++
template <class s> 
bool existDatabase(const s& filename);

bool existDatabase(const char*  filename);
```
- [ ] true, если на диске существует файл.  
- [ ] может бросить исключение.  
- [ ] assert(filename);  

<br />
<br />





## existTable
Проверяет существует ли указанная таблица.  
```c++
template <class s1, class s2> 
bool existTable(const s1& filename, const s2& table);

bool existTable(const char* filename, const char* table);
```
- [ ] true, если таблица существует.  
- [ ] может бросить исключение.  
- [ ] assert(filename);  
- [ ] assert(table);  

<br />
<br />





## existColumn
Проверяет существует ли указанный столбец.  
```c++
template <class s1, class s2, class s3> 
bool existColumn(const s1& filename, const s2& table, const s3& column);

bool existColumn(const char* filename, const char* table, const char* column);
```
- [ ] true, если столбец существует.  
- [ ] может бросить исключение.  
- [ ] assert(filename);  
- [ ] assert(table);  
- [ ] assert(column);  

<br />
<br />





## dropDatabase  
Удаляет базу данных.  
```c++
template <class s> 
void dropDatabase(const s& filename);

bool dropDatabase(const char* filename);
```
- [ ] Если базы не существует -> просто ничего не делает.
- [ ] Если базу удалить не получилось -> выбрасывает исключение.
- [ ] assert(filename);  

<br />
<br />




## dropTable  
Удаляет таблицу из базы.  
```c++
template <class s1, class s2> 
void dropTable(const s1& filename, const s2& table);

bool dropTable(const char* filename, const char* table);
```
- [ ] Если базы не существует -> бросает исключение.
- [ ] Если таблицы не существует -> просто ничего не делает..
- [ ] Если таблицу удалить не получилось -> выбрасывает исключение.
- [ ] assert(filename);  
- [ ] assert(table);  

<br />
<br />





## dropColumn  
Удаляет из таблицы столбец.  
```c++
template <class s1, class s2, class s3> 
void dropColumn(const s1& filename, const s2& table, const s2& column);

bool dropColumn(const char* filename, const char* table, const char* column);
```
- [ ] Если базы не существует -> бросает исключение.
- [ ] Если таблицы не существует -> бросает исключение.
- [ ] Если столбца не существует -> просто ничего не делает..
- [ ] Если столбец удалить не получилось -> выбрасывает исключение.
- [ ] assert(filename);  
- [ ] assert(table);  
- [ ] assert(column);  

<br />
<br />
