#include "q_player.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <game.h>

using namespace std;

q_player::q_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
    srand (time(NULL));

   // iterations = _iterations;
    this->EXPLORE_RATE_DECAY = 0.001; //(EXPLORE_RATE / iterations) / 3; //      //0.001 best
}
//               SAVE Q TABLE                ///////////////////////////////////////////////////////////////////////
void q_player::save_qtable(std::vector<std::vector<double>> &q_table, std::string filename)
{
    std::stringstream ss;
    std::ofstream output_file(filename.c_str());
    boost::archive::text_oarchive oa(output_file);
    oa << q_table;
}

//               LOAD Q TABLE                ////////////////////////////////////////////////////////////////////////
std::vector<std::vector<double>> q_player::load_qtable(std::string filename)
{
    std::vector<std::vector<double>> q_table;
    std::ifstream input_file(filename.c_str());
    boost::archive::text_iarchive ia(input_file);
    ia >> q_table;
    input_file.close();
    return q_table;
}

//              STATE OF AGENT               ////////////////////////////////////////////////////////////////////////////
std::vector<int> q_player::input_to_state()
{
    int distance2;
    int killed=0;
    int state;
    std::vector<int> states(4);
    for (int i = 0; i < 4; i++) {




           state = pos_start_of_turn[i] + 1;

        // If token moves past goal position
        if (pos_start_of_turn[i] > 57){
            state = 58;

        state += 58 *i;
        }

        states[i] = state;

    }
    return(states);
}

//                ACTIONS                   ///////////////////////////////////////////////////////////////////////////////////
std::vector<int> q_player::get_actions()
{
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int hit = 0;

    for (int i=0; i<4; i++){
        if (pos_start_of_turn[i] == 99){
            count0 += count0;
        }
    }


    for (int j=4; j<8; j++){
        if (pos_start_of_turn[j] == 99){
            count1 +=count1;
        }
    }
    for (int j=8; j<12; j++){
        if (pos_start_of_turn[j] == 99){
            count2 +=count2;
        }
    }

    for (int j=12; j<16; j++){
        if (pos_start_of_turn[j] == 99){
            count3 +=count3;
        }
    }

    if (count1>=count2 && count1>count3 && count0<=count1){
        hit = 1;
    }

    if (count2>count1 && count2>=count3 && count0<=count2){
        hit = 2;
    }

    if (count3>=count1 && count3>count2 && count0<= count3){
        hit = 3;
    }




    std::vector<int> actions;
    for (int i = 0; i < 4; i++) {
        int action = 0;
        int countD =0;
//        int countDN = 0;

        int danger =0;
//        int dangerN = 0;
        int start_pos = pos_start_of_turn[i];
        int new_pos = start_pos + dice_roll;

        for (int j = 4; j < 16; j++){
        if (start_pos-pos_start_of_turn[j] >0 && start_pos-pos_start_of_turn[j] < 8){
            danger = 1;
            countD++;
        }
//        if (new_pos -pos_start_of_turn[j] <0 && new_pos - pos_start_of_turn[j]>-7){
//            dangerN = 1;
//            countDN++;
//        }
    }
        for (int i=1; i<=6; i++){

            if (start_pos == -1 && dice_roll == 6 && danger==1 && countD ==i) {
                action = 12+i;

           }
        }

        // Move out of home
        if (start_pos == -1 && dice_roll == 6 && danger==0) {
            action = 1;
        // Check if token is stuck in home or is in goal
        } else if (start_pos != -1 && start_pos != 99) {
            // Move into goal
            if (new_pos == 56 || new_pos == 50)
                action = 7;

            // Form blockade
            for (int j = 0; j < 4; j++) {

                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==0) {
                    action = 5;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD==1) {
                    action = 10;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD==2) {
                    action = 19;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD==3) {
                    action = 20;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD==4) {
                    action = 21;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD==5) {
                    action = 22;
                    break;
                }
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99 && danger ==1 && countD>=6) {
                    action = 23;
                    break;
                }

            }
            // Move onto a star
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==0) ){
                action = 8;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD==1) ){
                action = 24;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD==2) ){
                action = 25;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD==3) ){
                action = 26;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD==4) ){
                action = 27;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD==5) ){
                action = 28;
            }
            if ((new_pos == 5 || new_pos== 11 || new_pos == 18 || new_pos==24 || new_pos== 31 || new_pos== 37 || new_pos== 44)&& (danger==1)&&(countD>=6) ){
                action = 29;
            }
            // Move onto the star before finish line
            if (new_pos== 50 && danger==0){
                action = 9;
            }

            if (new_pos== 50 && danger==1 && countD==1){          //         here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                action = 42;
            }
            if (new_pos== 50 && danger==1 && countD==2){
                action = 43;
            }
            if (new_pos== 50 && danger==1 && countD==3){
                action = 44;
            }
            if (new_pos== 50 && danger==1 && countD==4){
                action = 45;
            }
            if (new_pos== 50 && danger==1 && countD==5){
                action = 46;
            }
            if (new_pos== 50 && danger==1 && countD>=6){
                action = 12;
            }


            // Move onto same field as an enemy
            bool kill = false;
            bool suicide = false;



            for (int j = 4; j < 16; j++) {        

                if (new_pos == pos_start_of_turn[j] && start_pos != -1 && start_pos != 99) {
                    kill = true;
                    // Check if enemy token is on globe (Suicide)
                    if ((pos_start_of_turn[j] - 8) % 13 == 0 || (pos_start_of_turn[j] - 8) % 18 == 0) {
                        suicide = true;
                        break;
                    }



                    // Check if enemy has formed a blockade (Suicide)
                    for (int k = 0; k < 4; k++) {
                        int index = int(j/4) * k;
                        if (new_pos == pos_start_of_turn[index] && index != j) {
                            suicide = true;
                            break;
                        }
                    }
                }
            }

            // Protect token (goal stretch or globe)

            for (int l=1; l<=6; l++){

                  if ((new_pos > 50 && start_pos < 50) ||
                          ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false) && (danger==1) && (countD ==l) ) {
                       action = 29+l;
                                }
                               }

            if ((new_pos > 50 && start_pos < 50) ||
                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger == 0)) {
                action = 6;
            }

