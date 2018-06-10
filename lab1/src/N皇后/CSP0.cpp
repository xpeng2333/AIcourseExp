#include <climits>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <vector>

using namespace std;

typedef struct x_y {
    int x = -1;
    int y = -1;
    bool operator<(const x_y &b) const {
        if (this->x < b.x)
            return true;
        else if (this->x == b.x)
            return (this->y <= b.y);
        return false;
    }
} x_y;

list<set<x_y>> constraintSet_x;
list<set<x_y>> constraintSet_y;
list<set<x_y>> constraintSet_diag;
list<set<x_y>> constraintSet_revdiag;
list<set<x_y>>::iterator iter;

set<x_y> posTable;
set<x_y>::iterator iterset;
vector<x_y> posTable_cp;
vector<x_y>::iterator iterVec;
unsigned int m, n;

int isConflict(x_y pos) {
    int count = 0;
    int tmp;
    for (iter = constraintSet_x.begin(); iter != constraintSet_x.end();
         iter++) {
        if (pos.x == (*iter).begin()->x) {
            tmp = (*iter).size();
            count += tmp * (tmp + 1) / 2;
            break;
        }
    }

    for (iter = constraintSet_y.begin(); iter != constraintSet_y.end();
         iter++) {
        if (pos.y == (*iter).begin()->y) {
            tmp = (*iter).size();
            count += tmp * (tmp + 1) / 2;
            break;
        }
    }
    for (iter = constraintSet_diag.begin(); iter != constraintSet_diag.end();
         iter++) {
        if ((pos.x - (*iter).begin()->x) == (pos.y - (*iter).begin()->y)) {
            tmp = (*iter).size();
            count += tmp * (tmp + 1) / 2;
            break;
        }
    }
    for (iter = constraintSet_revdiag.begin();
         iter != constraintSet_revdiag.end(); iter++) {
        if ((pos.x - (*iter).begin()->x) == ((*iter).begin()->y - pos.y)) {
            tmp = (*iter).size();
            count += tmp * (tmp + 1) / 2;
            break;
        }
    }
    return count;
}

