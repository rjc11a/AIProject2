//
//  sorting.hpp
//  Algorithms
//
//  Created by Joseph on 12/2/16.
//  Copyright © 2016 JosephC. All rights reserved.
//

#ifndef sorting_h
#define sorting_h

bool operator<(string a, string b)
{
    if(a == b)
        return false;
    int adex=0, bdex=0;
    {
        while(a[adex] !=0 && b[bdex] != 0)
        {
            if(a[adex] < b[bdex])
                return true;
            if(b[bdex] > a[adex])
                return false;
            adex++;
            bdex++;
        }
        if(adex == 0)
            return true;
        return false;
    }
    return false;
}
bool operator>(string a, string b)
{
    return (b<a);
}
bool operator<=(string a, string b) // a <= b
{
    return !(a > b);
}

bool operator>=(string a, string b)
{
    return !(a<b);
}
//bubble

template <typename T>
void BubbleSort(T A[], int len)
{
    len--;//start with 1 less, because array checks i+1 when i = len
    int walker;
    while(len)
    {
        walker = 0;
        for(int i=0; i<len; i++)//len is the range needing to be checked again
        {
            if(A[i]>A[i+1])
            {
                swap(A[i], A[i + 1]);
                walker = i+1; //walker is the location of the last swapped item
            }
        }
        len = walker;
    }
    //cout<<endl;
    //for(int i=0; i<50000; i++)
    //    cout<<A[i]<<endl;
}

//insertion

template <typename T>
void InsertionSort(T A[], int len)
{
    for(int i=1; i<len; i++)
        for(int j = i; j>0 && (A[j] < A[j-1]); j--)
            swap(A[j], A[j-1]);
}

//mergesorts

/*if(k<rn && ((j>=ln) || r[k] <= l[j]))
 {
 body[i++] = r[k++];
 }
 else
 {
 body[i++] = l[j++];
 }*/
void Merge(int*l, int*r, int ln, int rn)
{
    //j is for left array, k for right array
    int i=0, j=0, k=0, m = ln+rn;
    int* body = new int[ln+rn];
    while(i<m)
    {
        body[i++] = (k<rn && ((j>=ln) || r[k] <= l[j]) ? r[k++] : l[j++]);
    }
    for(int o=0; o<m; o++)
    {
        l[o] = body[o];
    }
    delete [] body;
}
void MergeSort(int* a, int len)
{
    if(len <= 1)
        return;
    
    int mid = len/2;
    int* r = a + mid;
    int rn = len-mid;
    
    MergeSort(a,mid);
    MergeSort(r,rn);
    
    Merge(a,r,mid,rn);
}

void SuperMergeSort(int* a, int len)
{
    if(len <= 20)
    {
        InsertionSort(a,len);
        return;
    }
    int mid = len/2;
    int* r = a + mid;
    int rn = len-mid;
    
    SuperMergeSort(a,mid);
    SuperMergeSort(r,rn);
    
    Merge(a,r,mid,rn);
}

//quick *********************************

void QuickSort(int*a, int len)
{
    if(len <= 1)
        return;
    /*
     //in-place
     int* less = a, rb = len-1, ln = 0;
     int* greater = a+rb;
     int p = a[len/2];
     while(ln<rb)
     {
     if(a[ln] < p)
     {
     ln++;
     }
     else
     {
     swap(a[ln],a[rb]);
     rb--;
     }
     }
     */
    //try in-place later, possibly faster
    int p = a[len/2];
    int* less = new int[len], ln = 0;
    int* more = new int[len], mn = 0;
    int* same = new int[len], sn = 0;
    for(int i=0; i<len; i++)
    {
        if(a[i] < p)
            less[ln++] = a[i];
        else if(a[i] > p)
            more[mn++] = a[i];
        else
            same[sn++] = a[i];
    }
    QuickSort(less, ln);
    QuickSort(more, mn);
    int ac = 0;
    for(int i=0; i<ln; i++)
    {
        a[ac++] = less[i];
    }
    for(int i=0; i<sn; i++)
    {
        a[ac++] = same[i];
    }
    for(int i=0; i<mn; i++)
    {
        a[ac++] = more[i];
    }
    
    delete [] less;
    delete [] more;
    delete [] same;
    
}


//radix *********************************
void radixify(int* a, int len, int bottom)
{
    int bucket[10] = {0};
    int* store = new int[len];
    
    for(int i=0; i<len; i++)
        bucket[(a[i]/bottom) % 10]++;//cut off bottom digit, then mod 10 to place into bucket
    
    for(int i=1; i<10; i++)//shift digits to mark displacement from previous
    {
        bucket[i] += bucket[i-1];
    }
    
    //for(int i=0; i<len; i++)
    for(int i = len-1; i>=0; i--)
    {
        store[--bucket[(a[i]/bottom) % 10] ] = a[i];
        //store[bucket[(a[i]/bottom) % 10] - 1] = a[i];
    }
    //  fill array, pass pointer by reference instead?
    // can't, can't deallocate non-dynamic memory
    for(int i=0; i<len; i++)
        a[i] = store[i];
    delete [] store;
}
void RadixSort(int* a, int len)
{
    int max = a[0];
    for(int i=1; i<len; i++)
    {
        if(a[i]>max)
            max = a[i];
    }
    for(int dec=1; max/dec > 0; dec*=10)
    {
        radixify(a, len, dec);
        //       cout<<"a[] = {"<<a[0];
        //       for(int i=1; i<len; i++)
        //           cout<<", "<<a[i];
        //       cout<<"}\n";
    }
}

