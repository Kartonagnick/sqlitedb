Архитектура  
-----------
- [x] Описание версии биллиотеки [confbuild.hpp][0]
- [x] Весь код библиотеки расположен в пространстве имен `db`  
- [x] Вся библиотека состоит:  
  - [x] Нескольких функций управления (management.hpp):  
    - [x] [db::availableDatabase(filename)][1]  
    - [x] [db::existDatabase(filename)][2]  
    - [x] [db::existTable(filename, table)][3]  
    - [x] [db::existColumn(filename, table, column)][4]  
    - [x] [db::dropDatabase(filename)][5]  
    - [x] [db::dropTable(filename, table)][6]  
    - [ ] [db::dropColumn(filename, table, column)][7]  
  - [x] Функция создания соединения:  
    - [x] [db::connect(filename, params...)][8]  
  - [x] публичные классы:  
    - [x] [db::connection][9]  
  - [x] служебно-публичные классы:  
    - [x] [db::request][10]  
    - [x] [db::cursor][11]  
    - [x] [db::iterator][12]  
  - [x] приватные классы:  
    - [x] [db::device][13]  
    - [x] [db::exception][14]  


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
[11]: arch/005-cursor.md/#cursor    "отвечает за получение данных"  
[12]: arch/006-iterator.md/#iterator  "отвечает за получение контейнеров данных"  
[13]: arch/007-device.md/#device    "низко-уровневый служебный класс для работы с sqlite3"  
[14]: arch/008-exception.md/#exception  "класс исключений"  
