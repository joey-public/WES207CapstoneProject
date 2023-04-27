#include <gnuplot-iostream.h>

int main() {
    Gnuplot gp;
    gp << "plot sin(x)\n";
    return 0;
}
