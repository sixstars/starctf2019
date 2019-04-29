#include <iostream>
#include <cstdlib>
#include <cstdio>
#define ALEN 16
using namespace std;
class vec {
public:
    int *content;
    int size;
    vec(int size) : size(size) {
        content = (int*)malloc(sizeof(int)*size);
    }
    virtual ~vec() {
        free(content);
    }
    virtual void show() {
        cout << "No leakage! :P" << endl;
        return;
    }
};
vec* vec_list[ALEN] = {};
void add() {
    int size = 0;
    int idx = 0;
    int tmp = 0;
    vec* new_vec = 0;
    cout << "Index:";
    cin >> idx;
    if (0 <= idx && idx < ALEN && vec_list[idx] == NULL) {
        cout << "Size:";
        cin >> size;
        if (0 < size && size <= 0x10000) {
            cout << "Input " << size << " integers, -1 to stop:";
            new_vec = new vec(size);
            int i = 0;
            for (; i < size; ++i) {
                cin >> tmp;
                if (tmp == -1)
                    break;
                new_vec->content[i] = tmp;
            }
            vec_list[idx] = new_vec;
        } else {
            cout << "Invalid size!" << endl;
        }
    } else {
        cout << "Invalid vec index!" << endl;
    }
    return;
}
void remove() {
    int idx = 0;
    cout << "vec index:";
    cin >> idx;
    if (0 <= idx && idx < ALEN && vec_list[idx] != NULL) {
        delete vec_list[idx];
    } else {
        cout << "Invalid vec index!" << endl;
    }
    return;
}
void edit() {
    cout << "Not implemented yet." << endl;
    return;
}
void show() {
    int idx = 0;
    cout << "vec index:";
    cin >> idx;
    if (0 <= idx && idx < ALEN && vec_list[idx] != NULL) {
        vec_list[idx]->show();
    } else {
        cout << "Invalid vec index!" << endl;
    }
    return;
}
void welcome() {
    cout << " #   #    ####    #####  ######"   << endl;
    cout << "  # #    #    #     #    #"        << endl;
    cout << "### ###  #          #    #####"    << endl;
    cout << "  # #    #          #    #"        << endl;
    cout << " #   #   #    #     #    #"        << endl;
    cout << "          ####      #    #"        << endl;
    cout << "Welcome to *ctf2019" << endl;
    return;
}
int main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    int op = 0;
    welcome();
    while (true) {
        cout << "1. Add a vec" << endl;
        cout << "2. Remove a vec" << endl;
        cout << "3. Edit a vec" << endl;
        cout << "4. Show a vec" << endl;
        cout << "Your choice:";
        cin >> op;
        if (cin.eof()) return -1;
        switch (op) {
            case 1: add();break;
            case 2: remove();break;
            case 3: edit();break;
            case 4: show();break;
            default: goto bye;
        }
    }
bye:
    cout << "bye bye" << endl;
    return 0;
}
