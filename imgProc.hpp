//
//  imgProc.hpp
//  mosaic_puzzle
//
//  Created by jinbao wang on 24/08/2018.
//  Copyright © 2018 Wang-Jinbao. All rights reserved.
//

#ifndef imgProc_hpp
#define imgProc_hpp

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <string>
#include <vector>

class imgProc{

public:
    imgProc(int h, int w, int th_h, int th_w):mosaic_h(h), mosaic_w(w), thumbnail_h(th_h), thumbnail_w(th_w){};
    ~imgProc(){};
    
    void loadImage(const std::string path, cv::Mat& img);
    void loadDataBaseImgs(cv::String pattern);
    void getDataBaseImgHist();
    
    void getCoordIndices(const cv::Mat& img);
    
    void fillMosaicToPuzzle(cv::Mat& puzzle);
    
    void findTargetThumbnail(cv::Mat& thumbnail, int index);
    
    void selectThumbnail(const cv::Mat& mosaic, cv::Mat& thumbnail);
    

    
    void showInfo();

private:
    
    void getHistogram(const cv::Mat& img, cv::MatND& hist);
    double imgSimilarity(const cv::MatND& srcHist, const cv::MatND& refHist);
    
    int img_h;  // input img resolution
    int img_w;
    
    int mosaic_h_n; // how many mosaics
    int mosaic_w_n;
    
    int puzzle_h; // dst img resolution
    int puzzle_w;
    
    const int mosaic_h; // how many pixels of img build one mosaic
    const int mosaic_w;
    
    const int thumbnail_h; // thumbnail img resolution
    const int thumbnail_w;
    
    std::vector<cv::Rect> mosaic_coord_indices;
    std::vector<cv::Mat> mosaic_imgs;
    std::vector<cv::Rect> thumbnail_coord_indices;
    
    std::vector<cv::Mat> database_imgs;
    std::vector<cv::MatND> database_imgs_hist;
    

};





#endif /* imgProc_hpp */
