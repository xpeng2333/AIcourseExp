#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>

using namespace std;

typedef struct nodeInfo {
    unsigned short f;
    //    unsigned short h;
    char direc = '#';
} node;

map<string, node> nodeTable;
// multimap<unsigned short, string> f_strNode;
short srcMap[5][5], tgtMap[5][5];

const unsigned short f_limit = 60;
short zero_i, zero_j;
string srcNode, tgtNode;
string currNode;
char currDirec;
node nodeSrc;

multimap<unsigned short, string, greater<unsigned short>> tmpTable;
multimap<unsigned short, string>::iterator iter;

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
            continue;
        }
        for (short j = 0; j < 25; j++) {
            if (nNode[i] == tgtNode[j]) {
                imod5 = i % 5;
                jmod5 = j % 5;
                idiv5 = i / 5;
                jdiv5 = j / 5;
                count += abs(imod5 - jmod5) + abs(idiv5 - jdiv5);
                break;
            }
        }
    }
    return count;
}
void expanding() {
    unsigned short d_limit = h2(srcNode);
    unsigned short next_d_limit;
    short tmp_zero_i, tmp_zero_j;
    string tmpNode;
    node tmp;
    // while (d_limit < f_limit) {
    while (1) {
        next_d_limit = INT16_MAX;
        nodeTable.clear();
        stack<string> waiting_node;
        // f_strNode.clear();
        // waiting_node.
        nodeTable.insert(make_pair(srcNode, nodeSrc));
        // f_strNode.insert(make_pair(h2(srcNode), srcNode));
        waiting_node.push(srcNode);
        // while (!f_strNode.empty()) {

        while (!waiting_node.empty()) {
            // currNode = f_strNode.begin()->second;
            // f_strNode.erase(f_strNode.begin());
            currNode = waiting_node.top();
            waiting_node.pop();
            if (nodeTable[currNode].f > d_limit) {
                next_d_limit = min(next_d_limit, nodeTable[currNode].f);
            } else {
                if (currNode == tgtNode)
                    return;
                unsigned short g = nodeTable[currNode].f - h2(currNode) + 1;
                for (short i = 0; i < 25; i++) {
                    if (currNode[i] == '9') {
                        zero_i = i / 5;
                        zero_j = i % 5;
                        break;
                    }
                }
                tmpTable.clear();
                short hinderOffset;
                // left
                tmp_zero_i = zero_i;
                tmp_zero_j = zero_j;
                hinderOffset = 0;
                tmpNode = currNode;
                if (currDirec != 'R') {
                    while (tmp_zero_j > 0) {
                        if (tmp_zero_j - hinderOffset <= 0)
                            break;
                        if (tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset -
                                    1] == '!') {
                            hinderOffset++;
                            continue;
                        }

                        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                            tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset -
                                    1];
                        tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset -
                                1] = '9';
                        tmp.f = g + h2(tmpNode);
                        tmp.direc = 'L';
                        if (nodeTable.insert(make_pair(tmpNode, tmp)).second ==
                            false)
                            break;
                        tmpTable.insert(make_pair(tmp.f, tmpNode));
                        // f_strNode.insert(make_pair(tmp.f, tmpNode));
                        break;
                    }
                }
                // right
                tmp_zero_i = zero_i;
                tmp_zero_j = zero_j;
                tmpNode = currNode;
                hinderOffset = 0;
                if (currDirec != 'L') {
                    while (tmp_zero_j < 4) {
                        if (tmp_zero_j + hinderOffset >= 4)
                            break;
                        if (tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset +
                                    1] == '!') {
                            hinderOffset++;
                            continue;
                        }
                        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                            tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset +
                                    1];
                        tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset +
                                1] = '9';
                        tmp.f = g + h2(tmpNode);
                        tmp.direc = 'R';
                        if (nodeTable.insert(make_pair(tmpNode, tmp)).second ==
                            false)
                            break;
                        // f_strNode.insert(make_pair(tmp.f, tmpNode));
                        tmpTable.insert(make_pair(tmp.f, tmpNode));
                        break;
                    }
                }
                // up
                tmp_zero_i = zero_i;
                tmp_zero_j = zero_j;
                tmpNode = currNode;
                hinderOffset = 0;
                if (currDirec != 'D') {
                    while (tmp_zero_i > 0) {
                        if (tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j] == '!') {
                            break;
                        }
                        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                            tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j];
                        tmpNode[5 * (tmp_zero_i - 1) + tmp_zero_j] = '9';

                        tmp.f = g + h2(tmpNode);
                        tmp.direc = 'U';
                        if (nodeTable.insert(make_pair(tmpNode, tmp)).second ==
                            false)
                            break;
                        // f_strNode.insert(make_pair(tmp.f, tmpNode));
                        tmpTable.insert(make_pair(tmp.f, tmpNode));
                        break;
                    }
                }
                // down
                tmp_zero_i = zero_i;
                tmp_zero_j = zero_j;
                tmpNode = currNode;
                hinderOffset = 0;
                if (currDirec != 'U') {
                    while (tmp_zero_i < 4) {
                        if (tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j] == '!') {
                            break;
                        }
                        tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                            tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j];
                        tmpNode[5 * (tmp_zero_i + 1) + tmp_zero_j] = '9';
                        tmp.f = g + h2(tmpNode);
                        tmp.direc = 'D';
                        if (nodeTable.insert(make_pair(tmpNode, tmp)).second ==
                            false)
                            break;
                        // f_strNode.insert(make_pair(tmp.f, tmpNode));
                        tmpTable.insert(make_pair(tmp.f, tmpNode));
                        break;
                    }
                }
                for (iter = tmpTable.begin(); iter != tmpTable.end(); iter++) {
                    waiting_node.push(iter->second);
                }
            }
        }
        d_limit = next_d_limit;
    }
}

