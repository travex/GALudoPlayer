#ifndef LUDO_PLAYER_H
#define LUDO_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <vector>
#include "game.h"
#include <algorithm>
using namespace std;

class ludo_player : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    double star_signal,signal_start,hit_signal,goal_signal,danger_signal,safe_signal,in_danger_signal,globe_signal;
    std::vector<bool> piece1,piece2,piece3,piece4;
    int make_decision();
    bool hit_Star(positions_and_dice relative, int piece);
    bool can_Start(positions_and_dice relative, int piece);
    bool hit_opp(positions_and_dice relative, int piece);
    bool can_goal(positions_and_dice relative, int piece);
    bool enter_danger_zone(positions_and_dice relative, int piece);
    bool enter_safe_zone(positions_and_dice relative, int piece);
    bool is_danger_zone(positions_and_dice relative, int piece);
    bool is_on_globe(positions_and_dice relative, int piece);
    int isOccupied(positions_and_dice relative, int color, int index);
    double ANN(std::vector<double> ann_signals,std::vector<double> ann_weights);
    
    vector<double> weight_ann;
    
    game* g;
public:
    ludo_player();
    void set_ann_weights(std::vector<double> weights);
    
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_H
