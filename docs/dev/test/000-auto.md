
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
    - [ ] eREADWRITE
    - [ ] eREADONLY
    - [ ] eOPEN_URI
    - [ ] eCREATE
  - [ ] [db::connection][8]
    - [ ] Создание  
    - [ ] Копирование  
    - [ ] Перемещение  
    - [ ] Присвоение  

<br />
<br />

[7]: ../arch/002-connect.md/#connect
[8]: ../arch/003-connection.md/#Конструкторы





## connection
  - [ ] [connection::operator<<(sql)][9]  
  - [ ] [connection::existTable(name)][10]  
  - [ ] [connection::existColumn(table, column)][11]  
  - [ ] [connection::dropTable(name)][12]  
  - [ ] [connection::dropColumn(table, column)][13]  

[9]:  ../arch/003-connection.md/#operator-sql
[10]: ../arch/003-connection.md/#existTable
[11]: ../arch/003-connection.md/#existColumn
[12]: ../arch/003-connection.md/#dropTable
[13]: ../arch/003-connection.md/#dropColumn





## management deprecated  
  - [x] Тесты `db::existDatabase`  
    - [x] true, если к базе можно успешно подключиться.  
  - [x] Тесты `db::existTable`  
  - [x] Тесты `db::existColumn`  
  - [x] Тесты `db::dropDatabase`  
    - [x] Физически удаляет файл с диска.  
  - [ ] Тесты `db::dropTable`  

<br />
<br />






## type_safe  
  - [ ] Тесты типо-безопасности.  

<br />
<br />






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
