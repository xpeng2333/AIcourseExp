#include <fstream>
#include <iostream>
using namespace std;

int main() {
    int src[5][5], tgt[5][5];
    int zero_i, zero_j;
    ifstream path("output_Ah2.txt");
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
    return 0;
}