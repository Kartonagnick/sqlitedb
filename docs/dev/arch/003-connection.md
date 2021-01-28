
connection  
----------
������� ����� ����������.  
������ ��� ����������� � ���� ������, � ������������� ��������� ��������.  

- [ ] [����������][0]  
- [x] ����� ������:  
  - [x] [����������][1]  
  - [x] [������������][2]  
  - [x] [operator=(connection)][3]  
  - [ ] [operator<<(sql)][4]  
  - [ ] [existTable(name)][5]  
  - [ ] [existColumn(table, column)][6]  
  - [ ] [dropTable(name)][7]  
  - [ ] [dropColumn(table, column)][8]  

[0]: #����������               "���������� � ����������"  
[1]: #����������               "�������� �����������"  
[2]: #������������             "�������� �������������"  
[3]: #����������               "�������� operator="  
[4]: #operator-sql             "�������� ����� ������ �������"  
[5]: #existTable               "���������� �� ��������� �������"  
[6]: #existColumn              "���������� �� ��������� �������"  
[7]: #dropTable                "������� �������"  
[8]: #dropColumn               "������� �������"  

<br />
<br />




����������
----------
- [x] ������ ������� �������������������� ����������.  
  - [x] ���� ������ ����������, ������ ��� ����������� � ���� ������.  
- [ ] Thread-Safe.  
  - [ ] ����� ��� ����������� ������������ � ������ �������.  
- [ ] Static initialization order fiasco immunity.  
  - [ ] ����� ��� ����������� ��������� ����������� �������.  
  - [ ] ����� ��� ����������� ��������� ��������� ����������� �������.  
  - [ ] ����� ��� ����������� ��������� thread_local �������.  
  - [ ] ����� ��������������� �� ������� main.  

<br />
<br />





����������
----------

```cpp
~connection();
```

- [x] ������� ��� ������ assert.  
- [x] ������� �� ������ ���������� (no-throw guarantee).  
- [x] ��������� ����������.  





������������
------------

```cpp
private:
    connection() noexcept;
public:
    connection(const connection&) noexcept;
    connection(connection&&)      noexcept;
```

- [x] ����������� �������� ��������.  
  - [x] ��� �������� ������� ����������� [db::connect](002-connect.md/#connect)  
- [x] ������������ ����������� ��������������:  
  - [x] ������� �� ������ ���������� (no-throw guarantee).  
  - [x] ��� ������� ������������ std::shared_ptr.  
  - [x] �������.  

<br />
<br />





����������
----------

```cpp
public:
    connection& operator=(const connection&) noexcept;
    connection& operator=(connection&&)      noexcept;
```

- [x] ��������� ���������� ��������������:  
  - [x] ������� �� ������ ���������� (no-throw guarantee).  
  - [x] ��� ������� ������������ std::shared_ptr.  
  - [x] �������.  

<br />
<br />





operator<< (sql)
----------------
��� ������ ����� ������ �������������� ���� SQL �������.  
���������� ������������ ����� SQL �������, � ���� �� ������, �� ������ ������ [db::request](004-request.md/#request).  
�������, � ���� �������, ����� ��������� ����������� �������, � ��������� � ���� �� ����������.  
```cpp
    request operator << (const str_t& sql) const;
    request operator << (const char*  sql) const;
```

- [x] ���������� ������ [db::request](004-request.md/#request)  
- [x] ���� ������ ������������ -> ������� ����������.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!sql.empty())  
- [x] assert(sql)  

<br />
<br />





existTable
----------
���������� true, ���� ��������� ������� ����������.  

```cpp
    bool existTable(const char* name) const;

    template<class s>
    bool existTable(const s& name) const;
```

- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!name.empty());  
- [x] assert(name);  

<br />
<br />




existColumn
-----------
���������� true, ���� ��������� ������� ����������.  

```cpp
    bool existColumn(const char* table, const char* column) const;

    template<class s1, class s2>
    bool existColumn(const s1& table, const s2& column) const;
```

- [x] ���� ������� �� ���������� -> ������� ����������.  
- [x] ����� ������� ����������.  
- [x] assert(!table.empty());  
- [x] assert(!column.empty());  
- [x] assert(table);  
- [x] assert(column);  

<br />
<br />





dropTable
---------
������� �������.  

```cpp
    bool dropTable(const str_t& name) const;
    bool dropTable(const char* name) const;
```

- [x] ���� ������� ���� ������� ������� -> ���������� true.  
- [x] ���� ������� �� ���������� -> ���������� false.
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!name.empty());  
- [x] assert(name.empty);  

<br />
<br />





dropColumn
----------
������� �������.  

```cpp
    void dropColumn(const str_t& table, const str_& column) const;
```

- [ ] ���� ������� �� ���������� -> ������� ����������.  
- [x] ����� ������� ����������.  
- [ ] assert(!table.empty());  
- [ ] assert(!column.empty());  

<br />
<br />
