//
//  imgProc.cpp
//  mosaic_puzzle
//
//  Created by jinbao wang on 24/08/2018.
//  Copyright © 2018 Wang-Jinbao. All rights reserved.
//

#include "imgProc.hpp"
#include <iostream>
#include <algorithm>


using namespace cv;
using namespace std;


void imgProc::loadImage(const std::string path, cv::Mat& img)
{
    img = imread(path);
    
    if(img.empty()){
        cerr << "load image failed!"<< endl;
    }
    
    mosaic_h_n = img.rows / mosaic_h;
    mosaic_w_n = img.cols / mosaic_w;
    
    // resize src img
    img_w = mosaic_w_n * mosaic_w;
    img_h = mosaic_h_n * mosaic_h;
    
    resize(img, img, Size(img_w, img_h));
    
    puzzle_h = mosaic_h_n * thumbnail_h;
    puzzle_w = mosaic_w_n * thumbnail_w;

}

void imgProc::loadDataBaseImgs(cv::String pattern)
{
    vector<String> filenames;
    glob(pattern, filenames, false);
    
    auto n = filenames.size();
    // n = 5000;
    for(size_t i = 0; i < n; ++i)
    {
        //cout<<filenames[i]<<endl;
        Mat img = imread(filenames[i]);
        
        if(!img.data){
            cerr << "Problem loading image!!!" << endl;
            continue;
        }

        //resize(img, img, cv::Size(40, 30));
        database_imgs.push_back(img);
    }
}

void imgProc::getDataBaseImgHist()
{
    for(vector<Mat>::const_iterator iter=database_imgs.cbegin(); iter!=database_imgs.cend(); ++iter){
        MatND hist;
        getHistogram(*iter, hist);
        database_imgs_hist.push_back(hist);
    }
}


void imgProc::getCoordIndices(const cv::Mat& img)
{
    thumbnail_coord_indices.clear();
    mosaic_coord_indices.clear();
    mosaic_imgs.clear();
    
    for(int i=0; i!=mosaic_w_n; ++i){
        for(int j=0; j!=mosaic_h_n; ++j){
            Rect rect;

            rect.x = i * thumbnail_w;
            rect.y = j * thumbnail_h;
            rect.width = thumbnail_w;
            rect.height = thumbnail_h;
            thumbnail_coord_indices.push_back(rect);
            
            Rect rect_m;
            rect_m.x = i * mosaic_w;
            rect_m.y = j * mosaic_h;
            rect_m.width = mosaic_w;
            rect_m.height = mosaic_h;
            mosaic_coord_indices.push_back(rect_m);
            
            Mat mosaic = Mat(img, rect_m);
            mosaic_imgs.push_back(mosaic);

            //cout<<"x: " << rect.x << " y: "<< rect.y <<" w: " << rect.width <<" h: "<< rect.height <<endl;
            //cout<<"x: " << rect_m.x << " y: "<< rect_m.y <<" w: " << rect_m.width <<" h: "<< rect_m.height <<endl;
        }
    }
}

void imgProc::selectThumbnail(const cv::Mat& mosaic, cv::Mat& thumbnail)
{
    MatND mosaic_hist;
    getHistogram(mosaic, mosaic_hist);
    
    vector<double> evaluation;
    
    for(vector<MatND>::const_iterator iter=database_imgs_hist.cbegin(); iter!=database_imgs_hist.cend(); ++iter){
        double eval = imgSimilarity(mosaic_hist, *iter);
        //cout << eval << endl;
        evaluation.push_back(eval);
    }
    
    vector<double>::iterator maxLoc = max_element(evaluation.begin(), evaluation.end());
    
    auto location = distance(evaluation.begin(), maxLoc);
    //cout << *maxLoc << " at the postion of " << location <<endl;
    
    thumbnail = database_imgs[location];
    
}

// TODO: the target img replaces mosaic_imgs[index]
void imgProc::findTargetThumbnail(cv::Mat& thumbnail, int index)
{
    //thumbnail = mosaic_imgs[index]; // fill src img

    double s, e;
    s = clock();
    selectThumbnail(mosaic_imgs[index], thumbnail);
    e = clock();
    //std::cout << "selectThumbnail: " << (e - s)/CLOCKS_PER_SEC << std::endl;
    
    resize(thumbnail, thumbnail, Size(thumbnail_w, thumbnail_h));

}


void imgProc::fillMosaicToPuzzle(cv::Mat& puzzle)
{
    puzzle = Mat(Size(puzzle_w, puzzle_h), CV_8UC3, Scalar::all(255));
    

    int n = mosaic_h_n * mosaic_w_n;
    for(int i=0; i< n; ++i){
        Mat thumbnail;
        Mat roi = puzzle(thumbnail_coord_indices[i]);
        findTargetThumbnail(thumbnail, i);
        
        thumbnail.copyTo(roi);
    }
}


void imgProc::getHistogram(const cv::Mat& img, cv::MatND& hist)
{
    Mat hsv;
    cvtColor(img, hsv, CV_BGR2HSV);

    //H,S channel
    int channels[] = {0, 1};
    int histSize[] = {30, 32};
    float HRanges[] = {0, 180};
    float SRanges[] = {0, 256};
    const float *ranges[] = {HRanges, SRanges};

    calcHist(&hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
    normalize(hist, hist, 0, 1, NORM_MINMAX);

}

double imgProc::imgSimilarity(const cv::MatND& srcHist, const cv::MatND& refHist)
{
    return compareHist(srcHist, refHist, 1);
}







void imgProc::showInfo()
{
    cout << "src_img" << endl;
    cout << "   w: " << img_w << endl;
    cout << "   h: " << img_h << endl;

    cout << "mosaic_img " << endl;
    cout << "   w: " << mosaic_w << endl;
    cout << "   h: " << mosaic_h << endl;

    cout << "thumbnail_num" << endl;
    cout << "   w: " << mosaic_w_n << endl;
    cout << "   h: " << mosaic_h_n << endl;
    cout << "   n: " << mosaic_w_n * mosaic_h_n << endl;
    
    cout << "thumbnail_img" << endl;
    cout << "   w: " << thumbnail_w << endl;
    cout << "   h: " << thumbnail_h << endl;

    cout << "puzzle_img" << endl;
    cout << "   w: " << puzzle_w << endl;
    cout << "   h: " << puzzle_h << endl;
    
    
    // test similarity of two images
    //    cv::Mat img = cv::imread("/Users/Link/Desktop/MM2018/images/A/lviv.jpg");
    //    cv::Mat img2 = cv::imread("/Users/Link/Desktop/MM2018/images/A/lviv_New.jpg");
    //    cv::Mat img3 = cv::imread("/Users/Link/Desktop/MM2018/images/A/house_New.jpg");
    //
    //    cv::resize(img, img, cv::Size(40, 30));
    //    cv::resize(img2, img2, cv::Size(40, 30));
    //    cv::resize(img3, img3, cv::Size(40, 30));
    //
    //    double s, e;
    //    s = clock();
    //
    //    cv::MatND hist, hist2, hist3;
    //    imgproc.getHistogram(img, hist);
    //    imgproc.getHistogram(img2, hist2);
    //    imgproc.getHistogram(img3, hist3);
    //
    //    double s1 = imgproc.imgSimilarity(hist, hist2);
    //    double s2 = imgproc.imgSimilarity(hist, hist3);
    //
    //    std::cout << s1 << std::endl;
    //    std::cout << s2 << std::endl;
    //    e = clock();
    //    
    //    std::cout << "time cost: " << (e - s)/CLOCKS_PER_SEC << std::endl;
    
}


