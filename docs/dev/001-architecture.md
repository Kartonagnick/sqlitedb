Архитектура  
-----------
- [x] Описание версии биллиотеки [confbuild.hpp][0]
- [x] Весь код библиотеки расположен в пространстве имен `db`  
- [x] Вся библиотека состоит:  
  - [ ] Нескольких функций управления (management.hpp):  
    - [ ] [db::availableDatabase(filename)][1]  
    - [ ] [db::existDatabase(filename)][2]  
    - [ ] [db::existTable(filename, table)][3]  
    - [ ] [db::existColumn(filename, table, column)][4]  
    - [ ] [db::dropDatabase(filename)][5]  
    - [ ] [db::dropTable(filename, table)][6]  
    - [ ] [db::dropColumn(filename, table, column)][7]  
  - [x] Функция создания соединения:  
    - [ ] [db::connect(filename, params...)][8]  
  - [x] публичные классы:  
    - [ ] [db::connection][9]  
  - [x] служебно-публичные классы:  
    - [ ] [db::request][10]  
    - [ ] [db::cursor][11]  
  - [x] приватные классы:  
    - [ ] [db::device][12]  


[0]:  arch/000-confbuild.md "версия библиотеки"  

[1]:  arch/001-management.md/#availableDatabase "true, если база существует, и доступна"  
[2]:  arch/001-management.md/#existDatabase     "true, если база существует"  
[3]:  arch/001-management.md/#existTable        "true, если таблица существует"  
[4]:  arch/001-management.md/#existColumn       "true, если столбец существует"  
[5]:  arch/001-management.md/#dropDatabase      "удаляет базу данных"  
[6]:  arch/001-management.md/#dropTable         "удаляет таблицу"  
[7]:  arch/001-management.md/#dropColumn        "удаляет столбец"  

[8]:  arch/002-connect.md/#connect    "функция создания нового соединения"  
[9]:  arch/003-connection.md/#connect    "объект соединения"  
[10]: arch/004-request.md/#request   "объект запроса"  
[11]: arch/005-cursor.md/#cursor    "объект, определяющий текущую позицию в таблице результата"  
[12]: arch/006-device.md/#device    "низко-уровневый служебный класс для работы с sqlite3"  
