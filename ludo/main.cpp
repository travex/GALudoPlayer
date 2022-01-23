#include "dialog.h"
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "ludo_player_random.h"
#include "positions_and_dice.h"
#include <algorithm>
#include <stdlib.h> 
#include <fstream>
#include <iostream> 
#include "ludo_player_a.h"
#include "main_class.h"
#include <cstring>
#include "q_player.hpp"
#define ITER 10000
#define POPULATION 20
#define GENERATIONS 150

using namespace std;
std::vector<std::vector<double>> fits_list;
std::vector<std::vector<double>> weights_list;
std::vector<std::vector<double>> fittmax;
std::vector<double> fits_incoming;
int hits_in,wins_in;
const string filename = "alltrain_vs_2q";
Q_DECLARE_METATYPE( positions_and_dice )

// GA fitness function, selection, crossover and mutation over here!!!
void save_data(string filepath, double d1,double d2,double d3);
void save_weights(string filepath, std::vector<double> save_weights);
double fitness(double hits, double wins,double stars, double homes, double start);
std::vector<int> selection(std::vector<std::vector<double>> fits_in);
std::vector<std::vector<double>> crossover_and_mutate(std::vector<std::vector<double>> weights_in,std::vector<std::vector<double>> fits_in);
double generation_number =1.0;
double fRand(double fMin, double fMax);

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();
    srand(3);
    //instanciate the players here
    ludo_player p1,p2;
   // ludo_player_random p2;
  //  ludo_player_a p3;
    q_player p3,p4; 

    game g;
    g.setGameDelay(0); //if you want to see the game, set a delay
    main_class m_class;
   // cout << "initailize" << endl;
    /* Add a GUI <-- remove the '/' to uncomment block
    Dialog w;
    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
    QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
    QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
    QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    w.show();
    /*/ //Or don't add the GUI
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    //*/

    //set up for each player
    QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));
    
    QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    
    //std::vector<double> weights = {0.0,0.0,0.0,0.0,0.0,0.0};
    weights_list.resize(POPULATION);
    // fittmax.resize(20);
    std::vector<double> aggro_train = {7.2,4.1,6.1,7.6,3.4,5.3,5.8,8.9,3.4,3.4,4.3,1.1,4.7,7,5.6,9.5,2.2,4.3,6,4.5,6.5,4.8,-0.4,2.2,2.1,1.7,-0.2,0.3,-0.5,1.1,0.5,1.9,0.5,-0.1,1.9,1.5,-2.77556e-17,0.2,2.5,0.2,-0.7,0.7,1.1,1};
    std::vector<double> random_train = {9.3,3.7,8.4,5.8,6.6,4.6,4.3,6.1,7.8,6.2,5.9,7,4,5.9,8.6,3,7.4,8.6,9.8,3.7,8.8,9,1.5,1.1,3.1,0.9,2.6,3.3,2.7,2.8,1.1,-0.5,1.9,-0.3,3.1,3.7,2.6,1.8,3.1,2,1.7,0.1,1,1.2};
    std::vector<double> q_train = {6.5,4.3,5.8,5.4,7.1,4.1,10.2,6.9,3.7,6,7.3,10.4,3.7,3.8,5.6,4.6,4.4,6.8,7.3,3.7,4.5,4.2,-1.1,2.3,-0.1,3.3,1.4,1.5,-0.1,2.2,2.4,2.9,0.4,0.9,1.9,1.6,0.3,1.7,0.4,1.3,1.2,1.5,0.3,2.2};
    std::vector<double> all_train = {5.4,3.2,7.4,4.3,7.2,5.7,6.8,4.1,2.8,8.2,4.2,6.4,5.7,4.6,6.2,4,7.7,3.3,6.3,0.5,9.6,2.5,1.7,0.7,1,0.7,1.6,1.5,3.6,2.6,1.3,0.2,0.8,0.3,1,0.7,1.2,2.1,0.2,1.6,2.1,0.2,-0.4,0.1};
   // cout << "initialize3" << endl;
  // double rand_db;
    // for(unsigned int k =0;k<POPULATION;k++)
    // {
    //     for(unsigned int d=0;d<44;d++)
    //     {
    //        // rand_db = fRand(0.0,10.0);
    //        // cout << rand_db << endl;
    //         weights_list[k].push_back(0.0);
    //        // fittmax[k].push_back(0.0);
    //     }
       
        
    //  }
   // cout << "initalized2" << endl;
    //  for(unsigned int v = 0; v < GENERATIONS; ++v){
    //      for(unsigned int b=0;b<POPULATION;b++)
    //      {
    //         // cout << "Weights set to: ";
    //         //     for(unsigned int i =0;i<weights_list[b].size();i++)
    //         //     {
    //         //         cout << weights_list[b].at(i) << " " ;
                    
    //         //     }
    //         //     if(b>45)
    //         //         {
    //         //             int some;
    //         //             cin >> some;
    //         //         }
    //         //     cout << endl;
             p1.set_ann_weights(all_train);
    //         // p2.set_ann_weights(fittmax[b]);
    //         // p3.set_ann_weights(fittmax[b]);
    //         // p4.set_ann_weights(fittmax[b]);
    //         cout << "weights set"<< endl;
            for(unsigned int c=0;c<ITER;c++)
            {
                //p1.set_ann_weights(aggro_train_30);
                //cout << "weights set again" << endl;
                g.start();
                //cout << "game over" << endl;
                a.exec();
                m_class.get_data(g.getData());
               // cout << "Got data" << endl;
                g.reset();
                // cout << "Hits: " << m_class.hits_in;
                // cout << "stars: " << m_class.stars_in;
                // cout << "Homes: " << m_class.homes_in;
                // cout << "Starts: " << m_class.out_in;
                // cout << " Wins: "<< m_class.wins_in << endl;
             }
           cout << "Winrate Hemanth: " << m_class.wins_in << "/" << ITER << " Winrate George: " << m_class.george << "/" << ITER << endl;
           
            double fitsss = fitness(m_class.hits_in,m_class.wins_in,m_class.stars_in,m_class.homes_in,m_class.out_in);
            save_data("../Matlab/fittest_individual_"+filename+".m",fitsss,m_class.wins_in,0.0);
    //         fits_incoming.push_back(m_class.hits_in);
    //         fits_incoming.push_back(m_class.wins_in);
    //         fits_incoming.push_back(m_class.stars_in);
    //         fits_incoming.push_back(m_class.homes_in);
    //         fits_incoming.push_back(m_class.out_in);
    //         fits_list.push_back(fits_incoming);
    //         fits_incoming.clear();
    //          m_class.hits_in = 0.0;
    //          m_class.wins_in = 0.0;
    //          m_class.stars_in = 0.0;
    //          m_class.homes_in = 0.0;
    //          m_class.out_in = 0.0;
            
    //      }
    //     std::vector<std::vector<double>> tempstuff;
    //    // cout << "crossover starts" << endl;
    //     tempstuff = crossover_and_mutate(weights_list, fits_list);
    //     //  cout << "crossover done" << endl;
    //     weights_list.clear();
    //     weights_list = tempstuff;
    //     tempstuff.clear();
    //     cout << "Generation number " << v << endl;
    //    // cout << "cross+ mutate done" << endl;
    //     fits_list.clear();
        
    //  }
    
    
    

    return 0;
}
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
double fitness(double hits, double wins,double stars, double homes, double start)
{
    double points = (0.2*hits) + (10*wins) + (0.1*stars) +(-0.3*homes) + (0.5*start);
    return points;
}

