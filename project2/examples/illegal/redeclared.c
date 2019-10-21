int a, b, a;				/* 'a' redeclared */

int f(int x, int y, int x)		/* 'x' redeclared */
{
    int a, y;				/* 'y' redeclared */
    g(a);
}

int b, c;				/* 'b' redeclared */

int g(int x)				/* 'g' redeclared */
{
}
