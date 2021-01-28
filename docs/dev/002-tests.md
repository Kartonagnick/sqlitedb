
Тестирование функциональности
-----------------------------

[X1]: ../images/failed.png     "2021y-01m-28d"
[V1]: ../images/success.png    "2021y-01m-28d"
[E1]: ../images/nodata.png     "2021y-01m-28d"
[N1]: ../images/na.png         "2021y-01m-28d"

| **ID** | **Сценарии**      | **0.0.2**    |  
|:------:|:-----------------:|:------------:|  
|  0000  | [staff][0]        | [![][V1]][0] |  
|  0001  | [management][1]   | [![][V1]][1] |  
|  0002  | [connect][2]      | [![][V1]][2] |  
|  0003  | [connection][3]   | [![][V1]][3] |  
|  0004  | [table/column][4] | [![][V1]][3] |  
|  0005  | [type_safe][5]    | [![][E1]][4] |  
|  0006  | [lambda][6]       | [![][E1]][5] |  
|  0007  | [serialize][7]    | [![][E1]][6] |  
|  0008  | [container][8]    | [![][E1]][7] |  


[0]:  test/000-auto.md/#staff          "тестирование staff"  
[1]:  test/000-auto.md/#management     "тестирование глобальных функций управления базой данных"  
[2]:  test/000-auto.md/#connect        "тестирование db::connect, и конструкторов db::connection"  
[3]:  test/000-auto.md/#connection     "тестирование методов db::connection"  
[4]:  test/000-auto.md/#tablecolumn    "тестирование создания/удаления таблиц или столбцов через db::connection"  
[5]:  test/000-auto.md/#type_safe      "тестирование типо-безопасности"  
[6]:  test/000-auto.md/#lambda         "тестирование работы с лябдами"  
[7]:  test/000-auto.md/#serialize      "тестирование работы с пользовательскими типами"  
[8]:  test/000-auto.md/#container      "тестирование работы с контейнерами"  


