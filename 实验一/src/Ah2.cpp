#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sys/time.h>

using namespace std;

unsigned long nodeCount = 0;
map<string, unsigned short> strNode_f;
multimap<unsigned short, string> f_strNode;
map<string, string> path;
set<string> doneNode;
short srcMap[5][5], tgtMap[5][5];

string srcNode, tgtNode;
string currNode;

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
            strNode += shortMatrix[i][j] + 34;
        }
    }
}
void str2short(string &strNode, short (*shortMatrix)[5]) {
    for (short i = 0; i < 25; i++) {
        (*shortMatrix)[i] = strNode[i] - 34;
    }
}
unsigned short h1(string nNode, string mNode = tgtNode) {
    unsigned short count = 0;
    for (short i = 0; i < 25; i++) {
        if (nNode[i] != mNode[i])
            count++;
    }
    return count;
}
void expanding() {
    short zero_i, zero_j;
    short tmp_zero_i, tmp_zero_j;
    string tmpNode;
    string currPath = path[currNode];
    doneNode.insert(currNode);
    unsigned short g = strNode_f[currNode] - h1(currNode) + 1;
    f_strNode.erase(f_strNode.begin());
    strNode_f.erase(currNode);
    path.erase(currNode);
    for (short i = 0; i < 25; i++) {
        if (currNode[i] == '"') {
            zero_i = i / 5;
            zero_j = i % 5;
            break;
        }
    }
    short hinderOffset;
    // left
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    hinderOffset = 0;
    tmpNode = currNode;
    while (tmp_zero_j > 0) {
        if (tmp_zero_j - hinderOffset <= 0)
            break;
        if (tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1] == '!') {
            hinderOffset++;
            continue;
        }
        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
            tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1];
        tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1] = '"';
        if (!doneNode.count(tmpNode)) {
            unsigned short tmpf = g + h1(tmpNode);
            if (strNode_f.insert(make_pair(tmpNode, tmpf)).second == false)
                break;
            f_strNode.insert(make_pair(tmpf, tmpNode));
            path.insert(make_pair(tmpNode, currPath + "L"));
            nodeCount++;
            break;
        } else {
            break;
        }
    }
    // right
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    tmpNode = currNode;
    hinderOffset = 0;
    while (tmp_zero_j < 4) {
        if (tmp_zero_j + hinderOffset >= 4)
            break;
        if (tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1] == '!') {
            hinderOffset++;
            continue;
        }
        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
            tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1];
        tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1] = '"';
        if (!doneNode.count(tmpNode)) {
            unsigned short tmpf = g + h1(tmpNode);
            if (strNode_f.insert(make_pair(tmpNode, tmpf)).second == false)
                break;
            f_strNode.insert(make_pair(tmpf, tmpNode));

            path.insert(make_pair(tmpNode, currPath + "R"));
            nodeCount++;
            break;
        } else {
            break;
        }
    }
    // up
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    tmpNode = currNode;
    hinderOffset = 0;
    while (tmp_zero_i > 0) {
        if (tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j] == '!') {
            break;
        }
        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
            tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j];
        tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j] = '"';
        if (!doneNode.count(tmpNode)) {
            unsigned short tmpf = g + h1(tmpNode);
            if (strNode_f.insert(make_pair(tmpNode, tmpf)).second == false)
                break;
            f_strNode.insert(make_pair(tmpf, tmpNode));
            path.insert(make_pair(tmpNode, currPath + "U"));
            nodeCount++;
            break;
        } else {
            break;
        }
    }
    // down
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    tmpNode = currNode;
    hinderOffset = 0;
    while (tmp_zero_i < 4) {
        if (tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j] == '!') {
            break;
        }
        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
            tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j];
        tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j] = '"';
        if (!doneNode.count(tmpNode)) {
            unsigned short tmpf = g + h1(tmpNode);
            if (strNode_f.insert(make_pair(tmpNode, tmpf)).second == false)
                break;
            f_strNode.insert(make_pair(tmpf, tmpNode));
            path.insert(make_pair(tmpNode, currPath + "D"));
            nodeCount++;
            break;
        } else {
            break;
        }
    }
}

int main() {
    reader(srcMap, "input.txt");
    reader(tgtMap, "target.txt");
    short2str(srcMap, srcNode);
    short2str(tgtMap, tgtNode);
    struct timeval start;
    gettimeofday(&start, NULL);
    strNode_f.insert(make_pair(srcNode, h1(srcNode)));
    f_strNode.insert(make_pair(h1(srcNode), srcNode));
    currNode = srcNode;
    path.insert(make_pair(currNode, ""));
    while (h1(currNode)) {
        expanding();
        currNode = f_strNode.begin()->second;
    }
    struct timeval end;
    gettimeofday(&end, NULL);
    ofstream output("output_Ah1.txt", ios::out);
    cout << (end.tv_sec - start.tv_sec) +
                (end.tv_usec - start.tv_usec) / 1000000.0
         << endl;
    cout << path[currNode] << endl;
    cout << (path[currNode]).length() << endl;
    cout << nodeCount << endl;
    output << (end.tv_sec - start.tv_sec) +
                  (end.tv_usec - start.tv_usec) / 1000000.0
           << endl;
    output << path[currNode] << endl;
    output << (path[currNode]).length() << endl;
    output.close();
    return 0;
}