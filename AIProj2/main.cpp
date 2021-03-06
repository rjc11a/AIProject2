//
//  main.cpp
//  AIProj1
//
//  Created by Joseph on 2/18/17.
//  Copyright © 2017 JosephC. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <stdlib.h>
using namespace std;
#include <fstream>
#include "sorting.h"
#include <iomanip>
#include "phase1.h"
#include "phase2.h"
#include "phase3.h"

struct sack{
    string * names;
    int* costs;
    int* values;
    double* ratios;
};
void init_sack(sack &s, int n){
    s.names = new string[n];
    s.costs = new int[n];
    s.values = new int[n];
    s.ratios = new double[n];
}
int valueofsack(sack* a, int n)
{
    int tot = 0;
    for(int i=0; i<n; i++)
    {
        tot += a->values[i];
    }
    return tot;
}
void copylefttoright(sack * a, sack * b, int n)//unused
{
    
}
void filterlefttoright(sack* a,int items, sack* b, int& miniitems, bool* master)
{
    miniitems = 0;
    for(int i=0; i<items; i++)
    {
        if(master[i])
        {
            b->names[miniitems]=a->names[i];
            b->costs[miniitems]=a->costs[i];
            b->values[miniitems]=a->values[i];
            b->ratios[miniitems]=a->ratios[i];
            miniitems++;
        }
    }
}

void destroy(sack& s)
{
    delete [] s.names;
    delete [] s.costs;
    delete [] s.values;
    delete [] s.ratios;
}
void greedy(int n, sack* s, int maxweight, bool* masterlist)
{
  //  cout<<"doing greedy, "<<n<<"items, max weight = "<<maxweight<<"\n";
    for(int i=0; i<n; i++)
    {
        masterlist[i] = false;
    }
    //s is ordered in descending usefulness
    int tolwt=0;
    int tolval=0;
    //initially sack is empty
    for(int i=0; i<n; i++)
    {
  //      cout<<"trying "<<s->names[i]<<", ";
        tolwt += s->costs[i];//try adding current item to sack or skip
        if(tolwt <= maxweight)
        {
  //          cout<<"success\n";
            tolval += s->values[i];
            masterlist[i] = true;
        }
        else
        {
  //          cout<<"failed\n";
            tolwt -= s->costs[i];
        }
    }
 //   cout<<"weight after greedy: "<<tolwt;
}
bool partial(int n, sack* s, int maxweight, bool* masterlist, double& pct, int &extraspot)
{
    for(int i=0; i<n; i++)
    {
        masterlist[i] = false;
    }
    //s is ordered in descending usefulness
    int tolwt=0;
    double tolval=0;
    //initially sack is empty
    for(int i=0; i<n; i++)
    {
        tolwt+=s->costs[i];//try adding current item to sack or else if not underweight
        if(tolwt < maxweight)
        {
            tolval += s->values[i];
            masterlist[i] = true;
        }
        else //not underweight
        {
            int wtgap = tolwt - maxweight;
            if (wtgap != 0) //overweight
            {
                tolwt -= s->costs[i];
                wtgap = maxweight - tolwt;
                int partial = s->costs[i];//give partial i's cost
                pct = (double) wtgap / (double) partial; //ratio is gap/partial
                extraspot = i;
                return true;
            }
            else//perfect fit
            {
                return false;
            }
        }
    }
    return false;
}
void brute(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval)
{
    //cur is current spot in the bool list simulated tree, initially called with 0
    //n is total length of tree
    //list is
    
    //check cur vs int
    if(cur>=n)
        return;
    
    //exclude current
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    
    //include current
    weight += s->costs[cur];
    value += s->values[cur];
    list[cur]=true;
    
    //current value is better than best value seen and valid weight
    if(value > bestval && weight <= maxweight)
    {
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    brute(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval);
    list[cur]=false;
}

void exhaustive(int cur, int n, sack* s, int weight, int value, int maxweight, bool* list, bool* masterlist, int &bestval, int rem, int& toppest)
{
    //check cur vs int
    if(cur>=n)
        return;
    if(value + rem < toppest)
        return;
    //exclude current
    
    rem = rem - s->values[cur];
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval, rem, toppest);
    
    //include current
    weight += s->costs[cur];
    
    //if overweight then break branch
    if(weight > maxweight)
        return;
    //if unfruitful return
    //if current total value + 
    
    
    
    value += s->values[cur];
    //not underweight,
    list[cur]=true;
    
    //current value is better than best value seen,
    if(value > bestval)
    {
        if( value > toppest)
            toppest = value;
        bestval = value;
        for(int i=0; i<n; i++)
        {
            masterlist[i] = list[i];
        }
    }
    exhaustive(cur+1,n,s,weight,value, maxweight,list,masterlist,bestval, rem, toppest);
    list[cur] = false;
}

