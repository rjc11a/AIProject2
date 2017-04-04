//
//  geneticheap.cpp
//  AIProj2
//
//  Created by Joseph on 3/6/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

/* This only works for a population size of 100-101 genetic algorithm */
//       srand(time(NULL))
//                oyvey = rand() % 20000000;

#ifndef geneticheap_h
#define geneticheap_h
#include <iostream>
#include <cmath>
#include <ctime>
struct geneticSack{
    int items;
    int capacity;
    string * names;
    int* costs;
    int* values;
    double* ratios;
};
void init_sack(geneticSack &s, int n, int c){
    
    s.items = n;
    s.capacity = c;
    s.names = new string[n];
    s.costs = new int[n];
    s.values = new int[n];
    s.ratios = new double[n];
}

//forest is a square 2d string structure with length 'dimensions' on each side,
// 's' is the starting spot in the string
// 'f' is a piece of food
// 'd' is a spot which kills the genome (death)
// 'x' represents an impassible wall, attempting to walk on a wall will prevent movement
// '.' represents an empty space
const int NUMSTRINGS = 20;
struct forest{
    int dimensions;
    int steps;
    int startstr;
    int startchar;
    string maze[NUMSTRINGS];
};

struct p1Node{
    
    string genome;
    int strength;
};
struct genNode{
    //genNode(){wallhit = 0; food = 0; dead=false;}
    string genome;
    int strength;
    int wallhit;
    int food;
    bool dead;
};

bool operator>(p1Node a, p1Node b)
{
    return a.strength > b.strength;
}
bool operator<(p1Node a, p1Node b)
{
    return a.strength < b.strength;
}
bool operator>=(p1Node a, p1Node b)
{
    return a.strength >= b.strength;
}
bool operator<=(p1Node a, p1Node b)
{
    return a.strength <= b.strength;
}

bool operator==(p1Node a, p1Node b)
{
    return a.genome == b.genome;
}
bool operator!=(p1Node a, p1Node b)
{
    return a.genome != b.genome;
}
//bool operator=


bool operator>(genNode a, genNode b)
{
    return a.strength > b.strength;
}
bool operator<(genNode a, genNode b)
{
    return a.strength < b.strength;
}
bool operator>=(genNode a, genNode b)
{
    return a.strength >= b.strength;
}
bool operator<=(genNode a, genNode b)
{
    return a.strength <= b.strength;
}

bool operator==(genNode a, genNode b)
{
    return a.genome == b.genome;
}
bool operator!=(genNode a, genNode b)
{
    return a.genome != b.genome;
}



const int HEAPSIZE = 10002;

template <typename T>
struct Heap
{
    T data[HEAPSIZE];  // use array, start indexing at 1
    int size; // current size of heap
};

template <typename T>
void myswap(T &a, T &b)
{
    T c = a;
    a = b;
    b = c;
}

template <typename T>
void initialize(Heap<T> &h)
{
    h.size = 0;
}

template <typename T>
void destroy(Heap<T> &h)
{
    h.size = 0;
}

template <typename T>
bool isEmpty(Heap<T> h)
{
    return (h.size == 0);
}

template <typename T>
void put(Heap<T> &h, T value)
{
    // Place new value at bottom of heap
    h.data[++h.size] = value;
    
    // Restore heap ordering, as needed
    int x = h.size;
    while( x > 1 && h.data[x] < h.data[x/2] )
    {
        myswap(h.data[x],h.data[x/2]);
        x /= 2;
    }
}

template <typename T>
T get(Heap<T> &h)
{
    // Move last value to root
    myswap(h.data[1],h.data[h.size]);
    h.size--;
    
    // Restore heap ordering, as needed
    int x = 1;
    while( true )
    {
        int min = ( x*2+1 <= h.size && h.data[x*2+1] < h.data[x*2] ? x*2+1 : x*2 );
        if( min > h.size || h.data[x] <= h.data[min] )
            break;
        myswap(h.data[x],h.data[min]);
        x = min;
    }
    
    return h.data[h.size+1]; // old root value
}


genNode maxFour(genNode&a, genNode&b, genNode&c, genNode&d)
{
    if(a>b)
    {
        if(a>c)
        {
            if(a>d)//a>b,c,d
                return a;
            return d;//a>b,c but d>=a
        }
        if(c>d)
            return c;//a>b and c>=a and c>d
        return d;// a>b and c>a and d>c
    }
    if(b>c)
    {
        if(b>d)
            return b;
        return d;
    }
    if(c>d)
        return c;
    return d;
}


//fake fitness simulation on (a)
/*
 int sum =0;
 for(int k = 0;str1[k] != 0; k++)
 {
 if(str1[k] == '1')
 sum++;
 }
 a.strength = sum;
 */
//end fake fitness simulation

/*
 cout<<"randomized initial heap: \n";
 int k = 1;
 while(!isEmpty(h))
 {
 cout<<"\ngenome "<<k<<": ";
 genNode cur = get(h);
 cout<<"strength = "<<cur.strength<<endl;
 cout<< cur.genome <<endl;
 cout<<"translation: ";
 string readable="";
 for(int i=0; i<f.steps * 2; i+=2)
 {
 if(cur.genome[i] == '1')
 {
 if(cur.genome[i+1] == '1')//11
 readable+='N';
 else                    //10
 readable+='W';
 }
 else
 {
 if(cur.genome[i+1] == '1')//01
 readable+='E';
 else                       //00
 readable+='S';
 }
 }
 cout<<readable<<endl;
 k++;
 }
 */

#endif
