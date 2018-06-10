#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef struct {
    int row;
    int column;
} queen;
typedef struct {
    int num;
    int r;
    int c;

} record;
int IsSame(queen *a, int row, int column, int n) {
    int k;
    for (k = 0; k < n; k++) {
        if (a[k].row == row && a[k].column == column) {
            return 1;
        }
    }
    return 0;
}
int IsAttack(queen a, queen b) {
    int k;
    if (a.column == b.column || a.row == b.row)
        return 1;
    else {
        k = (a.column - b.column) / (a.row - b.row);
        if (k == 1 || k == -1)
            return 1;
    }
    return 0;
}
int h(queen *a, int n, int m) {
    int i, j;
    int count = 0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (IsAttack(a[i], a[j]) == 1) {
                count++;
            }
        }
    }
    return abs(count - m);
}
int main() {
    int scount;
    int i, j;
    int hh, minh, nextmh;
    int n, m;
    int step = 0;
    int cc, rr, minc, minr;
    double time;
    record re;
    queen *pos;
    FILE *fp;
    ifstream input("input.txt");
    input >> n;
    input >> m;
    input.close();
    pos = (queen *)malloc(n * sizeof(queen));
    clock_t start, finish;
    double totaltime;
    start = clock();
    for (i = 0; i < n; i++) {
        pos[i].row = 7;
        pos[i].column = i;
    }
    nextmh = minh = h(pos, n, m);
    scount = 0;
    do {
        minh = nextmh;
        for (i = 0; i < n; i++) {
            rr = pos[i].row;
            for (j = 0; j < n; j++) {
                pos[i].row = j;
                hh = h(pos, n, m);
                if (hh <= nextmh) {
                    re.num = i;
                    re.r = j;
                    nextmh = hh;
                }
            }
            pos[i].row = rr;
        }
        pos[re.num].row = re.r;
        step++;
        if (nextmh == minh) {
            scount++;
        }
    } while (scount < 100);
    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    ofstream output("output_climb.txt");
    for (i = 0; i < n; i++) {
        output << pos[i].row << endl;
    }
    output << time << endl;
    return 0;
}