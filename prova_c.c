#include <stdio.h>
int f(int x, int y) {
int z = x+y;
if (z > 0) {
double x = 42.0;
std::cout << x << "\n"; // valid: 'double x' shadows 'int x'
}
std::cout << x << "\n"; // this is again 'int x'
return z;
}

