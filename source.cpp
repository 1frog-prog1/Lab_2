#include <assert.h>
#include <iostream>
#include <deque>
#include "deque.h"

using namespace lol;

int main() {

    {
        Deque<int,Allocator<Node<int>>> a = Deque<int, Allocator<Node<int>>>();
        a.push_back(3);
        a.push_back(1);
        assert(3 == a[0]);
        assert(1 == a[1]);
        a.push_front(5);
        assert(5 == a[0]);
        assert(3 == a[1]);
        assert(1 == a[2]);
    }

    {
        std::deque<int> first = {1, 2, 3};
        Deque<int,Allocator<Node<int>>> deq(first.begin(), first.end());
        Deque_iterator<int> b = deq.begin();
        Deque_iterator<int> c = deq.end();
        Deque_reverse_iterator<Deque_iterator<int>> g = deq.rbegin();
        Deque_reverse_iterator<Deque_iterator<int>> e = deq.rend();
        assert(1 == *b++);
        assert(3 == *++b);
        assert(3 == *c);
        assert(b == c);
        assert(b >= c);
        assert(b <= c);
        assert(1 == *g--);
        assert(3 == *--g);
        assert(2 == *++g);
        assert(3 == *e);
    }

    {
        std::deque<int> v = {1, 2, 3, 5};
        std::initializer_list<int> ilist = {11, 12, 13, 15};
        Deque<int,Allocator<Node<int>>> d(v.begin(), v.end());
        Deque_const_iterator<int> it = d.cbegin();
        it++;
        auto m = d.insert(it, 20);
        auto n = d.insert(it, ilist);
        assert(d[1] == 20);
        assert(20 == *m);
        assert(15 == *n);
        assert(9 == d.size());
        assert(5 == d[8]);
    }

    {
        std::deque<int> v = {6, 2, 3, 6};
        Deque<int, Allocator<Node<int>>> a(v.begin(), v.end());

        Deque_const_iterator<int> e = a.cbegin();
        e++;
        a.erase(e);
        assert(3 == a.size());

        Deque_const_iterator<int> g = a.cbegin();
        a.erase(g);
        assert(2 == a.size());

        Deque_const_iterator<int> d = a.cend();
        a.erase(d);
        assert(1 == a.size());

        Deque_const_iterator<int> l = a.cend();
        a.erase(l);
        assert(0 == a.size());

        a.push_back(3);
        a.push_back(4);
        a.erase(a.cbegin(), a.cend());
        assert(0 == a.size());

        a.clear();
        a.push_back(5);
        erase(a, 5);
        assert(0 == a.size());

    }

    {
        std::deque<int> v1 = {1,2,3,5};
        std::deque<int> v2 = {5,3,2,1};
        Deque<int,Allocator<Node<int>>> a(v1.begin(), v1.end());
        Deque<int,Allocator<Node<int>>> b(v2.begin(), v2.end());

        a.swap(b);
        assert(5 == a[0]);
        assert(1 == b[0]);

        a.swap(b);
        assert(5 == b[0]);
        assert(1 == a[0]);

        Deque<int> d = {1, 2, 3, 5};
        Deque<int> d1 = {2};
        d.swap(d1);
        assert(d[0] == 2);
        assert(d1[0] == 1);
        swap(d, d1);
        assert(d[0] == 1);
        assert(d1[0] == 2);
    }

    {
        std::deque<int> v = {1,2,3};
        Deque<int,Allocator<Node<int>>> deq1(v.begin(), v.end());
        Deque<int,Allocator<Node<int>>> deq2(v.begin(), v.end());

        assert(deq1 == deq2);
        assert((deq1 != deq2) == false);
        assert(deq1 < deq2 == false);
        assert(deq2 < deq1 == false);
        assert((deq1 > deq2) == false);
        assert((deq2 > deq1) == false);
        assert((deq2 < deq1) == false);
        assert(deq1 >= deq2);
        assert(deq1 <= deq2);

        deq1.pop_back();
        deq2.push_back(2);
        assert(deq1 != deq2);
        assert((deq1 == deq2) == false);
        assert(deq1 < deq2);
        assert(deq1 <= deq2);
        assert((deq1 > deq2) == false);

        deq1.push_back(1);
        assert(deq1 != deq2);
        assert((deq1 == deq2) == false);
        assert(deq1 < deq2);
        assert(deq1 <= deq2);
        assert((deq1 > deq2) == false);
        assert((deq1 >= deq2) == false);
    }


    {
        std::deque<int> v = {1, 2, 3, 4};
        Deque<int,Allocator<Node<int>>> d1(v.begin(), v.end());

        assert(d1.size() == 4);
        d1.resize(1);
        assert(d1.size() == 1);
        d1.resize(100);
        assert(d1.size() == 100);

        Deque<int> d2 = {1, 2, 3, 5};

        assert(d2.size() == 4);
        d2.resize(1);
        assert(d2.size() == 1);
        d2.resize(1);
        assert(d2.size() == 1);
    }

    {
        Deque<int> d(6);
        assert(d.size() == 6);
        d.clear();
        assert(d.size() == 0);
        assert(d.empty());
    }

    std::cout << "1";

    return 0;
}