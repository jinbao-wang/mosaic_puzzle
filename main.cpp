//
//  main.cpp
//  mosaic_puzzle
//
//  Created by jinbao wang on 24/08/2018.
//  Copyright © 2018 Wang-Jinbao. All rights reserved.
//



#include <time.h>
#include <iostream>
#include "imgProc.hpp"


void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    
    while((pos=strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}

void process()
{
    
    double a, b, c, d, e;
    
    cv::Mat img;
    imgProc imgproc(16, 12, 60, 40);
    
    std::string path = "/Users/Link/Desktop/1.jpg";
    cv::String mp_db = "/Users/Link/Documents/MP_DB/test2015_60x40_10224/*.jpg";
    
    a = clock();
    imgproc.loadImage(path, img);
    imgproc.getCoordIndices(img);
    
    b = clock();
    std::cout << "b-a: " << (b - a)/CLOCKS_PER_SEC << std::endl;
    
    imgproc.loadDataBaseImgs(mp_db);
    
    c = clock();
    std::cout << "c-b: " << (c - b)/CLOCKS_PER_SEC << std::endl;

    imgproc.getDataBaseImgHist();
    
    d = clock();
    std::cout << "d-c: " << (d - c)/CLOCKS_PER_SEC << std::endl;
    
    cv::Mat dst;
    imgproc.fillMosaicToPuzzle(dst);
    
    e = clock();
    std::cout << "e-d: " << (e - d)/CLOCKS_PER_SEC << std::endl;
    
    
    imgproc.showInfo();
    
    
    int width = 600;
    cv::resize(img, img, cv::Size(width, img.rows * width / img.cols));
    cv::imshow("img", img);
    
    string_replace(path, ".jpg", "_out_n.jpg");
    cv::imwrite(path, dst);
    
    cv::resize(dst, dst, cv::Size(width, img.rows * width / img.cols));
    cv::imshow("dst", dst);
    
    

    
    
    //cv::waitKey();
    
}


int main(int argc, const char * argv[]) {
    
    double s, e;
    s = clock();
    
    process();

    
    e = clock();
    
    std::cout << "time cost: " << (e - s)/CLOCKS_PER_SEC << std::endl;

    
    
    return 0;
}
