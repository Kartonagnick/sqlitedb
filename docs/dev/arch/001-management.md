
Список функций
--------------

- [ ] [cleanDatabase](#cleanDatabase "обслуживание базы: чек-поинты, вакуумб анализ")
- [x] [availableDatabase](#availableDatabase "Проверяет доступность базы данных для использования")
- [x] [existDatabase](#existDatabase "Проверяет существование базы данных")
- [x] [existTable](#existTable "Проверяет существует ли указанная таблица")
- [x] [existColumn](#existColumn "Проверяет существует ли указанный столбец")
- [x] [dropDatabase](#dropDatabase "Удаляет базу данных")
- [x] [dropTable](#dropTable "Удаляет таблицу из базы")
- [ ] [dropColumn](#dropColumn "Удаляет из таблицы столбец")

<br />
<br />






## cleanDatabase
Обслуживание базы: чек-поинты, вакуум, аналиp.  
Эту функцию полезно вызывать перед бэкапами.  
Она удаляет весь накопившийся временный хлам, 
и осуществляет оптимизацию.  

```c++
template <class s> 
void cleanDatabase(const s& filename);

void cleanDatabase(const char* filename);
```

- [x] если база не доступна -> бросит исключение.  
- [x] если соединение readonly -> бросит исключение.  
- [x] может бросить исключение. (например: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  



## availableDatabase
Проверяет доступность базы данных для использования.  

```c++
template <class s> 
bool availableDatabase(const s& filename);

bool availableDatabase(const char* filename);
```

- [x] true, если база существует, и к ней можно подключиться.  
- [x] false, если SQLITE3 вернула ошибку подключения.  
- [x] может бросить исключение. (например: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />





## existDatabase
Проверяет существование базы данных.  

```c++
template <class s> 
bool existDatabase(const s& filename);

bool existDatabase(const char*  filename);
```

- [x] true, если на диске существует файл.  
- [x] может бросить исключение. (например: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />





## existTable
Проверяет существует ли указанная таблица.  

```c++
template <class s1, class s2> 
bool existTable(const s1& filename, const s2& table);

bool existTable(const char* filename, const char* table);
```
- [x] true, если таблица существует.  
- [x] если база не доступна -> бросит исключение.  
- [x] может бросить исключение. (например: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(filename);  
- [x] assert(table);  

<br />
<br />





## existColumn
Проверяет существует ли указанный столбец.  

```c++
template <class s1, class s2, class s3> 
bool existColumn(const s1& filename, const s2& table, const s3& column);

bool existColumn(const char* filename, const char* table, const char* column);
```

- [x] true, если столбец существует.  
- [x] если база не доступна -> бросит исключение.  
- [x] если таблица отсутствует -> бросит исключение.  
- [x] может бросить исключение. (например: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(!column.empty());  
- [x] assert(filename);  
- [x] assert(table);  
- [x] assert(column);  

<br />
<br />





## dropDatabase  
Удаляет базу данных.  

```c++
template <class s> 
bool dropDatabase(const s& filename);

bool dropDatabase(const char* filename);
```
- [x] Если файл был успешно удален -> возвращает true.  
- [x] Если базы не существует -> возвращает false.  
- [x] Если базу удалить не получилось -> выбрасывает исключение.  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />




## dropTable  
Удаляет таблицу из базы.  

```c++
template <class s1, class s2> 
bool dropTable(const s1& filename, const s2& table);

bool dropTable(const char* filename, const char* table);
```
- [x] Если таблица была успешно удалена -> возвращает true.  
- [x] Если таблицы не существует -> возвращает false.
- [x] Если базы не существует -> бросает исключение.
- [x] Если базы не доступна -> бросает исключение.
- [x] Если таблицу удалить не получилось -> выбрасывает исключение.
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(filename);  
- [x] assert(table);  

<br />
<br />





## dropColumn  
Удаляет из таблицы столбец.  

```c++
template <class s1, class s2, class s3> 
bool dropColumn(const s1& filename, const s2& table, const s2& column);

bool dropColumn(const char* filename, const char* table, const char* column);
```
- [ ] Если столбец был успешно удален -> возвращает true.  
- [ ] Если столбца не существует -> возвращает false.  
- [ ] Если базы не существует -> бросает исключение.  
- [ ] Если таблицы не существует -> бросает исключение.  
- [ ] Если столбец не удалось удалить -> бросает исключение.  
- [ ] assert(!filename.empty());  
- [ ] assert(!table.empty());  
- [ ] assert(!column.empty());  
- [ ] assert(filename);  
- [ ] assert(table);  
- [ ] assert(column);  

<br />
<br />
