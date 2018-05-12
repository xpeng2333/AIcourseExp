#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

typedef struct nodeInfo {
    unsigned short f;
    unsigned short h;
    char direc = '#';
} node;

/*
struct f_str {
    friend bool operator<(f_str n1, f_str n2) {
        if (n1.f > n2.f)
            return true;
        else if (n1.f == n2.f && n1.h > n2.h)
            return true;
        else
            return false;
    }
    unsigned short f;
    unsigned short h;
    string strNode;
};
*/
struct f_h {
    friend bool operator<(f_h n1, f_h n2) {
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

map<string, node> nodeTable;
multimap<f_h, string> f_strNode;
// priority_queue<f_str> f_strNode;
short srcMap[5][5], tgtMap[5][5];

short zero_i, zero_j;
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
void expanding() {
    short tmp_zero_i, tmp_zero_j;
    string tmpNode;
    node tmp;
    unsigned short h = h1(currNode);
    unsigned short g = nodeTable[currNode].f - h + 1;
    char currDirec = nodeTable[currNode].direc;
    f_strNode.erase(f_strNode.begin());
    // f_strNode.pop();
    for (short i = 0; i < 25; i++) {
        if (currNode[i] == '9') {
            zero_i = i / 5;
            zero_j = i % 5;
            break;
        }
    }
    short hinderOffset;
    // right
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    tmpNode = currNode;
    hinderOffset = 0;
    if (currDirec != 'L') {
        while (tmp_zero_j < 4) {
            if (tmp_zero_j + hinderOffset >= 4)
                break;
            if (tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1] ==
                '!') {
                hinderOffset++;
                // tmpg--;
                continue;
            }
            tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1];
            tmpNode[5 * tmp_zero_i + tmp_zero_j + hinderOffset + 1] = '9';
            tmp.h = h1(tmpNode);
            tmp.f = g + tmp.h;
            tmp.direc = 'R';
            if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
                break;
            f_strNode.insert(make_pair(f_h{tmp.f, tmp.h}, tmpNode));
            // f_strNode.push({tmp.f, tmp.h, tmpNode});
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
            tmp.h = h1(tmpNode);
            tmp.f = g + tmp.h;
            tmp.direc = 'D';
            if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
                break;
            f_strNode.insert(make_pair(f_h{tmp.f, tmp.h}, tmpNode));
            // f_strNode.push({tmp.f, tmp.h, tmpNode});
            break;
        }
    }
    // left
    tmp_zero_i = zero_i;
    tmp_zero_j = zero_j;
    hinderOffset = 0;
    tmpNode = currNode;
    if (currDirec != 'R') {
        while (tmp_zero_j > 0) {
            if (tmp_zero_j - hinderOffset <= 0)
                break;
            if (tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1] ==
                '!') {
                hinderOffset++;
                // tmpg--;
                continue;
            }

            tmpNode[5 * tmp_zero_i + tmp_zero_j] =
                tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1];
            tmpNode[5 * tmp_zero_i + tmp_zero_j - hinderOffset - 1] = '9';
            tmp.h = h1(tmpNode);
            tmp.f = g + tmp.h;
            tmp.direc = 'L';
            if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
                break;
            f_strNode.insert(make_pair(f_h{tmp.f, tmp.h}, tmpNode));
            // f_strNode.push({tmp.f, tmp.h, tmpNode});
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

            tmp.h = h1(tmpNode);
            tmp.f = g + tmp.h;
            tmp.direc = 'U';
            if (nodeTable.insert(make_pair(tmpNode, tmp)).second == false)
                break;
            f_strNode.insert(make_pair(f_h{tmp.f, tmp.h}, tmpNode));
            // f_strNode.push({tmp.f, tmp.h, tmpNode});
            break;
        }
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
    short src[5][5], tgt[5][5];
    int zero_i, zero_j;
    ifstream path("output_Ah1.txt");
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
    string strSource;
    for (int i = 0; i < strPath.size(); i++) {
        short2str(src, strSource);
        // cout << h1(strSource) << endl;
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
    node nodeSrc;
    string src2tgt;
    reader(srcMap, "input.txt");
    reader(tgtMap, "target.txt");
    short2str(srcMap, srcNode);
    short2str(tgtMap, tgtNode);
    nodeSrc.f = h1(srcNode);
    cout << nodeSrc.f << endl;
    clock_t start, finish;
    double totaltime;
    start = clock();
    // struct timeval start;
    // gettimeofday(&start, NULL);
    nodeTable.insert(make_pair(srcNode, nodeSrc));
    // f_strNode.insert(make_pair(h1(srcNode), srcNode));
    f_strNode.insert(make_pair(f_h{h1(srcNode), h1(srcNode)}, srcNode));
    // f_strNode.push({h1(srcNode), h1(srcNode), srcNode});
    currNode = srcNode;
    while (currNode != tgtNode) {
        expanding();
        currNode = f_strNode.begin()->second;
        // currNode = f_strNode.top().strNode;
    }
    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    for (short i = 0; i < 25; i++) {
        if (currNode[i] == '9') {
            zero_i = i / 5;
            zero_j = i % 5;
            break;
        }
    }
    // struct timeval end;
    // gettimeofday(&end, NULL);
    ofstream output("output_Ah1.txt", ios::out);
    cout << totaltime << endl;
    // cout << (end.tv_sec - start.tv_sec) +
    //            (end.tv_usec - start.tv_usec) / 1000000.0
    //     << endl;
    cout << path(currNode, src2tgt) << endl;
    cout << src2tgt.length() << endl;
    cout << nodeTable.size() << endl;
    output << totaltime << endl;
    // output << (end.tv_sec - start.tv_sec) +
    //              (end.tv_usec - start.tv_usec) / 1000000.0
    //       << endl;
    output << src2tgt << endl;
    output << src2tgt.length() << endl;
    output.close();
    move4test();
    return 0;
}
