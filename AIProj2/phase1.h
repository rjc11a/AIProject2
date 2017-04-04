//
//  phase1.h
//  AIProj2
//
//  Created by Joseph on 4/3/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

#ifndef phase1_h
#define phase1_h
#include <iostream>
#include "geneticheap.h"

//fitness function for phase 1
static int p1fitcalls = 0;
void p1fitness(p1Node& specimen, const geneticSack& s)
{
    p1fitcalls++;
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

void p1mutate(p1Node& a)
{
    int i=0;
    //    bool changed=false;
    while(a.genome[i] != 0)//0 term string
    {
        if((rand() % 1000) < 5)//0.5% flip chance
        {
            if(a.genome[i] == '1')
                a.genome[i] = '0';
            else
                a.genome[i]='1';
            //            changed = true;
        }
        i++;
    }
    //   return changed;
}
p1Node p1reproduce(const p1Node &a,const p1Node &b, const geneticSack &s)
{
    p1Node baby1, baby2;
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
    p1mutate(baby1);
    p1mutate(baby2);
    // end mutations
    
    p1fitness(baby1, s);
    p1fitness(baby2, s);
    if(baby1.strength > baby2.strength)//baby1 is better
        return baby1;
    return baby2;
}

void p1cataclysm(Heap<p1Node>&h, const geneticSack& s)
{
    p1Node * commies = new p1Node[101];
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
                else
                    commies[o].genome[i]='1';
            }
            i++;
        }
    }
    h.size=1;
    for(int i=1; i<101; i++)
    {
        p1fitness(commies[i], s);
        put(h,commies[i]);//put the stuff back in heap
    }
    delete [] commies;
}

void p1survive(Heap<p1Node> &h, const geneticSack& s)//initted heap
{
    //   int weight=0, val=0;
    cout<<"beginning survival:\n";
    int converges = 0;
    p1Node secBest=h.data[1], frsBest=h.data[1], baby, sacrifice; //init the needed nodes
    for(int i=2; i<101; i++)//find the second and first strongest
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
        cout<<"redoing.\n";
        while(! (h.data[1].strength == frsBest.strength) )
        {
            baby = p1reproduce(frsBest, secBest, s);
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
                baby = p1reproduce(frsBest, secBest, s);
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
                p1cataclysm(h, s);
                secBest=h.data[1];
                frsBest=h.data[1];
                for(int i=2; i<101; i++)//recheck 1st2ndbest
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
    
}

p1Node beginp1Genetics(const geneticSack &s )//this is the first func, initializes heap
{
    srand(time(NULL));
    Heap<p1Node> h;
    initialize(h);
    string str1="";
    for(int i=0; i<100; i++)//this is the number of genomes
    {
        cout<<"beginning p1, iteration "<<i<<endl;
        str1 = "";//init gen
        for(int iterations=0; iterations<s.items; iterations++)//this is the number of items
        {
            str1 += ('0' + rand() % 2);
        }
        p1Node a;
        a.genome = str1;
        
        
        p1fitness(a, s);
        
        put(h, a);
        
        
    }
    
    p1survive(h,s);
    return h.data[1];
}

//lololololol maymays;
//heap is complete
/*
 cout<<"randomized initial heap: \n";
 int k = 1;
 while(!isEmpty(h))
 {
 cout<<"\ngenome "<<k<<": ";
 cout<<(get(h).genome)<<endl;
 k++;
 }
 */
/*
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
 */

#endif /* phase1_h */
