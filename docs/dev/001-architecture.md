�����������  
-----------
- [x] �������� ������ ���������� [confbuild.hpp][0]
- [x] ���� ��� ���������� ���������� � ������������ ���� `db`  
- [x] ��� ���������� �������:  
  - [ ] ���������� ������� ���������� (management.hpp):  
    - [ ] [db::availableDatabase(filename)][1]  
    - [ ] [db::existDatabase(filename)][2]  
    - [ ] [db::existTable(filename, table)][3]  
    - [ ] [db::existColumn(filename, table, column)][4]  
    - [ ] [db::dropDatabase(filename)][5]  
    - [ ] [db::dropTable(filename, table)][6]  
    - [ ] [db::dropColumn(filename, table, column)][7]  
  - [x] ������� �������� ����������:  
    - [ ] [db::connect(filename, params...)][8]  
  - [x] ��������� ������:  
    - [ ] [db::connection][9]  
  - [x] ��������-��������� ������:  
    - [ ] [db::request][10]  
    - [ ] [db::cursor][11]  
  - [x] ��������� ������:  
    - [ ] [db::device][12]  


[0]:  arch/000-confbuild.md "������ ����������"  

[1]:  arch/001-management.md/#availableDatabase "true, ���� ���� ����������, � ��������"  
[2]:  arch/001-management.md/#existDatabase     "true, ���� ���� ����������"  
[3]:  arch/001-management.md/#existTable        "true, ���� ������� ����������"  
[4]:  arch/001-management.md/#existColumn       "true, ���� ������� ����������"  
[5]:  arch/001-management.md/#dropDatabase      "������� ���� ������"  
[6]:  arch/001-management.md/#dropTable         "������� �������"  
[7]:  arch/001-management.md/#dropColumn        "������� �������"  

[8]:  arch/002-connect.md/#connect    "������� �������� ������ ����������"  
[9]:  arch/003-connection.md/#connect    "������ ����������"  
[10]: arch/004-request.md/#request   "������ �������"  
[11]: arch/005-cursor.md/#cursor    "������, ������������ ������� ������� � ������� ����������"  
[12]: arch/006-device.md/#device    "�����-��������� ��������� ����� ��� ������ � sqlite3"  