void printsack(sack s, int n)
{
    cout<<"SACK_BEGIN\n";
    int weight = 0;
    int value = 0;
    for(int i=0; i<n; i++)
    {
        weight += s.costs[i];
        value += s.values[i];
        cout<<s.names[i]<<","<<s.costs[i]<<","<<s.values[i]<<','<<s.ratios[i]<<endl;
    }
    cout<<"TOTAL WEIGHT: "<<weight<<endl;
    cout<<"TOTAL VALUE: "<<value <<endl;
    cout<<"/SACK_END\n\n";
}
string printsack2(sack s, int n)
{
    //print cost and items
    int val=0;
    int cst=0;
    for(int i=0; i<n; i++)
    {
        val += s.values[i];
        cst += s.costs[i];
    }
    string start = "value: ";
    start += to_string(val);
    start += ", cost: ";
    start += to_string(cst);
    start += ", items: ";
    start += s.names[0];
    for(int i=1; i<n; i++)
    {
        start += ", ";
        start += s.names[i];
    }
    return start;
}
string printsack3(sack s, int n, double d, int c)
{
    //print cost and items
    double val=0;
    for(int i=0; i<n; i++)
    {
        val += s.values[i];
    }
    val += d;
    int realval = val;
    string start = "value: ";
    start += to_string(realval);
    start += ", cost: ";
    start += to_string(c);
    start += ", items: ";
    start += s.names[0];
    for(int i=1; i<n; i++)
    {
        start += ", ";
        start += s.names[i];
    }
    return start;
}