//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)) {
//                action = 10;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD==1)) {
//                action = 30;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD==2)) {
//                action = 31;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD==3)) {
//                action = 32;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD==4)) {
//                action = 33;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD==5)) {
//                action = 34;
//            }
//            if ((new_pos > 50 && start_pos < 50) ||
//                ((new_pos - 8) % 13 == 0 && new_pos < 52 && suicide == false)&& (danger ==1)&& (countD>=6)) {
//                action = 35;
//            }

            if (suicide) { // Suicide
                action = 4;
            } else if (kill && !suicide) { // Kill
                action = 3;
            }

            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0)
                        ))
            {
                action = 11;

            }

            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==1 && countD==1)
                        ))
            {
                action = 36;

            }
            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0&& countD==2)
                        ))
            {
                action = 37;

            }
            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0&& countD==3)
                        ))
            {
                action = 38;

            }
            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0&& countD==4)
                        ))
            {
                action = 39;

            }
            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0&& countD==5)
                        ))
            {
                action = 40;

            }
            if (kill && !suicide && ((
(hit==1 && (new_pos == pos_start_of_turn[4] || new_pos == pos_start_of_turn[5] ||new_pos == pos_start_of_turn[6] || new_pos == pos_start_of_turn[7])) ||
(hit==2 && (new_pos == pos_start_of_turn[8] || new_pos == pos_start_of_turn[9] ||new_pos == pos_start_of_turn[10] || new_pos == pos_start_of_turn[11])) ||
(hit==3 && (new_pos == pos_start_of_turn[12] || new_pos == pos_start_of_turn[13] ||new_pos == pos_start_of_turn[14] || new_pos == pos_start_of_turn[15])) && danger ==0&& countD>=6)
                        ))
            {
                action = 41;

            }

            // Move token
            if (action == 0 && new_pos > start_pos)
                action = 2;


        }
        actions.push_back(action);
    }
    return actions;
}

