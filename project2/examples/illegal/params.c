int x[10];
char y[10];

int f(int a[], int x)
{
}

int g(int y, char b[])
{
    int a[10];
    char c;

    f(b, y);			/* invalid arguments to 'f' */
    f(a, c);
    y = f(x, 1);
}

int h(char s[], char t[], int n)
{
    n = g(n, s);
    n = g(s, t);		/* invalid arguments to 'g' */
    g(n, y);
    g(x, y);			/* invalid arguments to 'g' */
}