//struct geneticSack{
//    int items;
//    int capacity;
//    string * names;
//    int* costs;
//    int* values;
//    double* ratios;
//};
//void init_sack(geneticSack &s, int n, int c){
//    
//    s.items = n;
//    s.capacity = c;
//    s.names = new string[n];
//    s.costs = new int[n];
//    s.values = new int[n];
//    s.ratios = new double[n];
//}
int main()
{
    forest f;
 //   s1.items = 25;
 //   beginGenetics(s1);
    
     //string out1="1. ",out2="",out3="",out4="",out5="",out6="6. ",out7="7. ";
     string filename1, filename2, bigline, readable2="", readable=""; int items,capacity; geneticSack sack1;
    p1Node p1strand; genNode survivor,p2strand;
    bool p1done=false, p3done=false;
    ifstream fin;
    ofstream fout;
    cout<<"Enter exact filename for Phase1 testing: ";
    cin>>filename1; //    out1 += filename;
    cout<<"Enter exact filename for Phase 2/3 testing:";
    cin>> filename2;
    fin.open(filename1.c_str());
    if(fin.is_open())
    {
        p1done = true;
        cout<<"file opened successfully.\n";
  //       init_sack(sack1, 200);
         
        int cur = 0;
        string store="";
        getline(fin,store);
        //        out2 = "2. ";
        //        out2+=store;
        capacity = stoi(store);
        init_sack(sack1, 200, capacity);
        cout<<store<<endl;
        while(!fin.eof())
        {
         //   cout<<"took a line again\n";
            getline(fin,bigline);
            if(bigline == "")
                break;
            store="";
            //item name
            int ii = 0;
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.names[cur] = store;
            ii++;
            store = "";
            //1st int
            for(;bigline[ii]!=','; ii++)//until char is comma
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.costs[cur] = stoi(store);
            ii++;
            store = "";
            //2nd int
            for(;bigline[ii]!=0; ii++)//until 0 termin string
            {
                store += bigline[ii]; //add char to sto
            }
            sack1.values[cur] = stoi(store);
            cur++;
        }
        items = cur;
        sack1.items = items;
        //        printsack(sack1, items);
        int totsumsum = 0;
        int totval = 0;
        for(int i=0; i<items; i++)
        {
            totsumsum+=sack1.costs[i];
            totval += sack1.values[i];
        }
        cout<<"TOTAL COST ALL ITEMS = "<<totsumsum<<endl;
        cout<<"TOTAL VALUE ALL ITEMS = "<<totval<<endl;

        cout<<"Beginning Phase 1.\n";
        
        p1strand = beginp1Genetics(sack1);
        cout<<"Phase 1 complete.\n";
        cout<<p1strand.genome<<endl<<"strength: "<<p1strand.strength<<endl;
     }
    else
        cout<<"Failed to open "<<filename1<<".\n";
    
    
    fin.close();
    cout<<"Proceeding with Phase 2.\n";
    fin.open(filename2.c_str());
    if(fin.is_open())
    {
        p3done = true;
        fin>>f.dimensions;
        fin>>f.startstr;
        fin>>f.startchar;
        fin>>f.steps;
        for(int i=0; i<f.dimensions; i++)
        {
            cout<<i<<"th string: ";
            fin>>f.maze[i];
            cout<<f.maze[i]<<endl;
        }
    
        for(int i=0; i<20; i++)
            cout<<f.maze[i]<<endl;
        p2strand = beginp2Genetics(f);
        cout<<"A survivor has appeared for Phase 2.\ngenome: \n"<<p2strand.genome<<endl;
        
        cout<<"translation: ";
        
        for(int i=0; i<f.steps * 2; i+=2)
        {
            if(p2strand.genome[i] == '1')
            {
                if(p2strand.genome[i+1] == '1')//11
                    readable2+='N';
                else                    //10
                    readable2+='W';
            }
            else
            {
                if(p2strand.genome[i+1] == '1')//01
                    readable2+='E';
                else                       //00
                    readable2+='S';
            }
        }
        cout<<readable2<<endl;
        
        cout<<"***breakdown***\n";
        cout<<"strength = "<<p2strand.strength<<endl<<"walls hit (-1 per): "<<p2strand.wallhit<<endl<<"food found (+20 per): "<<p2strand.food<<endl;
        if(p2strand.dead)
            cout<<"This specimen died.\n";
        else cout<<"This specimen did not die.\n";
        
        
        //cout<<endl<<"tot fitness calls: "<<p3fitcalls<<endl;
        //cout<<"the brute force exhaustive calls for this input would be: 4^"<<f.steps<<
        //" = "<<pow(4,f.steps)<<endl;
        
        cout<<endl<<endl;

        
        cout<<"beginning Phase 3:\n";
        
        genNode i1 = beginp3Genetics(f);
        genNode i2 = beginp3Genetics(f);
        genNode i3 = beginp3Genetics(f);
        genNode i4 = beginp3Genetics(f);
        
        survivor = maxFour(i1, i2, i3, i4);
        
//        for(int i=0; i<10; i++)
//        {
//            genNode surv2 = beginp3Genetics(f);
//            if(surv2>survivor)
//                survivor = surv2;
//        }
   
        cout<<"A survivor has appeared.\ngenome: \n"<<survivor.genome<<endl;
        
        cout<<"translation: ";
        
        for(int i=0; i<f.steps * 2; i+=2)
        {
            if(survivor.genome[i] == '1')
            {
                if(survivor.genome[i+1] == '1')//11
                    readable+='N';
                else                    //10
                    readable+='W';
            }
            else
            {
                if(survivor.genome[i+1] == '1')//01
                    readable+='E';
                else                       //00
                    readable+='S';
            }
        }
        cout<<readable<<endl;

        
        
        cout<<"***breakdown***\n";
        cout<<"strength = "<<survivor.strength<<endl<<"walls hit (-1 per): "<<survivor.wallhit<<endl<<"food found (+20 per): "<<survivor.food<<endl;
        if(survivor.dead)
            cout<<"This specimen died.\n";
        else cout<<"This specimen did not die.\n";

        
        cout<<endl<<"tot fitness calls: "<<p3fitcalls<<endl;
        cout<<"the brute force exhaustive calls for this input would be: 4^"<<f.steps<<
        " = "<<pow(4,f.steps)<<endl;
   
    }
    else
        cout<<"unable to open.\n";
    
    
    /***** This is the output section *****/
    cout<<"finished all computation.\n\n\n";
    if(p1done)
    {
        cout<<"*******************\nPhase1 result genome:\n";
        cout<<p1strand.genome<<endl<<"strength: "<<p1strand.strength<<endl;
        cout<<p1fitcalls<<" fitness calls for p1.\n\n";
    }
    else
        cout<<"*******************\nPhase 1 failed to open.\n\n";
    
    
    if(p3done)
    {
        cout<<"*******************\nPhase 2 result genome:\n";
        cout<<readable2<<endl;
        
      //  cout<<"***breakdown***\n";
        cout<<"strength = "<<p2strand.strength<<endl<<"walls hit: "<<p2strand.wallhit <<endl <<"food found (+20 per): "<<p2strand.food<<endl;
        if(p2strand.dead)
            cout<<"This specimen died.\n";
        else
            cout<<"This specimen did not die.\n";
        cout<<p2fitcalls<<" fitness calls for p2.\n\n";
        
        cout<<"*******************\nPhase 3 result genome:\n";
        cout<<readable<<endl;
       // cout<<"***breakdown***\n";
        cout<<"strength = "<<survivor.strength<<endl<<"walls hit (-1 per): "<<survivor.wallhit<<endl<<"food found (+20 per): "<<survivor.food<<endl;
        if(survivor.dead)
            cout<<"This specimen died.\n";
        else
            cout<<"This specimen did not die.\n";
        
        
        cout<<endl<<"tot fitness calls for p3: "<<p3fitcalls<<endl;
        cout<<"the brute force exhaustive calls for this input would be: 4^"<<f.steps<<
        " = "<<pow(4,f.steps)<<endl;
    }
    else cout <<"Phase 2/3 file failed to open.\n";
}

 