//                           SELECT ACTION                     //////////////////////////////////////////////////////////////////////////
int q_player::select_action(std::vector<std::vector<double>> q_table,
    std::vector<int> states, std::vector<int> possible_actions)
{
    int best_action = 0;
    if (EXPLORE_RATE == 0 || (double)(rand() % 1000) / 1000.0 > EXPLORE_RATE) {
        double max_q = -10000;
        for (int i = 0; i < 4; i++) {
            if (pos_start_of_turn[i] > 55 || (pos_start_of_turn[i] == -1 && dice_roll != 6))
                continue;

            if (q_table[possible_actions[i]][states[i]] > max_q && possible_actions[i] != 0) {
                max_q = q_table[possible_actions[i]][states[i]];
                best_action = i;
            }
        }
    // Random action
    } else {
        bool token_out_of_home = false;
        for (int i = 0; i < 4; i++) {
            if (pos_start_of_turn[i] != -1 && pos_start_of_turn[i] != 99) {
                token_out_of_home = true;
                break;
            }
        }
        while (true) {
            best_action = rand() % 4;
            if (pos_start_of_turn[best_action] < 56) {
                if (pos_start_of_turn[best_action] != -1 && token_out_of_home) {
                    break;
                } else if (!token_out_of_home) {
                    break;
                }
            }
        }
    }
    // Make sure that best_action is not moving a token in goal
    while(pos_start_of_turn[best_action] == 99) {
        best_action++;
        best_action = best_action % 4;
    }

    return best_action;
}

