confbuild.hpp
-------------

## Версия библиотеки:

```
#define dVERSION_MAJOR      1
#define dVERSION_MINOR      2
#define dVERSION_PATCH      3
```

<br />
<br />

## Версия, отражаемая в свойствах файла:

```
#define dFILE_VERSION       dVERSION_MAJOR, dVERSION_MINOR, dVERSION_PATCH
```

<br />
<br />

## Версия в формате беззнакового числа:

```
#define dFILE_VERSION_NUM   dVERSION_MAJOR * 100 + dVERSION_MINOR * 10 + dVERSION_PATCH
```

<br />
<br />
  
## Версия в формате `AA.BB.CC`  

```
#define dFILE_VERSION_STR \
    dSSTRINGIZE(dVERSION_MAJOR.dVERSION_MINOR.dVERSION_PATCH)
```

<br />
<br />

## Информация об особенностях сборки:  

```
#define dABOUT_ME \
    dTXT_CONFIGURATION "-" dTXT_ADDRESS_MODEL ", " dTXT_RELEASE_STABLE 
```
Пример: `debug-64-bit, unstable`

<br />
<br />

## Полная версия:

```
#define dFULL_VERSION dFILE_VERSION_STR ", " dABOUT_ME
```
Пример использования:

```
#pragma message("sqlitedb: " dFULL_VERSION)
```

Выввод в консоль:

```
sqlitedb: 0.0.2, debug-64-bit, unstable
```
