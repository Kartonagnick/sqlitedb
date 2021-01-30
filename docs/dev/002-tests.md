
Тестирование функциональности
-----------------------------

[X1]: ../images/failed.png     "2021y-01m-30d"
[V1]: ../images/success.png    "2021y-01m-30d"
[E1]: ../images/nodata.png     "2021y-01m-30d"
[N1]: ../images/na.png         "2021y-01m-30d"

| **ID** | **Сценарии**      | **0.0.2**     |  
|:------:|:-----------------:|:-------------:|  
|  0000  | [staff][0]        | [![][V1]][0]  |  
|  0001  | [management][1]   | [![][V1]][1]  |  
|  0002  | [connect][2]      | [![][V1]][2]  |  
|  0003  | [table/column][3] | [![][V1]][3]  |  
|  0004  | [connection][4]   | [![][V1]][4]  |  
|  0005  | [type_safe][5]    | [![][V1]][5]  |  
|  0006  | [variable][6]     | [![][V1]][6]  |  
|  0006  | [tuple][7]        | [![][V1]][7]  |  
|  0007  | [lambda][8]       | [![][V1]][8]  |  
|  0008  | [serialize][9]    | [![][E1]][9]  |  
|  0009  | [container][10]   | [![][E1]][10] |  


[0]:  test/000-auto.md/#staff          "тестирование staff"  
[1]:  test/000-auto.md/#management     "тестирование глобальных функций управления базой данных"  
[2]:  test/000-auto.md/#connect        "тестирование db::connect, и конструкторов db::connection"  
[3]:  test/000-auto.md/#tablecolumn    "тестирование db::connection - управление таблицами/столбцами"  
[4]:  test/000-auto.md/#connection     "тестирование db::connection - методы класса"  
[5]:  test/000-auto.md/#type_safe      "тестирование db::request - типо-безопасность"  
[6]:  test/000-auto.md/#variable       "тестирование db::request - работа с одиночной переменной"  
[7]:  test/000-auto.md/#tuple          "тестирование db::request - работа с std::tuple"  
[8]:  test/000-auto.md/#lambda         "тестирование db::request - работа с лябдами"  
[9]:  test/000-auto.md/#serialize      "тестирование db::request - работа с пользовательскими типами"  
[10]: test/000-auto.md/#container      "тестирование db::request - работа с контейнерами"  


