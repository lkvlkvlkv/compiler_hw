int a = 1.5, b, c;  // global

void printInt(int a);
void printDouble(double a);

int testFunction(int a, int b);
void nothing();
int fib(int a);

int main() {
    int a = 1.5, b;  // local
    double z;
    double x = 3.14, y = 2.71;

    printDouble(x);
    /*
    multiline comment
    */

    int z = testFunction(1, 2);
    printInt(z);

    a = 5;
    b = a + 10;
    c = (a + b) * 2 / testFunction(3 + 5, a + b); // global c

    if (a == b) {
        c = a + b;
    }
    else if (a > b) {
        c = a - b;
    }
    else {
        c = a * b; // c = 75
    }
    printInt(c);

    while (c > 0) {
        c = c / 2;
        if (c > 10) {
            continue;
        }
        printInt(c);
    }

    for (int i = 0; i < 10; i = i + 1) {
        if (i == 5) {
            break;
        }
        if (i == 3) {
            continue;
        }
        printInt(i);
    }

    printInt(fib(10));
    return 0;
}

int fib(int a) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        return 1;
    }
    return fib(a - 1) + fib(a - 2);

}

int testFunction(int a, int b) {
    int c = a + b;
    return c;
}

void nothing() {
    return;
}
