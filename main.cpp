#include <iostream>
#include <FibHeap.h>
#include <vector>

using std::cout;
using std::cin;

int main() {
    FibHeap<int> hp;
    Array<FibHeap<int>::Pointer> a;
    for (int i = 0; i < 10; ++i) {
        a.push(hp.insert(i));
    }
    *a[0] = 100;
    *a[-1] = -1;
    *a[5] = 200;
    for (int i = 0; i < 11; ++i) {
        try {
            cout << hp.extractMin() << "\n";
        } catch (std::exception &e) {
            cout << e.what() << "\n";
        }
    }
    return 0;
}