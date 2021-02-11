
OPENMODE  
--------

Режим открытия базы данных.

```
enum eOPENMODE 
{ 
    eREADONLY  = 1<<1, 
    eREADWRITE = 1<<2, 
    eOPEN_URI  = 1<<3, 
    eCREATE    = 1<<4 

};
```
- `eREADONLY` Открывает базу в режиме "только для чтения"  
  - База данных должна существовать.  
  - Если не существует -> будет брошено исключение.  

- `eREADWRITE` Открывает базу как для чтения, так и для записи.  
  - База данных должна существовать.  
  - Если не существует -> будет брошено исключение.  

- `eOPEN_URI` Интерпретировать имя базы как [URI](https://habr.com/ru/post/232385/)  

- `eCREATE` Если базы не существует -> она будет созданна.  

<br />
<br />





connect  
-------
Создаёт новое соединение.

```c++
    class connection;

    connection connect(
        const str_t& path, 
        const int mode = eREADONLY,
        const size_t timeout = 150 // milliseconds
    );

    connection connect(
        const char* path, 
        const int mode = eREADONLY,
        const size_t timeout = 150 // milliseconds
    );

```

- `path` путь к базе данных.  
- `mode` задаёт [режим][0] открытия соединения.  
- `timeout` количество миллисекунд, которое мы готовы [ждать][1], пока бд освободится.  
  - [x] При успешном соединении возвращает объект [connection][2]
  - [x] assert(path.empty());  
  - [x] assert(path);  

<br />
<br />





timeout
-------
При параллельной обработке сразу нескольких запросов,
может возникать ситуация, когда один запрос блокирует работу другого запроса.
В этом случае заблокированный запрос ожидает разрешения работать.
Если время ожидания истекло, то запрос терпит неудачу с ошибкой [SQLITE_BUSY](https://www.sqlite.org/rescode.html)  





[0]: #OPENMODE "режим открытия соединения"
[1]: #timeout "время в миллисекундах, которое мы готовы подождать"
[2]: 003-connection.md/#connection "объект соединения"
