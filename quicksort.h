#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "phonebook.h"
#include <vector>

template <class T>
class Qsort
{
public:
    // Constructors
    Qsort()
    {
        ascending = true;
    }

    // Constructor accepting data
    Qsort(std::vector<T> a)
    {
        m_A = a;   // copy data to m_A; for vectors, '=' does perform the copying of elements
        ascending = true;
    }

    // Destructor
    ~Qsort()
    {
        if (m_A.size() > 0)
        {
            m_A.clear();
        }
    }

    // Get copy of m_A
    void GetA(std::vector<T> &a)
    {
        a = m_A;
    }

    // Set new value of m_A
    void SetA(const std::vector<T> &a)
    {
        m_A = a;
    }

    // Sort m_A
    void Sort(bool asc)
    {
        if (m_A.size() <= 0)
        {
            return;
        }
        ascending = asc;
        sort(0, m_A.size() - 1);
    }

private:
    std::vector<T> m_A;       // data array to sort
    bool ascending;

    // Swap 2 elements
    void swap(T &a, T &b)
    {
        T t = a;
        a = b;
        b = t;
    }

    // Partition m_A within indicies from l to h
    int partition(int l, int h)
    {
        int i = l - 1;

        for (int j = l; j <= h - 1; j++)
        {
            bool cmp = true;
            if (ascending)
            {
                cmp = (m_A[j] < m_A[h]);
            }
            else
            {
                cmp = (m_A[h] < m_A[j]);
            }
            if (cmp)
            {
                i++;
                swap(m_A[i], m_A[j]);
            }
        }
        swap(m_A[i + 1], m_A[h]);

        return (i + 1);
    }

    // Recursive sort of m_A from index l to index h
    void sort(int l, int h)
    {
        if (l < h)
        {
            int pi = partition(l, h);
            sort(l, pi - 1);
            sort(pi + 1, h);
        }
    }
};

#endif // QUICKSORT_H
