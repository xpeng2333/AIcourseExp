#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;
string srcNode, tgtNode;
struct f_str {
    friend bool operator<(f_str n1, f_str n2) {
        if (n1.f < n2.f)
            return true;
        else if (n1.f == n2.f && n1.h < n2.h)
            return true;
        else
            return false;
    }
    unsigned short f;
    unsigned short h;
};
multimap<f_str, string> mama;
void reader(short (*Map)[5], string filePath) {
    ifstream input(filePath);
    for (short i = 0; i < 5; i++) {
        for (short j = 0; j < 5; j++) {
            input >> Map[i][j];
        }
    }
    input.close();
}
void short2str(short (*shortMatrix)[5], string &strNode) {
    strNode = "";
    for (short i = 0; i < 5; i++) {
        for (short j = 0; j < 5; j++) {
            if (shortMatrix[i][j] == 0) {
                strNode += shortMatrix[i][j] + 57;
                continue;
            }
            strNode += shortMatrix[i][j] + 34;
        }
    }
}
void str2short(string &strNode, short (*shortMatrix)[5]) {
    for (short i = 0; i < 25; i++) {
        if (strNode[i] == '9') {
            (*shortMatrix)[i] = strNode[i] - 57;
            continue;
        }
        (*shortMatrix)[i] = strNode[i] - 34;
    }
}
unsigned short h1(string nNode, string mNode = tgtNode) {
    unsigned short count = 0;
    for (short i = 0; i < 25; i++) {
        if (nNode[i] == '9')
            continue;
        if (nNode[i] != mNode[i])
            count++;
    }
    return count;
}
unsigned short h2(string nNode, string mNode = tgtNode) {
    unsigned short count = 0;
    short idiv5, jdiv5, imod5, jmod5;
    short flag1_i, flag2_i, flag1_j, flag2_j;
    flag1_i = flag2_i = 2;
    flag1_j = 1;
    flag2_j = 3;
    for (short i = 0; i < 25; i++) {
        if (nNode[i] == '9')
            continue;
        if (nNode[i] == '!') {
            /*
            if (flag1_i == -1) {
                flag1_i = i / 5;
                flag1_j = i % 5;
            } else {
                flag2_i = i / 5;
                flag2_j = i % 5;
            }
            cout << flag1_i + " " + flag1_j << endl;
            */
            continue;
        }
        for (short j = 0; j < 25; j++) {
            if (nNode[i] == tgtNode[j]) {
                imod5 = i % 5;
                jmod5 = j % 5;
                idiv5 = i / 5;
                jdiv5 = j / 5;
                count += abs(imod5 - jmod5) + abs(idiv5 - jdiv5);
                short minrow = min(idiv5, jdiv5);
                short maxrow = max(idiv5, jdiv5);
                short mincow = min(imod5, jmod5);
                short maxcow = max(imod5, jmod5);
                // row--
                if (minrow <= flag1_i && maxrow >= flag1_i &&
                    mincow < flag1_j && maxcow > flag1_j)
                    count--;
                if (minrow <= flag2_i && maxrow >= flag2_i &&
                    mincow < flag2_j && maxcow > flag2_j)
                    count--;
                // cow+=2
                if ((imod5 == jmod5) &&
                    ((imod5 == flag1_j) || (imod5 == flag2_j))) {
                    for (short h = minrow + 1; h < maxrow; h++) {
                        if (nNode[h * 5 + imod5] == '!')
                            count += 2;
                    }
                }
                break;
            }
        }
    }
    return count;
    /*
        if (count < 15)
            return count;
        return count * 1.2;
    */
}
int main() {
    int i = 3;
    mama.insert(make_pair(f_str{3, 1}, "a"));
    mama.insert(make_pair(f_str{3, 2}, "b"));
    mama.insert(make_pair(f_str{2, 1}, "c"));
    mama.insert(make_pair(f_str{3, 2}, "d"));

    cout << mama.begin()->second << endl;
    mama.erase(mama.begin());
    cout << mama.begin()->second << endl;
    mama.erase(mama.begin());
    cout << mama.begin()->second << endl;
    mama.erase(mama.begin());
    cout << mama.begin()->second << endl;
    mama.erase(mama.begin());
    /*
    short srcMap[5][5], tgtMap[5][5];
    string src2tgt;
    reader(srcMap, "input.txt");
    reader(tgtMap, "target.txt");
    short2str(srcMap, srcNode);
    short2str(tgtMap, tgtNode);
    cout << h1(srcNode) << endl;
    cout << h2(srcNode) << endl;
*/
    /*
    priority_queue<f_str> hehe;
    hehe.push({1, 2, "b"});
    hehe.push({1, 1, "a"});
    hehe.push({2, 3, "c"});
    cout << hehe.top().strNode << endl;
    hehe.pop();
    cout << hehe.top().strNode << endl;
    hehe.pop();
    cout << hehe.top().strNode << endl;
    hehe.pop();
    */
    return 0;
}
