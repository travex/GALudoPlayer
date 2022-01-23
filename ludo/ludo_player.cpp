#include "ludo_player.h"

#include <random>
#include <iostream>
#include <algorithm>

using namespace std;
//Neural Network OVER HERE
ludo_player::ludo_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
    weight_ann.resize(6);
}

int ludo_player::make_decision(){
    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

void ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    // cout << "New position! Dice is " << relative.dice << endl;
    // for(unsigned int i =0;i<relative.pos.size();i++)
    // {
    //     cout << "Positions: " << relative.pos[i] << endl;
    //     // if(relative.pos[i] >= 66 && relative.pos[i] <90/* relative.pos[i]==12*/)
    //     // {
    //     //     int k;
    //     //     cin  >> k;
    //     // }
    // }
    
    dice_roll = relative.dice;
    
    //cout << "Color in ludo class is:" << g->color << endl;
    bool star,canstart,hitopp, cangoal,danja,safe, in_danja,glober;
    std::vector<int> value;
    for(unsigned int i =1; i<5;i++)
    {
        if(relative.pos[i-1]==99)
        {
            value.push_back(-100000);
        }
        else
        {
            std::vector<double> Ann_signals;
        
            star = hit_Star(relative, i);
            //cout << "Piece " << i << " can star? " << star << endl;
            canstart = can_Start(relative,i);
            //cout << "Piece " << i << " can start? " << canstart << endl;
            hitopp = hit_opp(relative,i);
            //cout << "Piece " << i << " can hit opp? " << hitopp << endl;
            cangoal = can_goal(relative,i);
            //cout << "Piece " << i << " can goal? " << cangoal << endl;
            danja = enter_danger_zone(relative,i);
            //cout << "Piece " << i << " in danger? " << danja << endl;
            safe = enter_safe_zone(relative,i);
            //cout << "Piece " << i << " is safe? " << safe << endl;
            in_danja = is_danger_zone(relative,i);
            glober = is_on_globe(relative,i);

            // if(star || canstart || hitopp || cangoal || danja || safe)
            // {
            //     cout << canstart << endl;
            //     int k;
            //     cin >> k;
            // }
            if(star){ star_signal = 1;} else {star_signal = 0;}
            if(canstart){signal_start = 1;}else {signal_start = 0;}
            if(hitopp){hit_signal=1;}else{hit_signal=0;}
            if(cangoal){goal_signal=1;}else{goal_signal=0;}
            if(danja){danger_signal=-1;}else{danger_signal=0;}
            if(safe){safe_signal=1;}else{safe_signal=0;}
            if(in_danja){in_danger_signal=1;}else{in_danger_signal=0;}
            if(glober){globe_signal=-1;}else{globe_signal=0;}
            

            Ann_signals.push_back(signal_start);
            Ann_signals.push_back(goal_signal);
            Ann_signals.push_back(hit_signal);
            Ann_signals.push_back(star_signal);
            Ann_signals.push_back(danger_signal);
            Ann_signals.push_back(safe_signal);
            Ann_signals.push_back(in_danger_signal);
            Ann_signals.push_back(globe_signal);
        
            value.push_back(ANN(Ann_signals,weight_ann));
            Ann_signals.clear();
            }
        }
        
        
    int minPos = 0, maxPos = 0;

    for (unsigned i = 0; i < value.size(); ++i)
    {
      
        if (value[i] > value[maxPos]) // Found a bigger max
           maxPos = i; 

    }
    int decision = maxPos;
   // cout << "Values size " << value.size() <<endl;
    //cout << "Piece " << decision << endl;
    value.clear();
    emit select_piece(decision);
    
}

void ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    
    emit turn_complete(game_complete);
}
bool ludo_player::hit_Star(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    int index;
    if(m_pos >= 0 && m_pos <51)
    {
        index = m_pos + relative.dice;   
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        index = 0;
    }
    else
    {
        index = -1;
        return false;
    }
    

    if(index == 5  ||
       index == 18 ||
       index == 31 ||
       index == 44){
        return true;
    } else if(index == 11 ||
              index == 24 ||
              index == 37 ||
              index == 50){
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
bool ludo_player::can_Start(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
   // int mf_pos;
    if(m_pos >= 0 && m_pos <51)
    {
       // mf_pos = m_pos + relative.dice;   
        return false;
    }
    else if(m_pos == -1 && relative.dice==6)
    {
       // mf_pos = 0;
        return true;
    }
    else
    {
       // mf_pos = -1;
        return false;
    }
    return false;
}
bool ludo_player::hit_opp(positions_and_dice relative, int piece)
{

    int m_pos = relative.pos[piece-1];
    int mf_pos;
    if(m_pos >= 0 && m_pos <51)
    {
        mf_pos = m_pos + relative.dice;   
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        mf_pos = 0;
    }
    else
    {
        mf_pos = -1;
        return false;
    }
    
    int pos2jump = g->isStar(mf_pos);
    if(pos2jump)
    {
        if(pos2jump + mf_pos == 57){
            mf_pos = 56;
        } else {
            mf_pos += pos2jump;
        }
    }

    for(unsigned int i=4;i<relative.pos.size();i++)
    {
        if(relative.pos[i]>=0 && relative.pos[i]<51 && mf_pos<51)
        {
            if(mf_pos == relative.pos[i])
            {
                return true;
            }
        }
    }
    return false;

}
bool ludo_player::can_goal(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    int mf_pos;
    if(m_pos >= 0 && m_pos <56)
    {
        mf_pos = m_pos + relative.dice;   
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        mf_pos = 0;
        return false;
    }
    else
    {
        mf_pos = -1;
        return false;
    }

    if(mf_pos == 56)
    {
        return true;
    }
    else if(mf_pos == 50) // LAST STAR
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
bool ludo_player::enter_danger_zone(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    int mf_pos;
    if(m_pos >= 0 && m_pos < 51)
    {
        mf_pos = m_pos + relative.dice;   
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        mf_pos = 0;
        return false;
    }
    else
    {
        mf_pos = -1;
        return false;
    }
    int pos2jump = g->isStar(mf_pos);
    if(pos2jump)
    {
        if(pos2jump + mf_pos == 57){
            mf_pos = 56;
        } else {
            mf_pos += pos2jump;
        }
    }
    if(mf_pos < 52){     //check only the indexes on the board, not in the home streak
        if(mf_pos % 13 == 0 || (mf_pos - 8) % 13 == 0)
        {
            if(isOccupied(relative,0,mf_pos))
            {
                return true;
            }
        }
        else if(isOccupied(relative,0,mf_pos)>1)
        {
            return true;
        } 
    }
    for(unsigned int i=4;i<relative.pos.size();i++)
    {
        if(relative.pos[i]>=0 && relative.pos[i]<51 && mf_pos < 51)
        {
            
            if(pos2jump)
            {
                int tempPos = mf_pos-pos2jump;
                if(tempPos <= relative.pos[i]+6 && tempPos > relative.pos[i])
                {
                    return true;
                }

            }
            else
            {
                for(unsigned int l=4;l<relative.pos.size();l++)
                {
                    if(relative.pos[i]>46 && relative.pos[i]<52 && mf_pos < 5 && mf_pos > 0 && mf_pos+52 <=relative.pos[i]+6)
                    {
                        return true;
                    }
                    else if(relative.pos[i]>=44 && relative.pos[i]<52 && mf_pos == 5)
                    {
                        return true;
                    }
                }

                if(mf_pos <= relative.pos[i]+6 && mf_pos > relative.pos[i])
                {
                    return true;
                }
            }
            
            
        }
    }
    return false;

}
bool ludo_player::enter_safe_zone(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    int mf_pos;
    if(m_pos >= 0 && m_pos <51)
    {
        mf_pos = m_pos + relative.dice;   
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        mf_pos = 0;
    }
    else
    {
        mf_pos = -1;
        return false;
    }

    int pos2jump = g->isStar(mf_pos);
    if(pos2jump)
    {
        if(pos2jump + mf_pos == 57){
            mf_pos = 56;
        } else {
            mf_pos += pos2jump;
        }
    }

    if(mf_pos>50 && mf_pos<56)
    {
        return true;
    }

    for(unsigned int i=0;i<4;i++)
    {
        if(mf_pos == relative.pos[i])
        {
            return true;
        }
    }

    if(mf_pos < 52){     //check only the indexes on the board, not in the home streak
        if(mf_pos % 13 == 0 || (mf_pos - 8) % 13 == 0){  //if more people of the same team stand on the same spot it counts as globe
            return true;
        }
    }

   
    return false;

}
bool ludo_player::is_danger_zone(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    if(m_pos >= 0 && m_pos < 51)
    {
         m_pos = relative.pos[piece-1];
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        m_pos = 0;
        return false;
    }
    else
    {
        m_pos = -1;
        return false;
    }
    int pos2jump = g->isStar(m_pos);
   

    for(unsigned int i=4;i<relative.pos.size();i++)
    {
        if(relative.pos[i]>=0 && relative.pos[i]<51 && m_pos < 51)
        {
            
            if(pos2jump)
            {
                int tempPos = m_pos-pos2jump;
                if(tempPos <= relative.pos[i]+6 && tempPos > relative.pos[i])
                {
                    return true;
                }

            }
            else
            {
                for(unsigned int l=4;l<relative.pos.size();l++)
                {
                    if(relative.pos[i]>46 && relative.pos[i]<52 && m_pos < 5 && m_pos > 0 && m_pos+52 <=relative.pos[i]+6)
                    {
                        return true;
                    }
                    else if(relative.pos[i]>=44 && relative.pos[i]<52 && m_pos == 5)
                    {
                        return true;
                    }
                }

                if(m_pos <= relative.pos[i]+6 && m_pos > relative.pos[i])
                {
                    return true;
                }
            }
            
            
        }
    }
    return false;

}
bool ludo_player::is_on_globe(positions_and_dice relative, int piece)
{
    int m_pos = relative.pos[piece-1];
    if(m_pos >= 0 && m_pos <51)
    {
       m_pos = relative.pos[piece-1];
    }
    else if(m_pos == -1 && relative.dice==6)
    {
        m_pos = 0;
    }
    else
    {
        m_pos = -1;
        return false;
    }

    int pos2jump = g->isStar(m_pos);
    for(unsigned int i=0;i<4;i++)
    {
        if(m_pos == relative.pos[i])
        {
            return true;
        }
    }

    if(m_pos < 52){     //check only the indexes on the board, not in the home streak
        if(m_pos % 13 == 0 || (m_pos - 8) % 13 == 0){  //if more people of the same team stand on the same spot it counts as globe
            return true;
        }
    }
    return false;
}
int ludo_player::isOccupied(positions_and_dice relative, int color, int index){ //returns number of people of another color
    int number_of_people = 0;

    if(index != 99){
        for(size_t i = 0; i < relative.pos.size(); ++i){
            if(i < static_cast<size_t>(color)*4 || i >= static_cast<size_t>(color)*4 + 4){        //Disregard own players
                if(relative.pos[i] == index){
                    ++number_of_people;
                }
            }
        }
    }
    return number_of_people;
}
void ludo_player::set_ann_weights(std::vector<double> weights)
{
    weight_ann.clear();
    weight_ann.assign(weights.begin(),weights.end());
    // cout << "Weights set to: ";
    // for(unsigned int i =0;i<weight_ann.size();i++)
    // {
    //     cout << weight_ann[i] << " " ;
    // }
    // cout << endl;

    
}

double ludo_player::ANN(std::vector<double> ann_signals,std::vector<double> ann_weights)
{
      double temp=0.0;
    // for(unsigned int i=0;i<ann_signals.size();i++)
    // {
    //     temp += ann_signals[i]*ann_weights[i];
    // }
   // cout << "Value is: " << temp << endl;
    
    double node1,node2,node3,node4,node5,node6,node7,node8,node9,node10,node11,node12;
    node1 = ann_signals[0]*ann_weights[0] + ann_signals[1]*ann_weights[1] + ann_signals[2]*ann_weights[2] + ann_signals[3]*ann_weights[3] + ann_signals[4]*ann_weights[4] + ann_signals[5]*ann_weights[5]+ ann_signals[6]*ann_weights[30]+ ann_signals[7]*ann_weights[31];
    node2 = ann_signals[0]*ann_weights[6] + ann_signals[2]*ann_weights[7] + ann_signals[5]*ann_weights[8];
    node3 = ann_signals[3]*ann_weights[9] + ann_signals[2]*ann_weights[10]; 
    node4 = ann_signals[4]*ann_weights[11] + ann_signals[2]*ann_weights[12];
    node5 = ann_signals[3]*ann_weights[13] + ann_signals[4]*ann_weights[14];
    node6 = ann_signals[2]*ann_weights[15] + ann_signals[3]*ann_weights[16] + ann_signals[4]*ann_weights[17];
    node7 = ann_signals[4]*ann_weights[18] + ann_signals[5]*ann_weights[19];
    node8 = ann_signals[3]*ann_weights[20] + ann_signals[1]*ann_weights[21];    
    node9 = ann_signals[6]*ann_weights[32] + ann_signals[7]*ann_weights[33];
    node10 = ann_signals[6]*ann_weights[34] + ann_signals[5]*ann_weights[35];
    node11 = ann_signals[6]*ann_weights[36] + ann_signals[2]*ann_weights[37];
    node12 = ann_signals[6]*ann_weights[38] + ann_signals[3]*ann_weights[39];
    temp = node1*ann_weights[22] + node2*ann_weights[23] + node3*ann_weights[24] + node4*ann_weights[25] + node5*ann_weights[26] + node6*ann_weights[27] + node7*ann_weights[28] + node8*ann_weights[29] + node9*ann_weights[40] + node10*ann_weights[41] + node11*ann_weights[42] + node12*ann_weights[43];

    

    return temp;
}