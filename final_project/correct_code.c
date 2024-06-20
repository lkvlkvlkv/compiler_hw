int a = 1.5, b, c;  // global

int testFunction(int a, int b);
void nothing();

int main() {
    int a = 1.5, b;  // local
    float z;
    float x = 3.14, y = 2.71;

    /*
    multiline comment
    */

    int z = testFunction(1, 2);

    a = 5;
    b = a + 10;
    c = (a + b) * 2 / testFunction(1, 2);

    if (a == b) {
        c = a + b;
    }
    else if (a > b) {
        c = a - b;
    }
    else {
        c = a * b; // c = 75
    }

    if (a < b) {
        c = a + 3; // c = 8
    }
    else {
        c = a - b;
    }

    // while (c > 0) {
    //     c = c - 1;
    // }

    // for (int i = 0; i < 10; i = i + 1) {
    //     x = x + y;
    // }

    // do {
    //     x = x - y;
    // } while (x > 0);

    testFunction(3 + 5, a + b);

    return 0;
}

int testFunction(int a, int b) {
    int c = a + b;
    return c;
}

void nothing() {
    return;
}
