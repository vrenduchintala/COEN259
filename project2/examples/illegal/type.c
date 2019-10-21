int a[10], x;

int f(void)
{
    a[x] = 10;
    x = a[10];

    a = x;		/* scalar type required for 'a' */
    x = f;		/* scalar type required for 'f' */

    x[1] = 0;		/* array type required for 'x' */
    x = f[x];		/* array type required for 'f' */

    x(3);		/* function type required for 'x' */
    x = a(10);		/* function type required for 'a' */
}
