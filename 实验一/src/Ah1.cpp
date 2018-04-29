#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sys/time.h>

using namespace std;

typedef struct nodeInfo {
    unsigned short f;
    string parent = "#";
    bool visited = false;
} node;

unsigned long nodeCount = 0;
map<string, node> nodeTable;
multimap<unsigned short, string> f_strNode;
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
    node tmp;
    nodeTable[currNode].visited = true;
    unsigned short g = nodeTable[currNode].f - h1(currNode) + 1;
    f_strNode.erase(f_strNode.begin());
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
        tmp.f = g + h1(tmpNode);
        tmp.parent = currNode;
        if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
            break;
        f_strNode.insert(make_pair(tmp.f, tmpNode));
        nodeCount++;
        break;
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
        tmp.f = g + h1(tmpNode);
        tmp.parent = currNode;
        if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
            break;
        f_strNode.insert(make_pair(tmp.f, tmpNode));
        nodeCount++;
        break;
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

        tmp.f = g + h1(tmpNode);
        tmp.parent = currNode;
        if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
            break;
        f_strNode.insert(make_pair(tmp.f, tmpNode));
        nodeCount++;
        break;
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
        tmp.f = g + h1(tmpNode);
        tmp.parent = currNode;
        if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
            break;
        f_strNode.insert(make_pair(tmp.f, tmpNode));
        nodeCount++;
        break;
    }
}

string path(string nodeN, string &currPath) {
    string tmp = nodeTable[nodeN].parent;
    if (tmp == "#")
        return currPath;
    int pos_zero, pos1, pos2;
    for (int i = 0; i < 25; i++) {
        if (tmp[i] != nodeN[i]) {
            pos1 = i;
            if (nodeN[i] == '"')
                pos_zero = i;
            break;
        }
    }
    for (int i = 24; i >= 0; i--) {
        if (tmp[i] != nodeN[i]) {
            pos2 = i;
            if (nodeN[i] == '"')
                pos_zero = i;
            break;
        }
    }
    if (pos2 - pos1 == 5) {
        if (pos1 == pos_zero) {
            currPath = "U" + currPath;
            return path(tmp, currPath);
        }
        currPath = "D" + currPath;
        return path(tmp, currPath);
    }
    if (pos2 - pos1 < 4) {
        if (pos1 == pos_zero) {
            currPath = "L" + currPath;
            return path(tmp, currPath);
        }

        currPath = "R" + currPath;
        return path(tmp, currPath);
    }
}
int main() {
    node nodeSrc;
    string src2tgt;
    reader(srcMap, "input.txt");
    reader(tgtMap, "target.txt");
    short2str(srcMap, srcNode);
    short2str(tgtMap, tgtNode);
    nodeSrc.f = h1(srcNode);
    struct timeval start;
    gettimeofday(&start, NULL);
    nodeTable.insert(make_pair(srcNode, nodeSrc));
    f_strNode.insert(make_pair(h1(srcNode), srcNode));
    currNode = srcNode;
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
    cout << path(currNode, src2tgt) << endl;
    cout << src2tgt.length() << endl;
    cout << nodeCount << endl;
    output << (end.tv_sec - start.tv_sec) +
                  (end.tv_usec - start.tv_usec) / 1000000.0
           << endl;
    output << src2tgt << endl;
    output << src2tgt.length() << endl;
    output.close();
    return 0;
}