//
// Created by user on 7/13/16.
//

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE AuctionTest
#include <boost/test/included/unit_test.hpp>
#include "../Auction.h"

//#include <boost/test/unit_test.hpp>

enum COUNTRIES{
    NARNIA,
    MORDOR,
    AMBER,
    IRON_EMPIRE
};


BOOST_AUTO_TEST_CASE( anyCountryTest )
{
    //auction works on reference, so we better redefine datasets and let them be local
    Auction::bannerList bannersTestDataset{
            {0, 1, 1,  {NARNIA}},
            {1, 2, 2,  {AMBER}},
            {2, 3, 3,  {MORDOR}},
            {3, 4, 4,  {IRON_EMPIRE}},
    };
    //в задании мутное описание параметра "страна", оставлю первый вариант, а не вектор
    auto result = Auction::auction(bannersTestDataset, 4, Auction::MIN_REGION_ID_GUARD);

    for (auto& banner : result){
        std::cout << banner.id << "\n";
    }
    BOOST_CHECK(result.size() == 4);

}

BOOST_AUTO_TEST_CASE( noCountryTest )
{
    //auction works on reference, so we better redefine datasets and let them be local
    Auction::bannerList bannersTestDataset{
            {0, 1, 1,  {NARNIA}},
            {1, 2, 2,  {AMBER}},
            {2, 3, 3,  {MORDOR}},
            {3, 4, 4,  {AMBER}},
    };
    //в задании мутное описание параметра "страна", оставлю первый вариант, а не вектор
    auto result = Auction::auction(bannersTestDataset, 4, IRON_EMPIRE);

    BOOST_CHECK(!result.size());

}

BOOST_AUTO_TEST_CASE( skipCheapBannerTest )
{
    //auction works on reference, so we better redefine datasets and let them be local
    Auction::bannerList bannersTestDataset{
            {0, 0, 0,  {NARNIA}},
            {1, 1, 1,  {NARNIA}},
            {2, 1, 2,  {NARNIA}},
            {3, 1, 3,  {NARNIA}},

            {4, 2, 4,  {NARNIA}},
            {5, 2, 5,  {NARNIA}},
            {6, 2, 6,  {NARNIA}},
            {6, 2, 99, {MORDOR}},
    };


    //all campaigns and banners can pass through country check
    //we have 3 campaigns, 2 ad-places, and campaign_id 0 can't make it because it has too cheap bids
    auto result = Auction::auction(bannersTestDataset, 2, NARNIA);

    for (auto& banner : result){
        BOOST_CHECK(banner.campaign_id > 0);
    }
}


BOOST_AUTO_TEST_CASE( checkWrongCountryTest )
{
    Auction::bannerList bannersTestDataset{
            {0, 0, 0,  {NARNIA}},
            {1, 1, 1,  {NARNIA}},
            {2, 1, 2,  {NARNIA}},
            {3, 1, 3,  {NARNIA}},

            {4, 2, 4,  {NARNIA}},
            {5, 2, 5,  {NARNIA}},
            {6, 2, 6,  {NARNIA}},
            {6, 2, 99, {MORDOR}},
    };
    //banner with a highest bid has a wrong country
    //we can't let MORDOR WIN
    auto result = Auction::auction(bannersTestDataset, 3, NARNIA);

    for (auto& banner : result){
             BOOST_CHECK( find(banner.region_ids.begin(), banner.region_ids.end(), MORDOR) ==
                   banner.region_ids.end() );
    }
}

BOOST_AUTO_TEST_CASE( checkSingleCountryWins )
{

    Auction::bannerList bannersTestDataset{
            {0, 0, 0,  {NARNIA}},
            {1, 1, 1,  {NARNIA}},
            {2, 1, 2,  {NARNIA, NARNIA}},
            {3, 1, 3,  {AMBER}},

            {4, 2, 4,  {NARNIA}},
            {5, 2, 5,  {NARNIA}},
            {6, 2, 6,  {AMBER}},
            {6, 3, 99, {MORDOR}},
    };

    auto result = Auction::auction(bannersTestDataset, 2, MORDOR); //two ad-places but...

        BOOST_CHECK( result.size() == 1 ); //... single country wins
        BOOST_CHECK( result.at(0).region_ids.at(0) == MORDOR );
}


BOOST_AUTO_TEST_CASE( checkCountriesSelected )
{
    //we target for MORDOR & AMBER, NARNIA can whine as long as it wants
    Auction::bannerList bannersTestDataset{
            {0, 0, 0,  {NARNIA}},
            {1, 1, 1,  {NARNIA}},
            {2, 1, 2,  {NARNIA, NARNIA}},
            {3, 1, 3,  {AMBER}},

            {4, 2, 4,  {NARNIA}},
            {5, 2, 5,  {NARNIA}},
            {6, 2, 6,  {AMBER, MORDOR}},
            {7, 3, 6,  {AMBER, MORDOR}},
            {8, 4, 99, {MORDOR}},
    };

    auto result = Auction::auction(bannersTestDataset, 3, MORDOR);

    BOOST_CHECK( result.size() == 3 );
    BOOST_CHECK( result.at(2).region_ids.at(0) == MORDOR ); //mordor bid's higher
    BOOST_CHECK( result.at(0).region_ids.at(0) == AMBER );
}


BOOST_AUTO_TEST_CASE( checkFinalEqualPriceRandomization )
{

    Auction::bannerList bannersTestDataset{
            {0, 1, 7,  {NARNIA}},
            {1, 2, 7,  {NARNIA}},
            {2, 3, 7,  {NARNIA, MORDOR}},
            {3, 4, 7,  {NARNIA}},
            {4, 5, 7,  {NARNIA}},
            {5, 6, 7,  {NARNIA}},
            {6, 7, 7,  {NARNIA, AMBER}},
            {7, 8, 9,  {AMBER, NARNIA}},
            {8, 9, 9,  {NARNIA, AMBER}},
            {9, 9, 9,  {NARNIA, AMBER}},
    };

    auto result = Auction::auction(bannersTestDataset, 9, NARNIA);

    BOOST_CHECK( result.size() == 9 );
    BOOST_CHECK( result.at(0).bid < result.at(8).bid );

}