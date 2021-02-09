
Тестирование функциональности
-----------------------------

[X1]: ../images/failed.png     "2021y-01m-31d"
[V1]: ../images/success.png    "2021y-01m-31d"
[E1]: ../images/nodata.png     "2021y-01m-31d"
[N1]: ../images/na.png         "2021y-01m-31d"

[V2]: ../images/success.png    "2021y-02m-09d"

| **ID** | **Сценарии**      | **0.0.2**     | **0.0.3**     |  
|:------:|:-----------------:|:-------------:|:-------------:|  
|  0000  | [staff][0]        | [![][V1]][0]  | [![][V2]][0]  |  
|  0001  | [management][1]   | [![][V1]][1]  | [![][V2]][1]  |  
|  0002  | [connect][2]      | [![][V1]][2]  | [![][V2]][2]  |  
|  0003  | [table/column][3] | [![][V1]][3]  | [![][V2]][3]  |  
|  0004  | [connection][4]   | [![][V1]][4]  | [![][V2]][4]  |  
|  0005  | [type_safe][5]    | [![][V1]][5]  | [![][V2]][5]  |  
|  0006  | [variable][6]     | [![][V1]][6]  | [![][V2]][6]  |  
|  0006  | [tuple][7]        | [![][V1]][7]  | [![][V2]][7]  |  
|  0007  | [lambda][8]       | [![][V1]][8]  | [![][V2]][8]  |  
|  0008  | [serialize][9]    | [![][V1]][9]  | [![][V2]][9]  |  
|  0009  | [container][10]   | [![][v1]][10] | [![][v2]][10] |  


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


