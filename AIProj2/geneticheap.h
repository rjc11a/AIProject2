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
#include <cmath>
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
struct genNode{
    string genome;
    int strength;
};

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

//fitness function for phase 1
void p1fitness(genNode& specimen, const geneticSack& s)
{
    int tolwt=0;
    specimen.strength=0;
    for(int i=0; i<s.items; i++)//for every item
    {
        if(specimen.genome[i] == '1')//item i taken on the genome
        {
            tolwt += s.costs[i];
            specimen.strength += s.values[i];
            
        }
    }
    if(tolwt > s.capacity)//over capacity
    {
        specimen.strength = 0;
    }
    return;
}
void p2fitness(genNode& specimen, const forest& f)
{
    string dir; int leftspot=f.startstr, rightspot=f.startchar;
    int worthy = 0;
    bool used[f.dimensions][f.dimensions];
    
    for(int i=0; i<f.dimensions; i++)
    {
        for(int o=0; o<f.dimensions; o++)
            used[i][o] = false;
    }
    for(int i=0; i<f.steps * 2; i+=2)
    {
        int leftgo = leftspot, rightgo = rightspot;
        dir = "";
    //    dir += specimen.genome[i] + specimen.genome[i+1];
        dir += specimen.genome[i];
        dir += specimen.genome[i+1];//next dir
    //    cout<<"going "<<dir<<"from string "<<leftspot<<" char "<<rightspot<<endl;
        
        if(dir == "11")//north
            leftgo -=1;
        else if(dir == "10")
            rightgo -=1;
        else if(dir == "01")
            rightgo +=1;
        else
            leftgo +=1;
    //    cout<<"next spot char is "<<f.maze[leftgo][rightgo]<<endl;
        if(f.maze[leftgo][rightgo] == 'x')//wall
        {
            //do nothing
        }
        else if(f.maze[leftgo][rightgo] == 'd')//died
        {
            specimen.strength = worthy - 10;
   //         cout<<"current specimen died, has "<<specimen.strength<<"fitness\n";
            return;
        }
        else if(f.maze[leftgo][rightgo] == 'f')//food
        {
            if(!used[leftgo][rightgo])
            {
                worthy += 20;
                used[leftgo][rightgo]=true;
            }
            leftspot=leftgo; rightspot=rightgo;
        }
        else                                //can move
        {
            leftspot = leftgo; rightspot = rightgo;
        }
    }
    specimen.strength = worthy;
 //   cout<<"current specimen has "<<worthy<<"fitness"<<endl;
}
void p2mutate(genNode& a)
{
    int i=0;
    bool changed=false;
    while(a.genome[i] != 0)//0 term string
    {
        if((rand() % 1000) < 5)//0.5% flip chance
        {
            if(a.genome[i] == '1')
                a.genome[i] = '0';
            a.genome[i]='1';
            changed = true;
        }
        i++;
    }
 //   return changed;
}
genNode p1reproduce(const genNode &a,const genNode &b, const geneticSack &s)
{
    genNode baby1, baby2;
    baby1.genome="";
    baby2.genome="";
    int enda = rand() % (s.items-1);
    for(int i=0; i<= enda; i++)
    {
        baby1.genome+=a.genome[i];
        baby2.genome+=b.genome[i];
    }
    for(int i=enda+1; i<s.items; i++)
    {
        baby1.genome += b.genome[i];
        baby2.genome += a.genome[i];
    }
    
    // mutations go here
    
    // end mutations
    
    p1fitness(baby1, s);
    p1fitness(baby2, s);
    if(baby1.strength > baby2.strength)//baby1 is better
        return baby1;
    return baby2;
}
void p3mutate(genNode& a)
{
    int i=0;
  //  bool changed=false;
    while(a.genome[i] != 0)//0 term string
    {
        if((rand() % 100) < 5)//5% flip chance
        {
            if(a.genome[i] == '1')
                a.genome[i] = '0';
            a.genome[i]='1';
  //          changed = true;
        }
        i++;
    }
    //   return changed;
}
genNode p2reproduce(const genNode &a,const genNode &b, const forest& f)
{
    genNode baby1, baby2;
    baby1.genome="";
    baby2.genome="";
    int enda = ((rand() * 2) +1) % ((f.steps * 2)-2); //force rand() to be odd, odd%even = odd
    for(int i=0; i<= enda; i++)
    {
        baby1.genome+=a.genome[i];
        baby2.genome+=b.genome[i];
    }
    for(int i=enda+1; i<(f.steps * 2); i++)
    {
        baby1.genome += b.genome[i];
        baby2.genome += a.genome[i];
    }
    
    // mutations go here
    p2mutate(baby1);
    p2mutate(baby2);
    // end mutations
    
    p2fitness(baby1, f);
    p2fitness(baby2, f);
    if(baby1.strength > baby2.strength)//baby1 is better
        return baby1;
    return baby2;
}