string path(string nodeN, string &currPath) {
    // cout << zero_i << " " << zero_j << endl;
    char tmpDirec = nodeTable[nodeN].direc;
    if (tmpDirec == '#')
        return currPath;
    switch (tmpDirec) {
    case 'L': {
        if (nodeN[zero_i * 5 + zero_j + 1] == '!') {
            nodeN[zero_i * 5 + zero_j] = nodeN[zero_i * 5 + zero_j + 2];
            nodeN[zero_i * 5 + zero_j + 2] = '9';
            zero_j += 2;
            currPath = 'L' + currPath;
            return path(nodeN, currPath);
        } else {
            nodeN[zero_i * 5 + zero_j] = nodeN[zero_i * 5 + zero_j + 1];
            nodeN[zero_i * 5 + zero_j + 1] = '9';
            zero_j += 1;
            currPath = 'L' + currPath;
            return path(nodeN, currPath);
        }
        break;
    }
    case 'R': {
        if (nodeN[zero_i * 5 + zero_j - 1] == '!') {
            nodeN[zero_i * 5 + zero_j] = nodeN[zero_i * 5 + zero_j - 2];
            nodeN[zero_i * 5 + zero_j - 2] = '9';
            zero_j -= 2;
            currPath = 'R' + currPath;
            return path(nodeN, currPath);
        } else {
            nodeN[zero_i * 5 + zero_j] = nodeN[zero_i * 5 + zero_j - 1];
            nodeN[zero_i * 5 + zero_j - 1] = '9';
            zero_j -= 1;
            currPath = 'R' + currPath;
            return path(nodeN, currPath);
        }
        break;
    }
    case 'U': {
        nodeN[zero_i * 5 + zero_j] = nodeN[(zero_i + 1) * 5 + zero_j];
        nodeN[(zero_i + 1) * 5 + zero_j] = '9';
        zero_i += 1;
        currPath = 'U' + currPath;
        return path(nodeN, currPath);
        break;
    }
    case 'D': {
        nodeN[zero_i * 5 + zero_j] = nodeN[(zero_i - 1) * 5 + zero_j];
        nodeN[(zero_i - 1) * 5 + zero_j] = '9';
        zero_i -= 1;
        currPath = 'D' + currPath;
        return path(nodeN, currPath);
        break;
    }
    default:
        break;
    }
}

void move4test() {
    int src[5][5], tgt[5][5];
    int zero_i, zero_j;
    ifstream path("output_IDAh2.txt");
    ifstream source("input.txt");
    ifstream target("target.txt");
    for (short i = 0; i < 5; i++) {
        for (short j = 0; j < 5; j++) {
            source >> src[i][j];
            target >> tgt[i][j];
            if (src[i][j] == 0) {
                zero_i = i;
                zero_j = j;
            }
        }
    }
    source.close();
    target.close();
    string strPath;
    path >> strPath;
    path >> strPath;
    cout << strPath << endl;
    path.close();
    for (int i = 0; i < strPath.size(); i++) {
        switch (strPath[i]) {
        case 'L': {
            if (src[zero_i][zero_j - 1] == -1) {
                src[zero_i][zero_j] = src[zero_i][zero_j - 2];
                src[zero_i][zero_j - 2] = 0;
                zero_j -= 2;
            } else {
                src[zero_i][zero_j] = src[zero_i][zero_j - 1];
                src[zero_i][zero_j - 1] = 0;
                zero_j--;
            }
            break;
        }
        case 'R': {
            if (src[zero_i][zero_j + 1] == -1) {
                src[zero_i][zero_j] = src[zero_i][zero_j + 2];
                src[zero_i][zero_j + 2] = 0;
                zero_j += 2;
            } else {
                src[zero_i][zero_j] = src[zero_i][zero_j + 1];
                src[zero_i][zero_j + 1] = 0;
                zero_j++;
            }
            break;
        }
        case 'U': {
            src[zero_i][zero_j] = src[zero_i - 1][zero_j];
            src[zero_i - 1][zero_j] = 0;
            zero_i--;
            break;
        }
        case 'D': {
            src[zero_i][zero_j] = src[zero_i + 1][zero_j];
            src[zero_i + 1][zero_j] = 0;
            zero_i++;
            break;
        }
        default:
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (src[i][j] != tgt[i][j]) {
                cout << "error" << endl;
                exit(1);
            }
        }
    }
    cout << "true" << endl;
}
int main() {
    string src2tgt;
    reader(srcMap, "input.txt");
    reader(tgtMap, "target.txt");
    short2str(srcMap, srcNode);
    short2str(tgtMap, tgtNode);
    nodeSrc.f = h2(srcNode);
    clock_t start, finish;
    double totaltime;
    start = clock();
    expanding();
    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    for (short i = 0; i < 25; i++) {
        if (currNode[i] == '9') {
            zero_i = i / 5;
            zero_j = i % 5;
            break;
        }
    }
    ofstream output("output_IDAh2.txt", ios::out);
    cout << totaltime << endl;
    cout << path(currNode, src2tgt) << endl;
    cout << src2tgt.length() << endl;
    output << totaltime << endl;
    output << src2tgt << endl;
    output << src2tgt.length() << endl;
    output.close();
    move4test();
    return 0;
}
