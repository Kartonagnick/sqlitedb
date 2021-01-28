
Тестирование функциональности
-----------------------------

## staff
  - [x] Требования:  
    - [x] Располагается: `staff_sqlitedb`  
    - [x] Алиас: `staff`  
    - [x] assert(filename);  
    - [x] assert(!filename.empty());  
  - [x] Тесты `staff::openRead`  
  - [x] Тесты `staff::openWrite`  
  - [x] Тесты `staff::fileExists`  
  - [x] Тесты `staff::fileDelete`  
  - [x] Тесты `staff::dbaseDelete`  
  - [x] Тесты `staff::makeTableAge`  

<br />
<br />







## management
  - [x] Требования:  
    - [x] assert(filename);  
    - [x] assert(!filename.empty());  
  - [x] Тесты служебной функции **db::exists**  
  - [x] Тесты служебной функции **db::remove**  
  - [x] Тесты API функции [db::availableDatabase][0]
  - [x] Тесты API функции [db::existDatabase][1]
  - [x] Тесты API функции [db::existTable][2]
  - [x] Тесты API функции [db::existColumn][3]
  - [x] Тесты API функции [db::dropDatabase][4]
  - [x] Тесты API функции [db::dropTable][5]
  - [ ] Тесты API функции [db::dropColumn][6]

<br />
<br />

[0]: ../arch/001-management.md/#availableDatabase  
[1]: ../arch/001-management.md/#existDatabase
[2]: ../arch/001-management.md/#existTable
[3]: ../arch/001-management.md/#existColumn
[4]: ../arch/001-management.md/#dropDatabase
[5]: ../arch/001-management.md/#dropTable
[6]: ../arch/001-management.md/#dropColumn





## connect
  - [ ] [db::connect][7]
    - [x] eREADWRITE
    - [x] eREADONLY
    - [ ] eOPEN_URI
    - [x] eCREATE
  - [x] [db::connection][8]
    - [x] Создание  
    - [x] Копирование  
    - [x] Перемещение  
    - [x] Присвоение  

<br />
<br />

[7]: ../arch/002-connect.md/#connect
[8]: ../arch/003-connection.md/#Конструкторы




## table/column
  - [x] [db::connect::existTable][9]
  - [x] [db::connect::existColumn][10]
  - [x] [db::connect::dropTable][11]
  - [ ] [db::connect::dropColumn][12]

<br />
<br />

[9]:  ../arch/003-connection.md/#existTable
[10]: ../arch/003-connection.md/#existColumn
[11]: ../arch/003-connection.md/#dropTable
[12]: ../arch/003-connection.md/#dropColumn




## connection
  - [x] [db::connect::operator<<(sql)][13]  
    - assert защита:  
      - [x] В качестве SQL запроса используем nullptr  
      - [x] В качестве SQL запроса используем пустую строку.  
    - [x] Должен бросить исключение:  
      - [x] В качестве SQL запроса используем бессмысленный набор букв,  

<br />
<br />

[13]:  ../arch/003-connection.md/#operator-sql






## type_safe  
  - [ ] Простейший тест на запись и чтение данных:  
    - [ ] Создаём несколько таблиц:  
      - [ ] В каждую таблицу добавляем несколько строк в цикле.  
      - [ ] А потом считываем их  
      - [ ] Исходные и полученные данные должны быть идентичны.  

  - [ ] Тест на безопасность типов аргументов:  
    - [ ] Создаём таблицу  
      - [ ] Добавляем строку, используя некоррктные аргументы.  
      - [ ] [request::operator<<][14] должен бросить исключение.  

  - [ ] Тест на безопасность типа получателя данных:  
    - [ ] Создаём таблицу  
      - [ ] Добавляем туда несколько строк.  
      - [ ] Запрашиваем кол-во строк в таблице.  
        - [ ] В качестве получателя данных указываем 'std::string'  
        - [ ] [request::operator<<][14] должен бросить исключение.  
      - [ ] Теперь, В качестве получателя данных указываем 'size_t'  
        - [ ] База должна вернуть корректное значение.  

  - [ ] Тест на получение одночного значения.  
    - [ ] База должна вернуть ровно одно единственное значение.  
      - [ ] Если ответов нет -> выбрасываем исключение.  
      - [ ] Если ответов больше 1 -> выбрасываем исключение.  

<br />
<br />

[14]:  ../arch/004-request.md/#operator-in
[15]:  ../arch/005-cursor.md/#operator-in





## lambda  
  - [ ] Тесты работы с лямбдой.  

<br />
<br />






## serialize  
  - [ ] Тесты работы с пользовательскими типами.  

<br />
<br />






## container  
  - [ ] Тесты работы с контейнерами.  