std::vector<int> selection(std::vector<std::vector<double>> fits_in,std::vector<std::vector<double>> in_weight)
{
    size_t pop_num = fits_in.size();
    //size_t fit_num = fits_in[0].size();
    std::vector<double> fitness_individuals;
    std::vector<double> temp_fit;
    std::vector<int> fit_index;
    std::vector<int> selection_index;
   // cout << "Size of population: " << pop_num << " Size of fitness vars: " << fit_num << endl;
    
    for(unsigned int i=0;i<pop_num;i++)
    {
        double fits = fitness(fits_in[i].at(0),fits_in[i].at(1),fits_in[i].at(2),fits_in[i].at(3),fits_in[i].at(4));
        fitness_individuals.push_back(fits);
    }
   //cout << "Fitness obtained" ;
    double max = *std::max_element(fitness_individuals.begin(),fitness_individuals.end());
    std::vector<double>::iterator mit = std::find(fitness_individuals.begin(),fitness_individuals.end(),max);
    save_data("../Matlab/fittest_individual_"+filename+".m",max,fits_in[mit-fitness_individuals.begin()].at(1),generation_number);
    std::vector<double> temp = in_weight[mit-fitness_individuals.begin()];
    // fittmax.clear();
    // for(unsigned int k =0;k<POPULATION;k++)
    // {
    //     fittmax.push_back(temp);
    // }
    save_weights("../Matlab/fittest_weight_"+filename+".m",temp);
    generation_number += 1.0;
    temp_fit = fitness_individuals;
    std::sort(temp_fit.begin(),temp_fit.end());
    
    int half_vec = (fitness_individuals.size()/2) -1; 
   // cout << "Half size fitness vec: " << half_vec << endl;
    unsigned int nums =0;
    while(nums>fitness_individuals.size()/2 || nums ==0)
    {
    for(unsigned int j =0;j<fitness_individuals.size();j++)
    {
        if(fitness_individuals[j]>=temp_fit[half_vec])
        {

            selection_index.push_back(1);
            nums = nums+1;
        }
        else
        {
            selection_index.push_back(0);
        }
        
        
    }
    if(nums>(fitness_individuals.size()/2))
    {
        std::vector<double>::iterator it = std::find(fitness_individuals.begin(),fitness_individuals.end(),temp_fit[half_vec]);
       // cout << "Actual it: " << *it <<" Presumed it: " << fitness_individuals[it-fitness_individuals.begin()] << endl;
        
        fitness_individuals[it-fitness_individuals.begin()] = 0;
        nums=0;
        selection_index.clear();
    }
    
    }
    // cout << selection_index.size() << endl;
    // for(int i=0;i<selection_index.size();i++)
    // {
    //     cout << "sele" << selection_index[i] << endl;
    // }
    
    return selection_index;
    
}

