//
// Created by user on 7/12/16.
//



#include "Auction.h"

namespace Auction {

    using std::begin;
    using std::end;
    void processCampaignBanners(std::map<int, bannerList> &campaignBanners, bannerList &banners,
                                unsigned int numOfPlaces);

    bannerList randomizeBannerList(std::map<int, bannerList> &campaignBanners, unsigned int numOfPlaces);

    bannerList auction(bannerList &banners, const unsigned int numOfPlaces, const int regionId) {
//  check  numOfPlaces
        if (!numOfPlaces) {
            //throw exception, return empty vector/array, log & return?
        }

//  remove bad regions, if we have them
        if(regionId != MIN_REGION_ID_GUARD){
            banners.erase( // throw away wrong regions
                    std::remove_if(banners.begin(), banners.end(),
                                   [&regionId](Banner banner) {
                                       return find(banner.region_ids.begin(), banner.region_ids.end(), regionId) ==
                                              banner.region_ids.end();
                                   }
                    ),
                    banners.end());
        }


//  sort by bid
        std::sort(banners.begin(), banners.end(), [](Banner &a, Banner &b) {
                      return (a.bid > b.bid);
                  }
        );


//  REAL WORK GOES HERE: PROCESS BANNERS
        std::map<int, bannerList> campaignBanners;
        processCampaignBanners(campaignBanners, banners, numOfPlaces);


        auto k = randomizeBannerList(campaignBanners, numOfPlaces);


        return randomizeBannerList(campaignBanners, numOfPlaces);
    }

    void processCampaignBanners(std::map<int, bannerList> &campaignBanners, bannerList &banners,
                                unsigned int numOfPlaces) {
        unsigned int bannerCounter = 0;
        long lastBid = MIN_BANNER_BID_GUARD; //let's pretend we have system banners with zero price

        for (auto &banner: banners) {
            //if we have enough banners, check if next banner hasn't smaller bid than previous
            //if it has, it means this banner and others in line have no chance to participate in this auction
            //so, break the loop
            //otherwise just push till the end
            if (campaignBanners.size() >= numOfPlaces) {
                if (bannerCounter >= numOfPlaces && lastBid < banner.bid) {
                    break;
                }
            } else {
                ++bannerCounter;
            }

//make sure equal priced banners from single campaign make it to randomizer
            if (campaignBanners[banner.campaign_id].size() //@todo: optimize here: just push first banner before FOR LOOP from banners and remove this check
                && campaignBanners[banner.campaign_id].back().bid == banner.bid) { //compare previous bid with current
                campaignBanners[banner.campaign_id].push_back(banner); //push banner by value, is it efficient?
            }

//add at least a single banner from every campaign (to a limit of numOfPlaces)
            else if (!campaignBanners[banner.campaign_id].size()) {
                campaignBanners[banner.campaign_id].push_back(banner);
            }
        }

        //random_shuffle is deprecated in c++14
        std::random_device rng;
        std::mt19937 myRng(rng());

        for (auto kv : campaignBanners) {
            std::shuffle(campaignBanners[kv.first].begin(), campaignBanners[kv.first].end(), myRng);
        }

    }

    bannerList randomizeBannerList(std::map<int, bannerList> &campaignBanners, unsigned int numOfPlaces) {
        std::map<float, std::vector<Banner>> bidBannerIdRandomize;

        bannerList outputBanners;
        outputBanners.reserve(numOfPlaces);

        long tempBid = MIN_BANNER_BID_GUARD;
        int tempCampaignId = MIN_CAMPAIGN_ID_GUARD;

        //random_shuffle is deprecated in c++14
        std::random_device rng;
        std::mt19937 myRng(rng());

        //we could do this with iterators, and it could be much better,
        //because we can check for .end() and randomize without another cycle
        for (auto kv : campaignBanners) {
            //outputBanners.push_back(campaignBanners[kv.first].front());
            auto bannerBid = campaignBanners[kv.first].front().bid;
            auto campaignId = campaignBanners[kv.first].front().campaign_id;
            if (tempBid == MIN_BANNER_BID_GUARD) {
                //base case, insert 1st row
                bidBannerIdRandomize[bannerBid].push_back(campaignBanners[kv.first].front());
            } else if ( tempBid > MIN_BANNER_BID_GUARD && tempCampaignId != campaignId ) {
                //campaigns are already shuffled, so we can safely take the first one
                bidBannerIdRandomize[bannerBid].push_back(campaignBanners[kv.first].front());
            }

            tempBid = bannerBid;
            tempCampaignId = campaignBanners[kv.first].front().campaign_id;
        }

        for (auto kv : bidBannerIdRandomize) {
            std::shuffle(bidBannerIdRandomize[kv.first].begin(), bidBannerIdRandomize[kv.first].end(), myRng);
            outputBanners.insert(outputBanners.end(),
                                 bidBannerIdRandomize[kv.first].begin(), bidBannerIdRandomize[kv.first].end());

        }

        return outputBanners;
    }


}