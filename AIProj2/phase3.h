//
//  phase3.h
//  AIProj2
//
//  Created by Joseph on 4/3/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

#ifndef phase3_h
#define phase3_h
#include <iostream>
#include "geneticheap.h"


void p3mutate(genNode& a)
{
    int i=0;
    //  bool changed=false;
    while(a.genome[i] != 0)//0 term string
    {
        if((rand() % 1000) < (50+i))//5% flip chance, further to the right = increased chance
        {
            if(a.genome[i] == '1')
                a.genome[i] = '0';
            else
                a.genome[i]='1';
            //          changed = true;
        }
        i++;
    }
    //   return changed;
}
static int p3fitcalls = 0;
void p3fitness(genNode& specimen, const forest& f)
{
    
    p3fitcalls++;
    string dir; int leftspot=f.startstr, rightspot=f.startchar;
    int worthy = 0;
    specimen.wallhit=0;
    specimen.food=0;
    specimen.dead=false;
    bool used[200][200];
    
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
            specimen.wallhit++;
        }
        else if(f.maze[leftgo][rightgo] == 'd')//died
        {
            specimen.strength = worthy - 10;
            //         cout<<"current specimen died, has "<<specimen.strength<<"fitness\n";
            specimen.dead = true;
            return;
        }
        else if(f.maze[leftgo][rightgo] == 'f')//food
        {
            if(!used[leftgo][rightgo])
            {
                worthy += 20;
                used[leftgo][rightgo]=true;
                specimen.food++;
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
    genNode * commies = new genNode[1001];//num of lifeforms
    for(int o=1; o<1001; o++)//every lifeform
    {
        commies[o] = h.data[o];
        int i=0;
        while(commies[o].genome[i] != 0)//0 term string
        {
            if((rand() % 100) < 25)//20% flip chance
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
    for(int i=1; i<1000; i++)//put em back
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
    for(int i=2; i<1001; i++)//find the second and first strongest
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
        for(int i=1; i<1000; i++)//check if all same genome str
        {
            if(h.data[i].strength != h.data[i+1].strength)//not same, repeat
            {
                cout<<"not all eq yet\n";
                alleq=false;
                i=1000;
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
                    for(int i=2; i<1001; i++)//recheck 1st2ndbest and worst
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
                for(int i=2; i<1001; i++)//recheck 1st2ndbest and worst
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
}
genNode beginp3Genetics(const forest& f)
{
    srand(time(NULL));
    Heap<genNode> h;
    initialize(h);
    string str1="";
    for(int i=0; i<1000; i++)//this is the number of genomes
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

//cout<< "\nWe have come to a standstill.\nThe best genome found is: ";
/*
 int k = 1;
 //while(!isEmpty(h))
 {
 cout<<"\ngenome "<<k<<": ";
 //genNode cur = get(h);
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
#endif /* phase3_h */