std::vector<std::vector<double>> crossover_and_mutate(std::vector<std::vector<double>> weights_in,std::vector<std::vector<double>> fits_in)
{
    std::vector<std::vector<double>> temp_w_list,next_w_list,mutate_list,final_w_list;
    std::vector<double> w_list1,w_list2,m_list1,temp_list;
    std::vector<int> temp_f_list;
    cout << "Selection" << endl;
    temp_f_list = selection(fits_in,weights_in);
     cout << "Selection over" << endl;
    for(unsigned int k =0; k<temp_f_list.size();k++)
    {
        if(temp_f_list[k]==1)
        {
            temp_w_list.push_back(weights_in[k]);
        }
    }
    
    int half_pop_size = temp_w_list.size()/2;
    
    temp_list = temp_w_list[0];
    //cout << temp_list[0] << endl;
    int half_weight_size = temp_list.size()/2;
    
    w_list1.resize(half_weight_size*2);
    w_list2.resize(half_weight_size*2);
    next_w_list.resize(half_pop_size*2);
     cout << "first init" << endl;
    for(unsigned int i = 0;i<half_pop_size;i++)
    {
        for(unsigned int j=0;j<half_weight_size;j++)
        {
            
            w_list1[j]=temp_w_list[i+half_pop_size].at(j);
            
            w_list1[j+half_weight_size] = temp_w_list[i].at(j+half_weight_size);
           
            w_list2[j]=temp_w_list[i].at(j);
            w_list2[j]=temp_w_list[i+half_pop_size].at(j+half_weight_size);
            
        }

        next_w_list[i] = w_list1;
        next_w_list[i+half_pop_size] = w_list2;
        

    }
     cout << "Crossover done" << endl;
    int ran_num;
    mutate_list.resize(half_pop_size);
    m_list1.resize(half_weight_size*2);
    std::vector<double> mutation = {-0.3,-0.2,-0.1,0.1,0.2,0.3,0.4,0.5,0.6,0.7};
    int ran_num2;
     cout << "2nd init" << endl;
    for(int l =0; l<half_pop_size;l++)
    {
        for(unsigned int m =0;m<next_w_list[l].size();m++)
        {
            ran_num = rand() % 100 + 1;
            if(ran_num<=40)
            {
                ran_num2 = rand() % 9;
                m_list1[m] = next_w_list[l].at(m)+mutation.at(ran_num2);
                // int some;
                // cin >> some;
                
               
            }
            else
            {
                m_list1[m] = next_w_list[l].at(m);
            }
            
            
        }
        mutate_list[l] = m_list1;
    }
     cout << "Mutation done" << endl;
    for(unsigned int k =0; k<temp_f_list.size();k++)
    {
        if(temp_f_list[k]==1)
        {
            final_w_list.push_back(weights_in[k]);
        }
    }
    for(int k =half_pop_size; k<next_w_list.size();k++)
    {
        final_w_list.push_back(next_w_list[k]);
    }
    for(unsigned int k =0; k<mutate_list.size();k++)
    {
        final_w_list.push_back(mutate_list[k]);
    }
    

    return final_w_list;


}
void save_data(string filepath, double d1,double d2,double d3)
{
    std::ofstream file;
    //can't enable exception now because of gcc bug that raises ios_base::failure with useless message
    //file.exceptions(file.exceptions() | std::ios::failbit);
    file.open(filepath, std::ios::out | std::ios::app);
    if (file.fail())
        throw std::ios_base::failure(std::strerror(errno));

    //make sure write fails with exception if something is wrong
    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

    file.seekp(0,ios::end);
    size_t size = file.tellp();
    if(size==0){

      cout<< "File: " << filepath << " wasn't initialized, initializing now." << endl;
      file  << " Data =[" ;
    }

    file  << d1 << "," <<  d2 << "," <<  d3 << ";"  << "\n";
}
void save_weights(string filepath, std::vector<double> save_weights)
{
    std::ofstream file;
    //can't enable exception now because of gcc bug that raises ios_base::failure with useless message
    //file.exceptions(file.exceptions() | std::ios::failbit);
    file.open(filepath, std::ios::out | std::ios::app);
    if (file.fail())
        throw std::ios_base::failure(std::strerror(errno));

    //make sure write fails with exception if something is wrong
    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

    file.seekp(0,ios::end);
    size_t size = file.tellp();
    if(size==0){

      cout<< "File: " << filepath << " wasn't initialized, initializing now." << endl;
      file  << " Data =[" ;
    }

    for(unsigned int i =0;i<save_weights.size();i++)
    {
        if(i==(save_weights.size()-1))
        {
            file << save_weights[i] << ";"  << "\n";
        }
        else
        {
            file << save_weights[i] << "," ;
        }
       
                   
    }
    
}