int totalConflict() {
    int count = 0;
    int tmp;
    for (iter = constraintSet_x.begin(); iter != constraintSet_x.end();
         iter++) {
        tmp = (*iter).size();
        count += tmp * (tmp - 1) / 2;
    }
    for (iter = constraintSet_y.begin(); iter != constraintSet_y.end();
         iter++) {
        tmp = (*iter).size();
        count += tmp * (tmp - 1) / 2;
    }
    for (iter = constraintSet_diag.begin(); iter != constraintSet_diag.end();
         iter++) {
        tmp = (*iter).size();
        count += tmp * (tmp - 1) / 2;
    }
    for (iter = constraintSet_revdiag.begin();
         iter != constraintSet_revdiag.end(); iter++) {
        tmp = (*iter).size();
        count += tmp * (tmp - 1) / 2;
    }
    return count;
}
void removePos(x_y pos) {
    for (iter = constraintSet_x.begin(); iter != constraintSet_x.end();
         iter++) {
        if (pos.x == (*iter).begin()->x) {
            (*iter).erase(pos);
            if ((*iter).empty())
                constraintSet_x.erase(iter);
            break;
        }
    }
    for (iter = constraintSet_y.begin(); iter != constraintSet_y.end();
         iter++) {
        if (pos.y == (*iter).begin()->y) {
            (*iter).erase(pos);
            if ((*iter).empty())
                constraintSet_y.erase(iter);
            break;
        }
    }
    for (iter = constraintSet_diag.begin(); iter != constraintSet_diag.end();
         iter++) {
        if ((pos.x - (*iter).begin()->x) == (pos.y - (*iter).begin()->y)) {
            (*iter).erase(pos);
            if ((*iter).empty())
                constraintSet_diag.erase(iter);
            break;
        }
    }
    for (iter = constraintSet_revdiag.begin();
         iter != constraintSet_revdiag.end(); iter++) {
        if ((pos.x - (*iter).begin()->x) == ((*iter).begin()->y - pos.y)) {
            (*iter).erase(pos);
            if ((*iter).empty())
                constraintSet_revdiag.erase(iter);
            break;
        }
    }
}
void insertPos(x_y pos) {
    set<x_y> tmpset;
    posTable.insert(pos);
    posTable_cp.push_back(pos);
    for (iter = constraintSet_x.begin(); iter != constraintSet_x.end();
         iter++) {
        if (pos.x == (*iter).begin()->x) {
            //(*iter).push_back({i, j});
            (*iter).insert(pos);
            break;
        }
    }
    if (iter == constraintSet_x.end()) {
        // tmplist.clear();
        tmpset.clear();
        // tmplist.push_back({i, j});
        tmpset.insert(pos);
        // constraintSet_x.push_back(tmplist);
        constraintSet_x.push_back(tmpset);
    }
    for (iter = constraintSet_y.begin(); iter != constraintSet_y.end();
         iter++) {
        if (pos.y == (*iter).begin()->y) {
            //(*iter).push_back({i, j});
            (*iter).insert(pos);
            break;
        }
    }
    if (iter == constraintSet_y.end()) {
        // tmplist.clear();
        tmpset.clear();
        // tmplist.push_back({i, j});
        tmpset.insert(pos);
        // constraintSet_x.push_back(tmplist);
        constraintSet_y.push_back(tmpset);
    }
    for (iter = constraintSet_diag.begin(); iter != constraintSet_diag.end();
         iter++) {
        if ((pos.x - (*iter).begin()->x) == (pos.y - (*iter).begin()->y)) {
            //(*iter).push_back({i, j});
            (*iter).insert(pos);
            break;
        }
    }
    if (iter == constraintSet_diag.end()) {
        // tmplist.clear();
        tmpset.clear();
        // tmplist.push_back({i, j});
        tmpset.insert(pos);
        // constraintSet_x.push_back(tmplist);
        constraintSet_diag.push_back(tmpset);
    }
    for (iter = constraintSet_revdiag.begin();
         iter != constraintSet_revdiag.end(); iter++) {
        if ((pos.x - (*iter).begin()->x) == ((*iter).begin()->y - pos.y)) {
            //(*iter).push_back({i, j});
            (*iter).insert(pos);
            break;
        }
    }
    if (iter == constraintSet_revdiag.end()) {
        // tmplist.clear();
        tmpset.clear();
        // tmplist.push_back({i, j});
        tmpset.insert(pos);
        // constraintSet_x.push_back(tmplist);
        constraintSet_revdiag.push_back(tmpset);
    }
}
void init() {
    int tmp = 0;
    set<x_y> tmpset;
    int i, j;
    while (tmp < n) {
        i = tmp;
        j = rand() % n;
        if (posTable.insert({i, j}).second == false)
            continue;
        posTable_cp.push_back({i, j});
        for (iter = constraintSet_x.begin(); iter != constraintSet_x.end();
             iter++) {
            if (i == (*iter).begin()->x) {
                //(*iter).push_back({i, j});
                (*iter).insert({i, j});
                break;
            }
        }
        if (iter == constraintSet_x.end()) {
            // tmplist.clear();
            tmpset.clear();
            // tmplist.push_back({i, j});
            tmpset.insert({i, j});
            // constraintSet_x.push_back(tmplist);
            constraintSet_x.push_back(tmpset);
        }
        for (iter = constraintSet_y.begin(); iter != constraintSet_y.end();
             iter++) {
            if (j == (*iter).begin()->y) {
                //(*iter).push_back({i, j});
                (*iter).insert({i, j});
                break;
            }
        }
        if (iter == constraintSet_y.end()) {
            // tmplist.clear();
            tmpset.clear();
            // tmplist.push_back({i, j});
            tmpset.insert({i, j});
            // constraintSet_x.push_back(tmplist);
            constraintSet_y.push_back(tmpset);
        }
        for (iter = constraintSet_diag.begin();
             iter != constraintSet_diag.end(); iter++) {
            if ((i - (*iter).begin()->x) == (j - (*iter).begin()->y)) {
                //(*iter).push_back({i, j});
                (*iter).insert({i, j});
                break;
            }
        }
        if (iter == constraintSet_diag.end()) {
            // tmplist.clear();
            tmpset.clear();
            // tmplist.push_back({i, j});
            tmpset.insert({i, j});
            // constraintSet_x.push_back(tmplist);
            constraintSet_diag.push_back(tmpset);
        }
        for (iter = constraintSet_revdiag.begin();
             iter != constraintSet_revdiag.end(); iter++) {
            if ((i - (*iter).begin()->x) == ((*iter).begin()->y - j)) {
                //(*iter).push_back({i, j});
                (*iter).insert({i, j});
                break;
            }
        }
        if (iter == constraintSet_revdiag.end()) {
            // tmplist.clear();
            tmpset.clear();
            // tmplist.push_back({i, j});
            tmpset.insert({i, j});
            // constraintSet_x.push_back(tmplist);
            constraintSet_revdiag.push_back(tmpset);
        }
        tmp++;
    }
}

int main(int argc, char const *argv[]) {
    clock_t start, finish;
    double totaltime;
    int selectIndex;
    int mindis = INT_MAX;
    x_y minpos;
    x_y selectPos;

    ifstream input("input.txt");
    input >> n;
    input >> m;
    input.close();
    cout << n << " " << m << endl;

    start = clock();
    init();
    while (totalConflict() != m) {
        // cout << posTable.size() << endl;
        // cout << totalConflict() << endl;
        // srand((unsigned)time(NULL));
        selectIndex = rand() % n;
        // cout << selectIndex << endl;
        selectPos = posTable_cp[selectIndex];
        iterVec = posTable_cp.begin();
        posTable_cp.erase(iterVec + selectIndex);
        // posTable.insert(selectPos);
        cout << posTable.size() << " ";
        posTable.erase(selectPos);
        cout << posTable.size() << endl;
        removePos(selectPos);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (posTable.find({i, j}) == posTable.end()) {
                    if (std::abs(isConflict({i, j}) - m) <= mindis) {
                        mindis = std::abs(isConflict({i, j}) - m);
                        minpos = {i, j};
                    }
                }
            }
        }
        insertPos(minpos);
    }
    finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << totaltime << endl;
    ofstream output("output_csp.txt");
    output.close();
    return 0;
}
