
������ �������
--------------

- [ ] [cleanDatabase](#cleanDatabase "������������ ����: ���-������, ������� ������")
- [x] [availableDatabase](#availableDatabase "��������� ����������� ���� ������ ��� �������������")
- [x] [existDatabase](#existDatabase "��������� ������������� ���� ������")
- [x] [existTable](#existTable "��������� ���������� �� ��������� �������")
- [x] [existColumn](#existColumn "��������� ���������� �� ��������� �������")
- [x] [dropDatabase](#dropDatabase "������� ���� ������")
- [x] [dropTable](#dropTable "������� ������� �� ����")
- [ ] [dropColumn](#dropColumn "������� �� ������� �������")

<br />
<br />






## cleanDatabase
������������ ����: ���-������, ������, �����p.  
��� ������� ������� �������� ����� ��������.  
��� ������� ���� ������������ ��������� ����, 
� ������������ �����������.  

```c++
template <class s> 
void cleanDatabase(const s& filename);

void cleanDatabase(const char* filename);
```

- [x] ���� ���� �� �������� -> ������ ����������.  
- [x] ���� ���������� readonly -> ������ ����������.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  



## availableDatabase
��������� ����������� ���� ������ ��� �������������.  

```c++
template <class s> 
bool availableDatabase(const s& filename);

bool availableDatabase(const char* filename);
```

- [x] true, ���� ���� ����������, � � ��� ����� ������������.  
- [x] false, ���� SQLITE3 ������� ������ �����������.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />





## existDatabase
��������� ������������� ���� ������.  

```c++
template <class s> 
bool existDatabase(const s& filename);

bool existDatabase(const char*  filename);
```

- [x] true, ���� �� ����� ���������� ����.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />





## existTable
��������� ���������� �� ��������� �������.  

```c++
template <class s1, class s2> 
bool existTable(const s1& filename, const s2& table);

bool existTable(const char* filename, const char* table);
```
- [x] true, ���� ������� ����������.  
- [x] ���� ���� �� �������� -> ������ ����������.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(filename);  
- [x] assert(table);  

<br />
<br />





## existColumn
��������� ���������� �� ��������� �������.  

```c++
template <class s1, class s2, class s3> 
bool existColumn(const s1& filename, const s2& table, const s3& column);

bool existColumn(const char* filename, const char* table, const char* column);
```

- [x] true, ���� ������� ����������.  
- [x] ���� ���� �� �������� -> ������ ����������.  
- [x] ���� ������� ����������� -> ������ ����������.  
- [x] ����� ������� ����������. (��������: std::bad_alloc)  
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(!column.empty());  
- [x] assert(filename);  
- [x] assert(table);  
- [x] assert(column);  

<br />
<br />





## dropDatabase  
������� ���� ������.  

```c++
template <class s> 
bool dropDatabase(const s& filename);

bool dropDatabase(const char* filename);
```
- [x] ���� ���� ��� ������� ������ -> ���������� true.  
- [x] ���� ���� �� ���������� -> ���������� false.  
- [x] ���� ���� ������� �� ���������� -> ����������� ����������.  
- [x] assert(!filename.empty());  
- [x] assert(filename);  

<br />
<br />




## dropTable  
������� ������� �� ����.  

```c++
template <class s1, class s2> 
bool dropTable(const s1& filename, const s2& table);

bool dropTable(const char* filename, const char* table);
```
- [x] ���� ������� ���� ������� ������� -> ���������� true.  
- [x] ���� ������� �� ���������� -> ���������� false.
- [x] ���� ���� �� ���������� -> ������� ����������.
- [x] ���� ���� �� �������� -> ������� ����������.
- [x] ���� ������� ������� �� ���������� -> ����������� ����������.
- [x] assert(!filename.empty());  
- [x] assert(!table.empty());  
- [x] assert(filename);  
- [x] assert(table);  

<br />
<br />





## dropColumn  
������� �� ������� �������.  

```c++
template <class s1, class s2, class s3> 
bool dropColumn(const s1& filename, const s2& table, const s2& column);

bool dropColumn(const char* filename, const char* table, const char* column);
```
- [ ] ���� ������� ��� ������� ������ -> ���������� true.  
- [ ] ���� ������� �� ���������� -> ���������� false.  
- [ ] ���� ���� �� ���������� -> ������� ����������.  
- [ ] ���� ������� �� ���������� -> ������� ����������.  
- [ ] ���� ������� �� ������� ������� -> ������� ����������.  
- [ ] assert(!filename.empty());  
- [ ] assert(!table.empty());  
- [ ] assert(!column.empty());  
- [ ] assert(filename);  
- [ ] assert(table);  
- [ ] assert(column);  

<br />
<br />
