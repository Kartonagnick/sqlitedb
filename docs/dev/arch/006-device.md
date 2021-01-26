device
------

Приватный класс.  
Взаимодействует с api sqlite3.  

Требования к классу:  
  - [ ] Thread-Safe.  
  - [x] Не копируемый.  
  - [x] Не перемещаемый.  

Публичные методы класса:  
  - [ ] [Конструктор][0]  
  - [ ] [Деструктор][1]  
  - [ ] [disconnect][2]  
Приватные методы класса:  
  - [ ] [begQuery][3]  

[0]: #Конструктор   "конструктор сразу создаёт новое соединение"  
[1]: #Деструктор    "разрывает соединение с базой"  
[2]: #disconnect    "обрыв соединения"  
[3]: #begQuery    "подготовка объекта sqlite3_stmt"  


<br />
<br />





Конструктор
-----------
Создаёт новое соединение с базой данных.  

```cpp
    device(
        const str_t& filename,
        const eOPENMODE mode,
        const size_t timeout
    );
```

- [x] `filename` - путь к файлу базы.  
  - [x] assert(!filename.empty());  
- [x] `mode` - [режим открытия базы](002-connect.md/#OPENMODE)
- [x] `timeout` - [таймаут](002-connect.md/#timeout)

<br />
<br />






Деструктор
----------

```cpp
    ~device();
```
- [ ] Защищён при помощи assert.  
- [x] Гарантирует noexcept (no-throw guarantee).  
- [ ] Закрывает соединение.  

<br />
<br />





disconnect
----------
Разрывает соединение с базой.  
Предназначен для вызова непосредственно перед запуском деструктора.  
Что бы можно было отловить возможные исключения.  
(Из деструктора исключения уже не вылетят)  

```cpp
    void disconnect();
```

- [ ] Thread-Safe.  
- [x] Может бросить исключение.  

<br />
<br />





begQuery
--------
Инициализирует объект `sqlite3_stmt`
Которым в свою очередь инициализиуется [request](004-request.md)

```cpp
    private:
        stmtT* begQuery(const char*  sql);
        stmtT* begQuery(const str_t& sql);
```

- [ ] Thread-Safe.  
- [x] Может бросить исключение.  


<br />
<br />


Схема инициализации
-------------------

Объект [connection](003-connection.md/#connection) владеет `device`  
Через него он создает объект `sqlite3_stmt* cursor`  
Который в свою очередь использует для создания объекта [request](004-request.md)  
Дальше уже пользовательский код работает с `request`  

```
    request connection::operator << (const str_t& sql) const
    {
        auto& device = *this->m_data;
        sqlite3_stmt* cursor = device.begQuery(sql);
        return request(cursor);
    }
```