//                       GET REWARD                            /////////////////////////////////////////////////////////////////////
void q_player::get_reward(std::vector<std::vector<double>> &q_table,
    int action, int state, int decision)
{
    double total_diff = 0;
    static int previous_state = 0;
    static int previous_action = 0;
    static int games_played = 0;
    static int game_counter = 0;
    static int discount_counter = 0;
    static int learning_counter = 0;
    static float winrate = 0.0;
    static float winMax = 0.0;
    static float won =0.0;
    static int winCounter = 0;
    static float oldLR = 0.0;

    double reward = 0;

    // Move out of home
    if (previous_action == 1)
        reward += 0.7; // + 0.25

    // Move piece closest to home
    if (previous_action != 0 && previous_action != 1 && previous_action != 4) {
        bool closest = true;
        for (int i = 0; i < 4; i++) {
            if (pos_end_of_turn[decision] < pos_end_of_turn[i] &&
                decision != i && pos_end_of_turn[i] != 99) {
                    closest = false;
            }
        }
        if (closest && pos_end_of_turn[decision] < 51)  //51

            reward -= 0.1;  // -0.2 for 5000 iterations, all random
}
     // Going into a star
    if (previous_action == 8 || previous_action == 24 || previous_action == 25 || previous_action == 26 || previous_action == 27 || previous_action == 28 || previous_action == 29 )
        reward += 0.5;

    // Going into the goal star
    if (previous_action == 9 || previous_action == 12 || previous_action == 42 || previous_action == 43 || previous_action == 44|| previous_action == 45 || previous_action == 46 )
        reward += 0.7;


    // Kill
    if (previous_action == 3)
        reward += 0.005;   // +0.15    0.005

    // Kill the strongest
    if (previous_action == 11 | previous_action == 36 | previous_action == 37 | previous_action == 38 | previous_action == 39 | previous_action == 40 | previous_action == 41)
        reward += 0.5;   // +0.15    0.005



    // Exit base while in danger
    if (previous_action == 18||previous_action == 13 || previous_action == 14 || previous_action == 15 || previous_action == 16 || previous_action == 17)
        reward += 0.1;   // +0.15    0.005

     //Move forward
//    if (previous_action == 13)   //action =2
//        reward -= 0.1;

    // Form Blockade

    if (previous_action == 5 || previous_action == 10 || previous_action == 19 || previous_action == 20 || previous_action == 21 || previous_action == 22 || previous_action == 23  )
        reward += 0.05;  // + 0.05

    // Protect token
    if (previous_action == 6)
        reward += 0.1;        // +0.2

    // Protect token while danger

    if (previous_action == 30 || previous_action == 31 || previous_action == 32 || previous_action == 33 || previous_action == 34 || previous_action == 35 )
        reward += 0.13;        // +0.2

    // Move into goal
    if (previous_action == 7)
        reward += 0.45;         // +0.25

    // Suicide
    if (previous_action == 4)
        reward -= 0.8;         // -0.8

    bool game_won = true;
    for (int i = 0; i < 4; i++) {
        // Winning the game
        if(pos_end_of_turn[i] != 99)
            game_won = false;

        // Getting a token knocked home
        if (pos_start_of_turn[i] == -1 && pos_end_of_turn[i] != -1)
            reward -= 0.15;    // -0.25
    }

    if (game_won) {
        if (EXPLORE_RATE > 0)
            EXPLORE_RATE -= EXPLORE_RATE_DECAY;
        games_played++;
        game_counter++;
        discount_counter++;
        learning_counter++;
        won++;

        reward += 1.1;   // +1
    } else {
        // Losing the game
        for (int i = 4; i < 16; i++) {
            if(pos_end_of_turn[i] == 99 && pos_start_of_turn[i] == -1) {
                if (EXPLORE_RATE > 0)
                    //EXPLORE_RATE = EXPLORE_RATE;
                    EXPLORE_RATE -= EXPLORE_RATE_DECAY;
                games_played++;
                game_counter++;
                discount_counter++;
                learning_counter++;
                reward -= 0.5;  // -1    -0.8
                break;
            }
        }
    }

    winrate = won/games_played;

//    if (learning_counter == 50000 && LEARNING_RATE> 0.006 ){
//        LEARNING_RATE = LEARNING_RATE - 0.0009;   //0.01
//        learning_counter =0;
//    }

    // if (winrate > winMax && training) {
    //     winMax = winrate;
    //     std::string filename = "../QMAX";
    //     save_qtable(q_table, filename);

    //     winCounter =0;
    //     //LEARNING
    // }
//    else {
//        winCounter++;
//    }

//    if (winCounter>12000){
//       // oldLR = LEARNING_RATE;
//        EXPLORE_RATE = 0.002;
//        //LEARNING_RATE = 0.0001
//        winCounter =0;
//    }


    if (EXPLORE_RATE < 0)
        EXPLORE_RATE = 0;


    // Update q-table
    if (reward != 0) {
        q_table[previous_action][previous_state] +=  LEARNING_RATE *
            (reward + DISCOUNT_FACTOR * q_table[action][state]
             - q_table[previous_action][previous_state]);
    }

    if (game_counter== 4000 && training /*&& games_played<60000*/) {
        EXPLORE_RATE = 0.01;
        game_counter =0;
    }

    if (discount_counter== 5000 && training && DISCOUNT_FACTOR> 0.7) {
        DISCOUNT_FACTOR -= 0.05;
        discount_counter =0;
    }

    previous_state = state;
    previous_action = action;
    static bool game_saved = false;
    static float maxC=0.0;
    //float maxT = maxP;
    // if (games_played == iterations - 1 && !game_saved) {
    //     std::string filename = "../Q" + std::to_string(iterations);
    //     save_qtable(q_table, filename);
    //     game_saved = true;

    // }

//    if (maxT>maxC){
//        std::string filename = "../Q" + std::to_string(iterations);
//        save_qtable(q_table, filename);
//        maxC= maxT;

//    }
}
//             MAKE DECISION                             //////////////////////////////////////////////////////////////////////
int q_player::make_decision()
{
    static bool first_turn = true;
    static std::vector<std::vector<double>> q_table(47, std::vector<double>(232, 0));

    if (first_turn) {
        for (int i = 0; i < 4; i++)
            pos_end_of_turn[i] = -1;

        if (!this->training) {
            q_table = load_qtable("../Q200000.txt");    //    TEST_q_tableLL9000        BOT-64%-41%     Q240000
        }
        first_turn = false;
    }

    std::vector<int> states = input_to_state();
    std::vector<int> possible_actions = get_actions();
    int decision = select_action(q_table, states, possible_actions);

    if (this->training) {
        get_reward(q_table, possible_actions[decision], states[decision], decision);
    }

    return decision;
}



//                START TURN                           /////////////////////////////////////////////////////////////////////////////////
void q_player::start_turn(positions_and_dice relative)
{
    dice_roll = relative.dice;
    pos_start_of_turn = relative.pos;

    int decision = make_decision();
    emit select_piece(decision);
}


//             POST GAME ANALYSIS                     ////////////////////////////////////////////////////////////////////////////////
void q_player::post_game_analysis(std::vector<int> relative_pos)
{
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; i++) {
        if(pos_end_of_turn[i] < 99) {
            game_complete = false;
            break;
        }
    }
    emit turn_complete(game_complete);
}
