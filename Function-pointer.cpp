int g(int);
int (*gp)(int);

gp=g;

typedef int (*Fun)(int a, int b);
int m(int, int);

Fun funp = m;

