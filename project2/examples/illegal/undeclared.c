int a;

int f(int b)
{
    int c;

    c = b + a;
    w = b + a;			/* 'w' undeclared */
    g();
    a = b + x;			/* 'x' undeclared */
}

int h(void)
{
    b = 10;			/* 'b' undeclared */
}
