//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<RandomAccessIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter> && CopyConstructible<Compare>
//   void
//   pop_heap(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
#include <memory>

struct indirect_less
{
    template <class P>
    bool operator()(const P& x, const P& y)
        {return *x < *y;}
};

#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES

void test(unsigned N)
{
    int* ia = new int [N];
    for (int i = 0; i < N; ++i)
        ia[i] = i;
    std::random_shuffle(ia, ia+N);
    std::make_heap(ia, ia+N, std::greater<int>());
    for (int i = N; i > 0; --i)
    {
        std::pop_heap(ia, ia+i, std::greater<int>());
        assert(std::is_heap(ia, ia+i-1, std::greater<int>()));
    }
    std::pop_heap(ia, ia, std::greater<int>());
    delete [] ia;
}

int main()
{
    test(1000);

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    {
    const int N = 1000;
    std::unique_ptr<int>* ia = new std::unique_ptr<int> [N];
    for (int i = 0; i < N; ++i)
        ia[i].reset(new int(i));
    std::random_shuffle(ia, ia+N);
    std::make_heap(ia, ia+N, indirect_less());
    for (int i = N; i > 0; --i)
    {
        std::pop_heap(ia, ia+i, indirect_less());
        assert(std::is_heap(ia, ia+i-1, indirect_less()));
    }
    delete [] ia;
    }
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}