void p2cataclysm(Heap<genNode>&h, const forest& f)
{
    genNode * commies = new genNode[101];
    for(int o=1; o<101; o++)
    {
        commies[o] = h.data[o];
        int i=0;
        while(commies[o].genome[i] != 0)//0 term string
        {
            if((rand() % 100) < 20)//20% flip chance
            {
                if(commies[o].genome[i] == '1')
                    commies[o].genome[i] = '0';
                commies[o].genome[i]='1';
            }
            i++;
        }
    }
    h.size=1;
    for(int i=1; i<100; i++)
    {
        p2fitness(commies[i], f);
        put(h,commies[i]);//put the stuff back in heap
    }
    delete [] commies;
}

//survival function for phase 1
void p2survive(Heap<genNode> &h, const forest& f)//initted heap
{
    //   int weight=0, val=0;
    cout<<"beginning survival:\n";
    int converges = 0;
    genNode secBest=h.data[1], frsBest=h.data[2], baby, sacrifice; //init the needed nodes
    for(int i=3; i<101; i++)//find the second and first strongest
    {
        if(h.data[i] > secBest)
        {
            if(h.data[i] > frsBest)
            {
                secBest = frsBest;
                frsBest = h.data[i];
            }
            else
                secBest = h.data[i];
        }
    }
    bool redo = true;
    int third = 3, second = 2, first = 1;
    while(redo)
    {
 //       cout<<"redoing.\n";
        while(! (h.data[1].strength == h.data[100].strength) )
        {
            baby = p2reproduce(frsBest, secBest, f);
            if(baby > secBest)
            {
                if(baby > frsBest)
                {
                    secBest = frsBest;
                    frsBest = baby;
                }
                else
                    secBest = baby;
            }
            put(h, baby);
            sacrifice = get(h);
        }
        bool alleq = true;
        for(int i=1; i<100; i++)//check if all same genome
        {
            if(h.data[i].strength != h.data[i+1].strength)//not same, repeat
            {
      //          cout<<"not all eq yet\n";
                alleq=false;
                i=100;
            }
            else
            {
                baby = p2reproduce(frsBest, secBest, f);
                if(baby > secBest)
                {
                    if(baby > frsBest)
                    {
                        secBest = frsBest;
                        frsBest = baby;
                    }
                    else
                        secBest = baby;
                }
                put(h, baby);
                sacrifice = get(h);
            }
        }
        if(alleq) //was all same, checkem
        {
            converges++;
            cout<<"converged "<<converges<<" times, strength: "<<h.data[1].strength;
            third=second;
            second=first;
            first=h.data[1].strength;
            
            if(third == second && second == first) //we done 4 real tho
            {
                redo = false;
            }
            else//not done yet better cataclysmize
            {
                p2cataclysm(h, f);
                secBest=h.data[1];
                frsBest=h.data[2];
                for(int i=3; i<101; i++)//recheck 1st2ndbest
                {
                    if(h.data[i] > secBest)
                    {
                        if(h.data[i] > frsBest)
                        {
                            secBest = frsBest;
                            frsBest = h.data[i];
                        }
                        else
                            secBest = h.data[i];
                    }
                }
            }
        }
        
    }
    cout<< "we have come to a standstill.\nThe best genomes are: ";
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
}
static int p3fitcalls = 0;
void p3fitness(genNode& specimen, const forest& f)
{
    
    p3fitcalls++;
    string dir; int leftspot=f.startstr, rightspot=f.startchar;
    int worthy = 0;
    bool used[f.dimensions][f.dimensions];
    
    for(int i=0; i<f.dimensions; i++)
    {
        for(int o=0; o<f.dimensions; o++)
            used[i][o] = false;
    }
    for(int i=0; i<f.steps * 2; i+=2)
    {
        int leftgo = leftspot, rightgo = rightspot;
        dir = "";
        //    dir += specimen.genome[i] + specimen.genome[i+1];
        dir += specimen.genome[i];
        dir += specimen.genome[i+1];//next dir
        //    cout<<"going "<<dir<<"from string "<<leftspot<<" char "<<rightspot<<endl;
        
        if(dir == "11")//north
            leftgo -=1;
        else if(dir == "10")
            rightgo -=1;
        else if(dir == "01")
            rightgo +=1;
        else
            leftgo +=1;
        //    cout<<"next spot char is "<<f.maze[leftgo][rightgo]<<endl;
        if(f.maze[leftgo][rightgo] == 'x')//wall
        {
            //lose 1 fitness, waste turn
            worthy -= 1;
        }
        else if(f.maze[leftgo][rightgo] == 'd')//died
        {
            specimen.strength = worthy - 10;
            //         cout<<"current specimen died, has "<<specimen.strength<<"fitness\n";
            return;
        }
        else if(f.maze[leftgo][rightgo] == 'f')//food
        {
            if(!used[leftgo][rightgo])
            {
                worthy += 20;
                used[leftgo][rightgo]=true;
            }
            leftspot=leftgo; rightspot=rightgo;
        }
        else                                //can move
        {
            leftspot = leftgo; rightspot = rightgo;
        }
    }
    specimen.strength = worthy;
    //   cout<<"current specimen has "<<worthy<<"fitness"<<endl;
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
genNode p3reproduce(const genNode &a,const genNode &b, const forest& f)
{
    genNode baby1, baby2, mutatedbaby1, mutatedbaby2;
    baby1.genome="";
    baby2.genome="";
    int enda = ((rand() * 2) +1) % ((f.steps * 2)-2); //force rand() to be odd, odd%even = odd
    for(int i=0; i<= enda; i++)
    {
        baby1.genome+=a.genome[i];
        baby2.genome+=b.genome[i];
    }
    for(int i=enda+1; i<(f.steps * 2); i++)
    {
        baby1.genome += b.genome[i];
        baby2.genome += a.genome[i];
    }
    mutatedbaby1=baby1;
    mutatedbaby2=baby2;
    // mutations go here
    p3mutate(mutatedbaby1);
    p3mutate(mutatedbaby2);
    // end mutations
    
    p3fitness(baby1, f);
    p3fitness(baby2, f);
    p3fitness(mutatedbaby1,f);
    p3fitness(mutatedbaby2,f);
    return maxFour(baby1,baby2,mutatedbaby1,mutatedbaby2);
}

void p3cataclysm(Heap<genNode>&h, const forest& f)
{
    genNode * commies = new genNode[10001];//num of lifeforms
    for(int o=1; o<10001; o++)//every lifeform
    {
        commies[o] = h.data[o];
        int i=0;
        while(commies[o].genome[i] != 0)//0 term string
        {
            if((rand() % 100) < 25)//20% flip chance
            {
                if(commies[o].genome[i] == '1')
                    commies[o].genome[i] = '0';
                commies[o].genome[i]='1';
            }
            i++;
        }
    }
    h.size=1;
    for(int i=1; i<10000; i++)//put em back
    {
        p3fitness(commies[i], f);
        put(h,commies[i]);//put the stuff back in heap
    }
    delete [] commies;
}

//survival function for phase 1
void p3survive(Heap<genNode> &h, const forest& f)//initted heap
{
    //   int weight=0, val=0;
    cout<<"beginning survival:\n";
    int converges = 0;
    genNode secBest=h.data[1], frsBest=h.data[1], worst = h.data[1], baby, sacrifice; //init the needed nodes
    for(int i=2; i<10001; i++)//find the second and first strongest
    {
        if(h.data[i] > secBest)
        {
            if(h.data[i] > frsBest)
            {
                secBest = frsBest;
                frsBest = h.data[i];
            }
            else
                secBest = h.data[i];
        }
        if(h.data[i] < worst)
            worst = h.data[i];
    }
    bool redo = true;
    int third = 3, second = 2, first = 1;
    while(redo)
    {
        cout<<"redoing.\n";
        while(! (frsBest.strength == worst.strength) )//not eq str
        {
            baby = p3reproduce(frsBest, secBest, f);
            if(baby > secBest)
            {
                if(baby > frsBest)
                {
                    secBest = frsBest;
                    frsBest = baby;
                }
                else
                    secBest = baby;
            }
            put(h, baby);
            worst = get(h);
        }
        bool alleq = true;
        for(int i=1; i<10000; i++)//check if all same genome str
        {
            if(h.data[i].strength != h.data[i+1].strength)//not same, repeat
            {
                cout<<"not all eq yet\n";
                alleq=false;
                i=10000;
            }
        }
        if(alleq) //was all same, checkem
        {
           // converges++;
            cout<<"converged "<<converges<<" times, strength: "<<h.data[1].strength<<". ";
            third=second;
            second=first;
            first=h.data[1].strength;
            
            if(third == second && second == first ) //we done 4 real tho
            {
                converges++;
                if(converges == 5)
                    redo = false;
                else
                {
                    cout<<"***CATACLYSM***\n";
                    p3cataclysm(h, f);
                    secBest=h.data[1];
                    frsBest=h.data[1];
                    worst = h.data[1];
                    for(int i=2; i<10001; i++)//recheck 1st2ndbest and worst
                    {
                        if(h.data[i] > secBest)
                        {
                            if(h.data[i] > frsBest)
                            {
                                secBest = frsBest;
                                frsBest = h.data[i];
                            }
                            else
                                secBest = h.data[i];
                        }
                        if(h.data[i] < worst)
                            worst = h.data[i];
                    }
                }
            }
            else//not done yet better cataclysmize
            {
                cout<<"***CATACLYSM***\n";
                converges = 0;
                p3cataclysm(h, f);
                secBest=h.data[1];
                frsBest=h.data[1];
                worst = h.data[1];
                for(int i=2; i<10001; i++)//recheck 1st2ndbest and worst
                {
                    if(h.data[i] > secBest)
                    {
                        if(h.data[i] > frsBest)
                        {
                            secBest = frsBest;
                            frsBest = h.data[i];
                        }
                        else
                            secBest = h.data[i];
                    }
                    if(h.data[i] < worst)
                        worst = h.data[i];
                }
            }
        }
        
    }
    cout<< "\nWe have come to a standstill.\nThe best genome found is: ";
    int k = 1;
    //while(!isEmpty(h))
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
}
void beginp1Genetics(const geneticSack &s )//this is the first func, initializes heap
{
    srand(time(NULL));
    Heap<genNode> h;
    string str1="";
    for(int i=0; i<100; i++)//this is the number of genomes
    {
        str1 = "";//init gen
        for(int iterations=0; iterations<s.items; iterations++)//this is the number of items
        {
            str1 += ('0' + rand() % 2);
        }
        genNode a;
        a.genome = str1;
        
        
        p1fitness(a, s);
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
        
        put(h, a);
        
        
    }
    //heap is complete
    cout<<"randomized initial heap: \n";
    int k = 1;
    while(!isEmpty(h))
    {
        cout<<"\ngenome "<<k<<": ";
        cout<<(get(h).genome)<<endl;
        k++;
    }
}

void beginp2Genetics(const forest &f )//this is the first func, initializes heap
{
    srand(time(NULL));
    Heap<genNode> h;
    initialize(h);
    string str1="";
    for(int i=0; i<100; i++)//this is the number of genomes
    {
        str1 = "";//init gen
        for(int iterations=0; iterations< f.steps *2; iterations++)//this is the number of items
        {
            str1 += ('0' + rand() % 2);
        }
        genNode a;
        a.genome = str1;
        
        
        p2fitness(a, f);
     //   cout<<"just made "<<a.genome<<" with strength "<<a.strength<<endl;
        put(h, a);
        
        
    }
    //heap is complete
    p2survive(h, f);
}

genNode beginp3Genetics(const forest& f)
{
    srand(time(NULL));
    Heap<genNode> h;
    initialize(h);
    string str1="";
    for(int i=0; i<10000; i++)//this is the number of genomes
    {
        str1 = "";//init gen
        for(int iterations=0; iterations< f.steps *2; iterations++)//this is the number of items
        {
            str1 += ('0' + rand() % 2);
        }
        genNode a;
        a.genome = str1;
        
        
        p3fitness(a, f);
 //       cout<<"just made "<<a.genome<<" with strength "<<a.strength<<endl;
        put(h, a);
        
        
    }
    //heap is complete
    p3survive(h, f);
    return h.data[1];
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

