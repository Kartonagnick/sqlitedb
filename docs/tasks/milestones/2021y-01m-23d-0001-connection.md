
| дата начала         |   дата конца          | длительность  | исполнитель  |
|:-------------------:|:---------------------:|:-------------:|:------------:|
| 2020y-01m-23d 12:00 | 2020y-01m-(28)d xx:xx | (2 weeks)     | Kartonagnick |

Веха: connection.  
Истекает: 2021y-01m-28d  

Цель: построить базовую функциональность `db::connection`.  

 - [ ] Должен уметь подключаться к базе.  
 - [ ] Выполнять любые sql-запросы.  
 - [x] В случае проблем - кидать эксепшены.  
 - [ ] Обеспечивать безопасность типов отправляемых и получаемых данных.  
 - [ ] В цикле перебрать все строки ответа.  
 - [ ] Сериализаций/десериализация пользовательских типов.  
 - [ ] Поддержка контейнеров.  
 - [ ] Поддержка std::tuple.  
 - [ ] Поддержка std::tie.  
 - [ ] Функциональность должна быть покрыта тестами.  

## Типобезопасность.  
```
size_t count = 0;
const char* sql = "SELECT count(*) FROM trailers WHERE login = ?";

auto connect = db::connect("name.db");
connect << sql  << login >> count;
std::cout << "count = " << count << '\n';
```

## Лямбда-Итератор.  
Должна быть предусмотрена возможность запросить таблицу, 
и перебрать её строкам:  

```
        std::string text = "not exist";
        const auto getter = [&text](const std::string& value)
        {
            text = value;
            return true; 
              // true  - выдать следующую строку.
              // false - остановить цикл.
        };
        db::connect("same.db") << "SELECT features FROM users WHERE login = ?"
           << login >> getter;
```

## Пользовательские типы.  
Должна быть возможность организовать поддержку пользовательских типов:  

```
        struct dsc { std::string text; }; 

        struct record
        {
            record(std::string l, std::string p) noexcept
                : login(std::move(l)), password(std::move(p))
            {}
            std::string login;
            std::string password;
        };
        record rec { "123", "pass" };
        dsc  dst;
        connect << "select * from desc where login = ? and password = ?"
            << rec
            >> dst;
```

## Работа с контейнерами.  

Должна быть поддержка работы с контейнерами:  

```
    std::list<login> logins;
    connect << "select * from logins" >> logins;
```

## Работа с std::tuple.  

Должна быть поддержка работы с std::tuple:  

```
    std::tuple<int, std::string> result;
    connect << "select * from sample" >> result;
```

## Работа с std::tie.  

Должна быть поддержка работы с std::tie:  

```
    int login;
    std::string dsc;
    connect << "select * from sample" >> std::tie(login, dsc);
```

