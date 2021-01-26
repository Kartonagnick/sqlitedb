
connection  
----------
Главный класс библиотеки.  
Служит для подключения к базе данных, и осуществления различных запросов.  

- [ ] [Требования][0]  
- [x] Члены класса:  
  - [ ] [Деструктор][1]  
  - [ ] [Конструкторы][2]  
  - [ ] [operator=(connection)][3]  
  - [ ] [operator<<(sql)][4]  
  - [ ] [existTable(name)][5]  
  - [ ] [existColumn(table, column)][6]  
  - [ ] [dropTable(name)][7]  
  - [ ] [dropColumn(table, column)][8]  

[0]: #Требования               "требования к реализации"  
[1]: #Деструктор               "описание деструктора"  
[2]: #Конструкторы             "описание конструкторов"  
[3]: #Присвоение               "описание operator="  
[4]: #operator-sql             "оператор ввода текста запроса"  
[5]: #existTable               "существует ли указанная таблица"  
[6]: #existColumn              "существует ли указанный столбец"  
[7]: #dropTable                "удалить таблицу"  
[8]: #dropColumn               "удалить столбец"  

<br />
<br />




Требования
----------
- [x] Нельзя создать неинициализированное соединение.  
  - [x] Если объект существует, значит уже подключился к базе данных.  
- [ ] Thread-Safe.  
  - [ ] Можно без ограничений использовать в разных потоках.  
- [ ] Static initialization order fiasco immunity.  
  - [ ] Можно без ограничений создавать статические объекты.  
  - [ ] Можно без ограничений создавать локальные статические объекты.  
  - [ ] Можно без ограничений создавать thread_local объекты.  
  - [ ] Можно эксплуатировать до запуска main.  

<br />
<br />





Деструктор
----------

```cpp
~connection();
```

- [ ] Защищён при помощи assert.  
- [ ] Гарантирует noexcept (no-throw guarantee).  
- [ ] Закрывает соединение.  





Конструкторы
------------

```cpp
private:
    connection() noexcept;
public:
    connection(const connection&) noexcept;
    connection(connection&&)      noexcept;
```

- [x] Конструктор создания запрещен.  
  - [x] Для создания объекта используйте [db::connect](002-connect.md/#connect)  
- [ ] Конструкторы копирования оптимизированы:  
  - [ ] Никогда не кидают исключений (no-throw guarantee).  
  - [ ] Под капотом используется std::shared_ptr.  
  - [ ] Быстрые.  

<br />
<br />





Присвоение
----------

```cpp
public:
    connection& operator=(const connection&) noexcept;
    connection& operator=(connection&&)      noexcept;
```

- [ ] Операторы присвоения оптимизированы:  
  - [ ] Никогда не кидают исключений (no-throw guarantee).  
  - [ ] Под капотом используется std::shared_ptr.  
  - [ ] Быстрые.  

<br />
<br />





operator<< (sql)
----------------
При помощи этого метода осуществляется ввод SQL запроса.  
Библиотека проанализует текст SQL запроса, и если всё хорошо, то вернет объект [db::request](004-request.md/#request).  
Который, в свою очередь, можно заполнить аргументами запроса, и отправить в базу на исполнение.  
```cpp
    request operator << (const str_t& sql) const;
    request operator << (const char*  sql) const;
```

- [ ] Thread-Safe.  
- [x] Может бросить исключение.  
- [ ] Возвращает объект [db::request](004-request.md/#request)  

<br />
<br />





existTable
----------
Возвращает true, если указанная таблица существует.  

```cpp
    bool existTable(const char* name) const;

    template<class s>
    bool existTable(const s& name) const;
```

- [ ] Thread-Safe.  
- [x] Может бросить исключение.  
- [ ] assert(name);  

<br />
<br />




existColumn
-----------
Возвращает true, если указанный столбец существует.  

```cpp
    bool existColumn(const char* table, const char* column) const;

    template<class s1, class s2>
    bool existColumn(const s1& table, const s2& column) const;
```

- [ ] Thread-Safe.  
- [ ] Если таблицы не существует -> бросает исключение.  
- [x] Может бросить исключение.  
- [ ] assert(table);  
- [ ] assert(column);  

<br />
<br />





dropTable
---------
Удаляет таблицу.  

```cpp
    void dropTable(const str_t& name) const;
```

- [ ] Thread-Safe.  
- [ ] Если таблицы не существует -> ничего не делает.  
- [x] Может бросить исключение.  
- [ ] assert(!name.empty());  

<br />
<br />





dropColumn
----------
Удаляет столбец.  

```cpp
    void dropColumn(const str_t& table, const str_& column) const;
```

- [ ] Thread-Safe.  
- [ ] Если таблицы не существует -> бросает исключение.  
- [x] Может бросить исключение.  
- [ ] assert(!table.empty());  
- [ ] assert(!column.empty());  

<br />
<br />


