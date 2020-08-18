/*
 * Program: Musical chairs game with n players and m intervals.
 * Author:  G PRAJWAL and Revanth Rokkam
 * Roll# :  CS18BTECH11010 and CS18BTECH11040
 */

#include <stdlib.h>  /* for exit, atoi */
#include <iostream>  /* for fprintf */
#include <errno.h>   /* for error code eg. E2BIG */
#include <getopt.h>  /* for getopt */
#include <assert.h>  /* for assert */
#include <chrono>   /* for timers */
#include <thread>   /* for threads */
#include <mutex>   /* for locks */
#include <condition_variable>   /* for conditional variables */
#include <vector>  /*for vector */
#include <string.h>
#include <bits/stdc++.h>

using namespace std;
/*
 * Forward declarations
 */
std::mutex mtx;
std::mutex mtx2;
std::condition_variable cv;
std::condition_variable cv2;
int PJ,a[100000],x,y;
std::vector <int> v;
void usage(int argc, char *argv[]);
unsigned long long musical_chairs(int nplayers);

int main(int argc, char *argv[])
{
    int c;
      int nplayers = 0;

    /* Loop through each option (and its's arguments) and populate variables */
    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"help",            no_argument,        0, 'h'},
            {"nplayers",         required_argument,    0, '1'},
            {0,        0,            0,  0 }
        };

        c = getopt_long(argc, argv, "h1:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            cerr << "option " << long_options[option_index].name;
            if (optarg)
                cerr << " with arg " << optarg << endl;
            break;

        case '1':
            nplayers = atoi(optarg);
            break;

        case 'h':
        case '?':
            usage(argc, argv);

        default:
            cerr << "?? getopt returned character code 0%o ??n" << c << endl;
            usage(argc, argv);
        }
 
    }

    if (optind != argc) {
        cerr << "Unexpected arguments.\n";
        usage(argc, argv);
    }


    if (nplayers == 0) {
        cerr << "Invalid nplayers argument." << endl;
        return EXIT_FAILURE;
    }

    unsigned long long game_time;
    game_time = musical_chairs(nplayers);

    cout << "Time taken for the game: " << game_time << " us" << endl;

    exit(EXIT_SUCCESS);
}

/*
 * Show usage of the program
 */
void usage(int argc, char *argv[])
{
    cerr << "Usage:\n";
    cerr << argv[0] << "--nplayers <n>" << endl;
    exit(EXIT_FAILURE);
}

void umpire_main(int nplayers)
{
    /* Add your code here */
    /* read stdin only from umpire */
    char str[20];
    int k;
    for(int i=1;i<nplayers;i++)
    {

        //printf("fuck4");

        PJ = nplayers -i;
        printf("%d %d\n",PJ,i);

        for(int j=0;j<100000;j++)
        {
            a[j]=0;
        }
    
    while(1){
       
        cin>>str;
        
        if (!strcmp(str,"lap_start"))
        {

        }
        else if (!strcmp(str,"music_start"))
        {

        }
        else if(!strcmp(str,"music_stop"))
        {
              cv.notify_all();
              std::unique_lock<std::mutex> lck(mtx2);
              cv2.wait(lck);

        }
        else if(!strcmp(str,"lap_stop"))
        {
          break;
        }
        else if(!strcmp(str,"umpire_sleep"))
        {
             cin>>k;
             this_thread::sleep_for(chrono::microseconds(k));
        }
        else if(!strcmp(str,"player_sleep"))
        {
            cin>> x;
            cin>> y;
            a[x]=y;
        }
        

      }
    }
    return;
}

void player_main(int plid)
{
    /* Add your code here */
    /* synchronize stdouts coming from multiple players */
   this_thread::sleep_for(chrono::microseconds(10));
   if(a[plid])
   {
     this_thread::sleep_for(chrono::microseconds(a[plid]));
    // printf("fuck7");
   }
   

   while(1)
   {
    std::unique_lock<std::mutex> lck(mtx);
      if(PJ>0)
      {
        PJ--;
        cv.wait(lck);
      }
      else
      {
        v.push_back(plid);
        cv2.notify_all();
        return;
      }
   }

    return;
}

unsigned long long musical_chairs(int nplayers)
{
    auto t1 = chrono::steady_clock::now();
    //printf("fuck");
    // Spawn umpire thread.
    std::thread thread_obj(umpire_main, nplayers);
    /* Add your code here */
    //printf("fuck2");
    std::thread t[nplayers];
    for(int i=0;i<nplayers;i++)
    {
       // printf("fuck3");
        t[i]=std::thread(player_main, i);
    }
    // Spawn n player threads.
    /* Add your code here */
    thread_obj.join();
     for (auto& th : t) th.join();
    
    cout<< "Musical Chairs: " << nplayers << " game with " << nplayers-1 <<" laps.\n";
        for(int i=0;i<nplayers-1;i++)
        {
            cout<<"======= lap# "<<i+1<<" =======\n";
            cout<<v[i]<<" could not get chair\n";
            cout<<"**********************\n";
        }
     cout<<"Winner is "<<v[nplayers-1]<<"\n";   
    
    auto t2 = chrono::steady_clock::now();

    auto d1 = chrono::duration_cast<chrono::microseconds>(t2 - t1);

    return d1.count();
}
