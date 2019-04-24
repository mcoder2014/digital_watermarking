#ifndef WATERMARKENCODER_H
#define WATERMARKENCODER_H

#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>

class WatermarkEncoder
{
public:
    WatermarkEncoder();

    void encode(const cv::Mat& src, cv::Mat& dst, uint64_t key = 0);
    void decode(const cv::Mat& src, cv::Mat& dst, uint64_t key = 0);

private:
    int fakeRand();
    void fresh(std::vector<int>& vec);
    uint64_t seed = 1;                  // Seed of Rand algorithm
};

#endif // WATERMARKENCODER_H
