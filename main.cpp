#include <iostream>
#include <vector>
#include <boost/log/trivial.hpp>
#include <map>
#include "Auction.h"

using std::vector;
using std::string;
using std::cout;

using Auction::Banner;



int main() {


    enum COUNTRIES{
        NARNIA,
        MORDOR,
        AMBER
    };
    Auction::bannerList bsm{
            {0, 0, 3,  {NARNIA}},
            {1, 1, 3,  {NARNIA}},
            {2, 1, 3,  {NARNIA}},
            {3, 1, 3,  {NARNIA}},

            {4, 2, 3, {NARNIA}},
            {5, 2, 3, {NARNIA}},
            {6, 2, 3, {NARNIA}},
            {6, 2, 3, {MORDOR}},
    };

    auto result = Auction::auction(bsm, 2, NARNIA);


    for (auto& x : result){
        std::cout << x.campaign_id <<", " << x.bid << "\n";
        for (auto& rid : x.region_ids){
            std::cout  << "region" << rid << "\n";
        }
    }
    //в этом тесте можно рэндомизировать кампании и суммы, которые должны получаться
    //но для простоты я предположу, что тестовые данные не меняются и рк с номером 1 не может попасть в выдачу
    //при четырех местах выдачи


    return 0;
}
