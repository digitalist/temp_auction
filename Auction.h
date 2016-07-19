//
// Created by user on 7/12/16.
//

#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <boost/log/trivial.hpp>
#include <map>
#include <map>


namespace Auction {


    const int MIN_BANNER_BID_GUARD = -1;
    const int MIN_CAMPAIGN_ID_GUARD = -1;
    const int MIN_REGION_ID_GUARD = -1;


    struct Banner {
        int id;
        int campaign_id;
        long bid;
        std::vector<int> region_ids;

    };


    using std::vector;
    using std::cout;
    using bannerList = vector<Banner>;
    using regionList = vector<int>;
    using bannerMapPrice = std::map<float, Banner>;

    bannerList auction(bannerList &banners, const unsigned int numOfPlaces, const int regionId);

}





