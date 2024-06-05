# 編譯器程式作業二

## 作者

410985013 羅凱威

## 編譯方式

### make

```bash
make
```

### 不用 make

```bash
bison -d yacc.y
flex lex.l
gcc -o parser lex.yy.c yacc.tab.c
```

## correct code 測試方式

### make

```bash
make run
```

### 不用 make

```bash
./parser < correct_code.c
```

## error code 測試方式

### make

```bash
make run_error
```

### 不用 make

```bash
./parser.exe < "./error_code/error_code1.c" > "./error_code/error_report1.txt"
./parser.exe < "./error_code/error_code2.c" > "./error_code/error_report2.txt"
./parser.exe < "./error_code/error_code3.c" > "./error_code/error_report3.txt"
./parser.exe < "./error_code/error_code4.c" > "./error_code/error_report4.txt"
./parser.exe < "./error_code/error_code5.c" > "./error_code/error_report5.txt"
./parser.exe < "./error_code/error_code6.c" > "./error_code/error_report6.txt"
./parser.exe < "./error_code/error_code7.c" > "./error_code/error_report7.txt"
./parser.exe < "./error_code/error_code8.c" > "./error_code/error_report8.txt"
./parser.exe < "./error_code/error_code9.c" > "./error_code/error_report9.txt"
./parser.exe < "./error_code/error_code10.c" > "./error_code/error_report10.txt"
```

## 清除編譯檔案

### make

```bash
make clean
```