//
//  phase2.h
//  AIProj2
//
//  Created by Joseph on 4/3/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

#ifndef phase2_h
#define phase2_h
#include <iostream>
#include "geneticheap.h"


void p2fitness(genNode& specimen, const forest& f)
{
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
            //do nothing
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
void p2mutate(genNode& a)
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

genNode p2reproduce(const genNode &a,const genNode &b, const forest& f)
{
    genNode baby1, baby2;
    baby1.genome="";
//    baby2.genome="";
    int enda = ((rand() * 2) +1) % ((f.steps * 2)-2); //force rand() to be odd, odd%even = odd
    for(int i=0; i<= enda; i++)
    {
        baby1.genome+=a.genome[i];
//        baby2.genome+=b.genome[i];
    }
    for(int i=enda+1; i<(f.steps * 2); i++)
    {
        baby1.genome += b.genome[i];
//        baby2.genome += a.genome[i];
    }
    
    // mutations go here
    p2mutate(baby1);
    //p2mutate(baby2);
    // end mutations
    
    p2fitness(baby1, f);
//    p2fitness(baby2, f);
//    if(baby1.strength > baby2.strength)//baby1 is better
    return baby1;
//    return baby2;
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
                else
                    commies[o].genome[i]='1';
            }
            i++;
        }
    }
    h.size=1;
    for(int i=1; i<101; i++)
    {
        p2fitness(commies[i], f);
        put(h,commies[i]);//put the stuff back in heap
    }
    delete [] commies;
}


void p2survive(Heap<genNode> &h, const forest& f)//initted heap
{
    //   int weight=0, val=0;
    cout<<"beginning survival:\n";
    int converges = 0;
    genNode secBest=h.data[1], frsBest=h.data[1], baby, sacrifice; //init the needed nodes
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
        //       cout<<"redoing.\n";
        while(! (h.data[1].strength == frsBest.strength) )
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
                
                sacrifice = get(h);
                put(h,frsBest);//keep the old best one
                
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
}


genNode beginp2Genetics(const forest &f )//this is the first func, initializes heap
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
    return h.data[1];
}


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
 }*/
#endif /* phase2_h */