//selection

template <typename T>
void SelectionSort(T A[], int len)
{
    int min;
    for(int i=0; i<len-1; i++)
    {
        min = i;
        for(int j = i+1; j<len; j++)
        {
            if(A[j] < A[min])
                min = j;
        }
        swap(A[min], A[i]);
    }
}

//shell
void ShellSort(int* a, int len)
{
    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
    int gap,cur;
    for(int g=0; g<9; g++)
    {
        gap = gaps[g];
        for(int o=gap; o<len; o++)
        {
            //cout<<"o = "<<o<<endl;
            cur = a[o];
            //cout<<"made it, gap = "<<gap<<endl;
            int i = o;
            while(i>= gap && a[i-gap] > cur)//mini insertion sort
            {
                a[i] = a[i-gap];
                i = i-gap;
            }
            //cout<<a[i]<<cur<<endl;
            a[i] = cur;
            
        }
    }
}

void ShellSort4ByName(std::string* a, int* b, int* c, double* d, int len)
{
    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
    int gap,curb,curc;
    std::string cura;
    double curd;
    for(int g=0; g<9; g++)
    {
        gap = gaps[g];
        for(int o=gap; o<len; o++)
        {
            cura = a[o];
            curb = b[o];
            curc = c[o];
            curd = d[o];
            //cout<<"made it, gap = "<<gap<<endl;
            int i = o;
            while(i>= gap && a[i-gap] > cura)//mini insertion sort
            {
                a[i] = a[i-gap];
                b[i] = b[i-gap];
                c[i] = c[i-gap];
                d[i] = d[i-gap];
                
                i = i-gap;
            }
            //cout<<a[i]<<cur<<endl;
            a[i] = cura;
            b[i] = curb;
            c[i] = curc;
            d[i] = curd;
            
        }
    }
}
void ShellSort4ByFirstInt(std::string* a, int* b, int* c, double* d, int len)
{
    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
    int gap,curb,curc;
    std::string cura;
    double curd;
    for(int g=0; g<9; g++)
    {
        gap = gaps[g];
        for(int o=gap; o<len; o++)
        {
            cura = a[o];
            curb = b[o];
            curc = c[o];
            curd = d[o];
            //cout<<"made it, gap = "<<gap<<endl;
            int i = o;
            while(i>= gap && b[i-gap] > curb)//mini insertion sort
            {
                a[i] = a[i-gap];
                b[i] = b[i-gap];
                c[i] = c[i-gap];
                d[i] = d[i-gap];
                
                i = i-gap;
            }
            //cout<<a[i]<<cur<<endl;
            a[i] = cura;
            b[i] = curb;
            c[i] = curc;
            d[i] = curd;
            
        }
    }
}
void ShellSort4ByDoubleDescending(std::string* a, int* b, int* c, double* d, int len)
{
    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
    int gap,curb,curc;
    std::string cura;
    double curd;
    for(int g=0; g<9; g++)
    {
        gap = gaps[g];
        for(int o=gap; o<len; o++)
        {
            cura = a[o];
            curb = b[o];
            curc = c[o];
            curd = d[o];
            //cout<<"made it, gap = "<<gap<<endl;
            int i = o;
            while(i>= gap && d[i-gap] < curd)//mini insertion sort
            {
                a[i] = a[i-gap];
                b[i] = b[i-gap];
                c[i] = c[i-gap];
                d[i] = d[i-gap];
                
                i = i-gap;
            }
            //cout<<a[i]<<cur<<endl;
            a[i] = cura;
            b[i] = curb;
            c[i] = curc;
            d[i] = curd;
            
        }
    }
}
void ShellSort4ByDoubleAscending(std::string* a, int* b, int* c, double* d, int len)
{
    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};
    int gap,curb,curc;
    std::string cura;
    double curd;
    for(int g=0; g<9; g++)
    {
        gap = gaps[g];
        for(int o=gap; o<len; o++)
        {
            cura = a[o];
            curb = b[o];
            curc = c[o];
            curd = d[o];
            //cout<<"made it, gap = "<<gap<<endl;
            int i = o;
            while(i>= gap && d[i-gap] > curd)//mini insertion sort
            {
                a[i] = a[i-gap];
                b[i] = b[i-gap];
                c[i] = c[i-gap];
                d[i] = d[i-gap];
                
                i = i-gap;
            }
            //cout<<a[i]<<cur<<endl;
            a[i] = cura;
            b[i] = curb;
            c[i] = curc;
            d[i] = curd;
            
        }
    }
}

#endif /* sorting_h */
