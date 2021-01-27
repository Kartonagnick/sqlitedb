�����������  
-----------

- [x] ���� ��� ���������� ���������� � ������������ ���� `db`  
- [x] ��� ���������� �������:  
  - [x] �� ���������� ���������� �������:  
    - [ ] [availableDatabase(filename)][0]  
    - [ ] [existDatabase(filename)][1]  
    - [ ] [existTable(filename, table)][2]  
    - [ ] [existColumn(filename, table, column)][3]  
    - [ ] [dropDatabase(filename)][4]  
    - [ ] [dropTable(filename, table)][5]  
    - [ ] [dropColumn(filename, table, column)][6]  
  - [x] ��������� ����� db::connection.  
    - [ ] �����������.  
    - [ ] ����������.  
    - [ ] �����������/����������.  
    - [ ] operator<<  
    - [ ] existTable(name)  
    - [ ] existColumn(table, column)  
    - [ ] dropTable(name)  
    - [ ] dropColumn(table, column)  
  - [x] � ���������� ��������� �������.  

[0]: #availableDatabase "true, ���� � ���� ����������, � ��������"  
[1]: #existDatabase "true, ���� � ���� ����������"  
[2]: #existTable "true, ���� ������� ����������"  
[3]: #existColumn "true, ���� ������� ����������"  

[4]: #dropDatabase "��������� ���������� ���� ������"  
[5]: #dropTable "��������� ���������� �������"  
[6]: #dropColumn "��������� ���������� �������"  



## availableDatabase
- [x] true, ���� ���� ����������, � � ��� ����� ������� ������������.  
- [x] ����� ������� ����������.  
- [x] assert(filename);  

```c++
bool availableDatabase(const char* filename);

templaye<class s> 
bool availableDatabase(const s& filename);
```

## existDatabase
- [x] ���� ���� ����������, ������ true.  
- [x] ����� ������� ����������.  

```c++
bool existDatabase(const char*  filename);

templaye<class s> 
bool existDatabase(const s& filename);
```

## existColumn
```c++
bool existColumn(const chat* filename, const char* column);

templaye<class s1, class s2> 
bool existColumn(const s1& filename, const s2& column);
```
���������� `true`, ���� � ��������� ���� ������ ���������� ��������� �������.  


## dropDatabase  
## dropTable  
## dropColumn   





db::connection  
--------------
������� ����� ����������.  
������ ��� ����������� � ���� ������, 
� ������������� ��������� ��������.  

  - [x] ����������� Thread-Safe.  


