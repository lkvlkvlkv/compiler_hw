# Final Project - LLVM IR

## 作者

410985013 羅凱威

## 編譯器支援的文法規則或功能

```
Program:
      GlobalStatements
    ;

GlobalStatements:
      GlobalStatement
    | GlobalStatements GlobalStatement
    ;

GlobalStatement:
      FunctionDeclaration
    | FunctionDefinition SEMICOLON
    | DeclarationExpression SEMICOLON
    ;

FunctionDefinition:
      type Identifier LPAREN FPDeclarationList RPAREN
    ;

FunctionDeclaration:
      FunctionDefinition FunctionBlock
    ;

FPDeclarationList:
      /* empty */
    | FPDeclarationList COMMA FPDeclaration
    | FPDeclaration
    ;

FPDeclaration:
      type Identifier
    ;

FunctionBlock:
      LBRACE FunctionStatements RBRACE
    ;

FunctionStatements:
      FunctionStatement
    | FunctionStatements FunctionStatement
    ;

FunctionStatement:
      Statement
    | KW_RETURN Expression SEMICOLON
    | KW_RETURN SEMICOLON
    ;

FunctionCallExpression:
      Identifier LPAREN FCParameterList RPAREN
    ;

FCParameterList:
      /* empty */
    | FCParameterList COMMA Expression
    | Expression
    ;


Statement:
      DeclarationExpression SEMICOLON
    | AssignExpression SEMICOLON
    | FunctionCallExpression SEMICOLON
    | IfStatement
    | WhileStatement
    ;

WhileStatement:
      KW_WHILE LPAREN Condition RPAREN Block
    ;

IfStatement:
      KW_IF LPAREN Condition RPAREN Block
    | KW_IF LPAREN Condition RPAREN Block KW_ELSE Block
    ;

Block:
      Statement
    | LBRACE Statements RBRACE
    ;

Statements:
      /* empty */
    | Statements Statement
    ;

DeclarationExpression:
      type DeclarationList
    ;

DeclarationList:
      DeclarationList COMMA Identifier
    | DeclarationList COMMA AssignExpression
    | AssignExpression
    | Identifier
    ;

AssignExpression:
      Identifier OP_ASSIGN Expression
    ;

Condition:
      Expression COM_LT Expression
    | Expression COM_GT Expression
    | Expression COM_EQ Expression
    | Expression COM_LE Expression
    | Expression COM_GE Expression
    | Expression COM_NE Expression
    ;

Expression:
      Term
    | Expression OP_PLUS Term
    | Expression OP_MINUS Term
    ;

Term:
      Factor
    | Term OP_MULT Factor
    | Term OP_DIV Factor
    ;

Factor:
      Identifier
    | Numeric
    | LPAREN Expression RPAREN
    | FunctionCallExpression
    ;

Identifier:
      IDENTIFIER
    ;

Numeric:
      NUMBER
    | FRAC_NUMBER
    ;

type:
      KW_INT
    | KW_Double
    | KW_VOID
    ;
```

## 編譯器製作的過程

### 如何撰寫

利用 flex 進行詞法分析，bison 進行語法分析，並在語法分析的途中，建出語法抽象樹 AST(Abstract Syntax Tree)，
接著遍歷 AST 並產生 LLVM IR（Intermediate Representation），最後利用 LLVM Backends 將 LLVM IR 編譯成目標代碼 (object code)，
生成的目標代碼可直接編譯成執行檔，也可以與其他目標代碼一同編譯成執行檔。

## 遭遇的困難

LLVM 算是中大型的專案，在 windows 下載並成功建置就需要一番工夫。

另外 LLVM 歷史悠久，網路上的資源未必適用於新版本，我的參考資料多數都超過五年以上。

## 如何解決問題

仔細閱讀 LLVM 官方文件，並參考其他人的經驗，通常我遇到的問題，都會有其他人遇到。
有複雜，難以描述清楚的問題，就透過 GPT 輔助，將問題拆解成小問題後再丟給 Google。

## 編譯器製作的成果：測試程式執行結果的截圖

```c
int a = 1.5, b, c;  // global
void printInt(int a);
void printDouble(int a);

int testFunction(int a, int b);
void nothing();

int main() {
    int a = 1.5, b;  // local
    double z;
    double x = 3.14, y = 2.71;

    printDouble(x);
    printDouble(y);
    /*
    multiline comment
    */

    int z = testFunction(1, 2);

    a = 5;
    b = a + 10;
    c = (a + b) * 2 / testFunction(1, 2); // global c = (5 + 15) * 2 / 3 = 40 / 3 = 13

    if (a == b) {
        c = a + b;
    }
    else if (a > b) {
        c = a - b;
    }
    else {
        c = a * b; // c = 75
    }

    while (c > 0) {
        c = c / 2;
    }

    testFunction(3 + 5, a + b);

    printInt(c); // output 13
    return 0;
}

int testFunction(int a, int b) {
    int c = a + b; // local c = a + b
    return c;
}

void nothing() {
    return;
}

```

## 編譯器製作的心得

這次的專案讓我對編譯器的運作有了更深入的了解，也讓我對 LLVM 有了初步的認識。

## 參考文獻

<https://llvm.org/docs/GettingStarted.html#tools>
<https://bayareanotes.com/llvm-ir-intro/>
<https://blog.csdn.net/zzhongcy/article/details/93753017>
<https://medium.com/codex/building-a-c-compiler-using-lex-and-yacc-446262056aaa>
<https://www.gnu.org/software/bison/manual/html_node/Shift_002fReduce.html>
<https://stackoverflow.com/questions/12731922/reforming-the-grammar-to-remove-shift-reduce-conflict-in-if-then-else>
<https://ithelp.ithome.com.tw/users/20157613/ironman/6494>
<https://gnuu.org/2009/09/18/writing-your-own-toy-compiler/>
<https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html>
