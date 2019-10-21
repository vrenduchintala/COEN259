/* math.c */

int main(void)
{
    int x, y, z;
    int a, b, c, d, e;

    x = 100;
    y = 30;
    z = 2;

    a = x + y + z;
    b = x - y - z;
    c = x * y * z;
    d = x / y + z;
    e = x % y - z;

    printf("%d %d %d %d %d\n", a, b, c, d, e);

    a = x > y + z;
    b = (x == y) - z;
    c = x >= y == z > y;
    d = x + (y <= y) * z;
    e = x > 0 != y > z;

    printf("%d %d %d %d %d\n", a, b, c, d, e);
}
