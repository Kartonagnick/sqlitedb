
OPENMODE  
--------

����� �������� ���� ������.

```
enum eOPENMODE 
{ 
    eREADONLY  = 1<<1, 
    eREADWRITE = 1<<2, 
    eOPEN_URI  = 1<<3, 
    eCREATE    = 1<<4 

};
```
- `eREADONLY` ��������� ���� � ������ "������ ��� ������"  
  - ���� ������ ������ ������������.  
  - ���� �� ���������� -> ����� ������� ����������.  

- `eREADWRITE` ��������� ���� ��� ��� ������, ��� � ��� ������.  
  - ���� ������ ������ ������������.  
  - ���� �� ���������� -> ����� ������� ����������.  

- `eOPEN_URI` ���������������� ��� ���� ��� [URI](https://habr.com/ru/post/232385/)  

- `eCREATE` ���� ���� �� ���������� -> ��� ����� ��������.  

<br />
<br />





connect  
-------
������ ����� ����������.

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

- `path` ���� � ���� ������.  
- `mode` ����� [�����][0] �������� ����������.  
- `timeout` ���������� �����������, ������� �� ������ [�����][1], ���� �� �����������.  
  - [x] ��� �������� ���������� ���������� ������ [connection][2]
  - [x] assert(path.empty());  
  - [x] assert(path);  

<br />
<br />





timeout
-------
��� ������������ ��������� ����� ���������� ��������,
����� ��������� ��������, ����� ���� ������ ��������� ������ ������� �������.
� ���� ������ ��������������� ������ ������� ���������� ��������.
���� ����� �������� �������, �� ������ ������ ������� � ������� [SQLITE_BUSY](https://www.sqlite.org/rescode.html)  





[0]: #OPENMODE "����� �������� ����������"
[1]: #timeout "����� � �������������, ������� �� ������ ���������"
[2]: 003-connection.md/#connection "������ ����������"
