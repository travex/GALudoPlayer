#include "main_class.h"

main_class::main_class()
{

}
void main_class::get_data(std::vector<double> data)
{
    wins_in += data[0];
    hits_in += data[1];
    stars_in += data[2];
    homes_in += data[3];
    out_in += data[4];
    george += data[5];
}