
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

<br />
<br />







## management
  - [x] Тесты служебной функции `db::exists`
  - [x] Тесты служебной функции `db::remove`

<br />
<br />






## construct
  - [x] Тесты конструкторов [db::connection](arch/003-connection.md/#Конструкторы)  
  - [x] Тесты функций:  
    - [x] [db::connect](002-connect.md/#connect)  
    - [x] `db::exest`  
    - [x] `db::remove`  

<br />
<br />





## management  
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
