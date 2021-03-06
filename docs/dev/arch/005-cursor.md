
cursor  
------

Задача класса: определить к какой категории относится тип получателя данных.  
И в зависимости от этого типа выбрать ту, или иную стратегию получения данных.  

Требования к классу:  
  - [x] Хэдер зависит только от `request`.  
  - [x] Все методы класса приватные.  
  - [x] Класс не должен использвать мутексы.  

Наследование:  
  `cursor` ---> [iterator][0]  

Конструктор:  
  - [x] [cursor(request&)][1]  

Основные методы класса:  
  - [x] [dFOR_(variable) get(dst)][2]  
  - [x] [dFOR_(tuple) get(dst)][3]  
  - [x] [dFOR_(container) get(dst)][4]  
  - [x] [dFOR_(lambda) get(dst)][5]  

Вспомогательные методы:
  - [x] [get_as_variable][6]  
  - [x] [get_as_tuple][7]  

Вспомогательные методы:
  - [x] [loop(lambda)][8]  
  - [x] [call(obj, method)][9]  
  - [x] [callimpl(obj, method, index_sequence)][9]  
  - [x] [getArg(index)][9]  

[0]: 006-iterator.md       "низкоо-уровневая механика получение данных от request"  
[1]: #Конструктор          "конструктор класса"  
[2]: #Одиночное-значение   "в ответ на запрос ожидаем одно единственное значение"  
[3]: #tuple                "получаем таблицу из одной единственной строки"  
[4]: #Контейнер            "можно сразу загрузить все строки в контейнер"  
[5]: #Лямбда               "с помощью лямбды перебираем строки ответа"  
[6]: #get_as_variable      "отвечает за загрузку данных для одиночной переменной"  
[7]: #get_as_tuple         "отвечает за загрузку данных для std::tuple"  
[8]: #loop                 "в цикле перебирает строки с ответами"  
[9]: #arguments            "bind аргументов sqlite3 на аргументы функции"  

<br />
<br />





Конструктор
-----------

```
cursor(request& owner) noexcept
    : irerator(owner)
{}
```
- [x] Гарантирует noexcept (no-throw guarantee).  
- [x] Инициализиует поле-ссылку.  
  - [x] По этой причине класс не подерживает new(placement)  

<br />
<br />




Одиночное значение
------------------
Используется, когда от базы в качестве ответа ожидается одно единственное значение.  

```cpp
    template<class T>
    dFOR_(variable) get(T& dst) const;
```

- [x] Может бросить исключение.  
- [x] Ожидает только одно значение.  
- [x] assert, если в ответе несколько строк.  

<br />
<br />

get_as_variable
---------------
Отвечает за загрузку данных для одиночной переменной.

Сюда попадаем из метода `dFOR_(variable) get(T& dst)`  
Здесь делается вся настоящая работа.  
```
    template<class T> 
    void get_as_variable(T& dst) const;
```
- [x] Может бросить исключение.  
- [x] Ожидает только одно значение.  
- [x] assert, если в ответе несколько строк.  

<br />
<br />




tuple
-----
Используется для получения одной единственной строки ответа.

```cpp
    template<class T>
    dFOR_(tuple) get(T& dst) const
```

- [ ] Thread-Safe.  
- [x] Может бросить исключение.  
- [x] Если по запросу ничего нет -> ничего не происходит.  
- [x] Количество строк в таблице ответа не должно превышать 1.  
  - [x] Если будет больше 1 -> сработает assert  
- [x] `std::tuple<>` без параметров запрещен времени компиляции.  

<br />
<br />



get_as_tuple
------------
Отвечает за загрузку данных для std::tuple.  
Cюда попадаем из `dFOR_(tuple) get(T& dst)`  
```
    template<class Tuple> 
    void get_as_tuple(Tuple& dst) const;
```
- [x] Может бросить исключение.  
- [x] Если по запросу ничего нет -> ничего не происходит.  
- [x] Количество строк в таблице ответа не должно превышать 1.  
  - [x] Если будет больше 1 -> сработает assert  
- [x] `std::tuple<>` без параметров запрещен времени компиляции.  

<br />
<br />




Контейнер
---------
Используется для получения сразу всех строк ответа,
которые нужно записать в массив.  

```cpp
    template<class T>
    dFOR_(container) get(T& dst) const;
```

- [x] Может бросить исключение.  
- [x] Если по запросу ничего нет -> ничего не происходит.  
- [x] Перебирает в цикле все строки ответа, и копируюет их в конейнер-приймник.

<br />
<br />




Лямбда
------
Используется для перебора всех строк ответа в цикле.

```cpp
    template<class T>
    dFOR_(lambda) get(T& dst) const;
```

- [ ] Thread-Safe.  
  - [ ] При условии, что сама лямбда тоже thread-safe.  
- [x] Может бросить исключение.  
- [x] Если по запросу ничего нет -> ничего не происходит.  
- [x] Если лямбда возвращает true -> нужно обработать следующую строчку.  
- [x] Если лямбда возвращает false -> нужно остановить перебор строк.  

<br />
<br />




loop
----
В цикле перебирает все строки ответа, и запускает лямбду.  
Если лямбда вернет false, значит данные больше не нужны, и цикл нужно прервать.  
Если лямбда вернет true, значит нужно продолжить цикл.  
```cpp
    template<class Lambda> 
    size_t loop(const Lambda& lambda) const
```

<br />
<br />





arguments
---------

Здесь используется трюк, который позволяет сначала вычислить значения всех аргументов, используя api sqlite3,
а затем вызвать произвольную функцию (в нашем случае это - `operator()(args...)`),
передав в качестве аргументов просчитанные раннее значения.  


