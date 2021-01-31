
exception
---------

Приватный класс.  
Служит для передачи информации о сбоях sqlite3 выше по стеку.  
Несет в себе следующую информацию:  
 - Код ошибки.  
 - Текст, в котором указывается откуда вылетело исключение.  

Требования к классу:  
  - [ ] Thread-Safe.  
  - [x] Наследник std::runtime_error




Публичные методы класса:  
  - [x] [Конструктор][0]  
  - [x] [Деструктор][1]  
  - [x] [what][2]  
  - [x] [code][3]  
  - [x] [code2desc][4]  
  - [x] [code2name][5]  
Статические методы класса:  
  - [x] [code2desc(code)][6]  
  - [x] [code2name(code)][7]  

[0]: #Конструктор   "конструктор сразу создаёт новое соединение"  
[1]: #Деструктор    "разрывает соединение с базой"  
[2]: #what          "возвращает текстовое сообщение с диагностикой"  
[3]: #code          "возвращает расширенный код ошибки"  
[4]: #code2desc     "описание кода ошибки"  
[5]: #code2text     "текстовое имя ошибки"  

[6]: #code2desccode    "описание кода ошибки"  
[7]: #code2namecode    "текстовое имя ошибки"  

<br />
<br />





Конструктор
-----------
Принимают строку с доагностикой.  

```cpp
   explicit exception(const int code, str_t msg) noexcept;
   explicit exception(const int code, const char* msg) noexcept;
```

- [x] `code` - расширеный код ошибки.
- [x] `msg` - текстовое описание ошибки.

<br />
<br />






Деструктор
----------

```cpp
    ~exception();
```
- [x] Гарантирует noexcept (no-throw guarantee).  

<br />
<br />





what
----
Возвращает текстовое описание ошибки.

```cpp
    const char* what() noexcept;
```

- [x] Гарантирует noexcept (no-throw guarantee).  

<br />
<br />





code
----
Возвращает расширенный код ошибки.

```cpp
int code() const noexcept
```

- [x] Thread-Safe.  
- [x] Гарантирует noexcept (no-throw guarantee).  


Расширенные коды
----------------

Включают в себя все коды sqlite3, а так же дополнительно:  

```
public:
    enum eEXTENDED_ERROR
    {
        eDATABASE_NOT_AVAILABLE = 26,
        eTABLE_DOES_NOT_EXIST  = 200,  
    };
```

<br />
<br />





code2desc
---------
Текстовое описание ошибки по его числову коду.

```cpp
const char* code2desc() const noexcept;
```

- [x] Thread-Safe.  
- [x] Гарантирует noexcept (no-throw guarantee).  


code2name 
---------
Текстовое имя кода ошибки

```cpp
const char* code2name() const noexcept;
```

code2desc(code)
---------------
Текстовое описание ошибки по его числову коду.

```cpp
const char* code2desc() const noexcept;
```

- [x] Thread-Safe.  
- [x] Гарантирует noexcept (no-throw guarantee).  


code2name(code)
---------------
Текстовое имя кода ошибки

```cpp
const char* code2name() const noexcept;
```

