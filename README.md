
[![logo](docs/SQLiteDB25.png)](docs/home.md "for developers") 

SQLiteC++ (SQLiteCpp) is a smart and easy to use C++ SQLite3 wrapper.  
Keywords: sqlite, sqlite3, C, library, wrapper C++  

Враппер для sqlite3  
-------------------

Гарантирует типобезопасность:  

```
    size_t count = 0;
    const char* sql = "SELECT count(*) FROM trailers WHERE login = ?";

    auto connect = db::connect("name.db");
    connect << sql << login 
            >> count;
    std::cout << "count = " << count << '\n';
```

Типобезопасный перебор строк при помощи лямбда-итераторов:

```
        std::string text = "not exist";
        const auto getter = [&text](const std::string& value)
        {
            text = value;
            return true; 
              // true  - выдать следующую строку.
              // false - остановить цикл.
        };
        db::connect("same.db") 
           << "SELECT features FROM users WHERE login = ?"
           << login
           >> getter;
```

Поддержка пользовательских типов:  

```
    struct Result { std::string text; }; 

    struct Request
    {
        std::string login;
        std::string password;
    };
    Request request { "123", "pass" };
    Result  dst;

    connect 
        << "select * from desc where login = ? and password = ?"
        << request
        >> dst;
```

Возможность работать с контейнерами:  

```
    std::list<login> logins;
    connect 
        << "select * from logins" 
        >> logins;
```

Поддержка работы с std::tuple:  

```
    std::tuple<int, std::string> result;
    connect 
        << "select * from sample" 
        >> result;
```

Поддержка работы с std::tie:  


```
    int login;
    std::string dsc;
    connect
        << "select * from sample" 
        >> std::tie(login, dsc);
```

1) [История](docs/history.md)  


