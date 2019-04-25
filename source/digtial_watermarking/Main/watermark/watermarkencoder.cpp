#include "watermarkencoder.h"



WatermarkEncoder::WatermarkEncoder()
{
}

/**
 * @brief WatermarkEncoder::encode
 * @param src CV_8UC4
 * @param dst CV_8UC4
 * @param key
 */
void WatermarkEncoder::encode(const cv::Mat &src, cv::Mat &dst, uint64_t key)
{
    this->seed = key;       // update key

    size_t width = src.size().width;                 // Width of src image
    size_t height = src.size().height;      // Half Height of src image

    std::vector<int> W_index(width);
    std::vector<int> H_index(height);

    for(int i=0; i<width; i++)
        W_index[i] = i;
    for(int i=0; i<height; i++)
        H_index[i]=i;

    // I want use fake rand to make sure with same key and same size,
    // the encoded image will get same result
    fresh(W_index);
    fresh(H_index);

    cv::Mat tmp = src.clone();
    for (int i=0; i<height; i++)
    {
        for (int j=0; j<width; j++) {
            // Change pixel based on W_index and H_index
            cv::Vec4b pixel = src.at<cv::Vec4b>(H_index[i],W_index[j]);
            tmp.at<cv::Vec4b>(i,j) = pixel;
        }
    }
//    cv::Mat half_upper = tmp(cv::Rect(0,0,width,half_height));
//    cv::Mat half_below = tmp(cv::Rect(0,half_height,width,half_height));
//    cv::flip(half_upper,half_below,-1);          // Flip

    // Clone and return
    dst = tmp.clone();
}

void WatermarkEncoder::decode(const cv::Mat &src, cv::Mat &dst, uint64_t key)
{
    this->seed = key;       // update key

    size_t width = src.size().width;                 // Width of src image
    size_t height = src.size().height;      // Half Height of src image

    std::vector<int> W_index(width);
    std::vector<int> H_index(height);

    for(int i=0; i<width; i++)
        W_index[i] = i;
    for(int i=0; i<height; i++)
        H_index[i]=i;

    // I want use fake rand to make sure with same key and same size,
    // the encoded image will get same result
    fresh(W_index);
    fresh(H_index);

    cv::Mat tmp = src.clone();
    for (int i=0; i<height; i++)
    {
        for (int j=0; j<width; j++) {
            // Change pixel based on W_index and H_index
            cv::Vec4b pixel = src.at<cv::Vec4b>(i,j);
            tmp.at<cv::Vec4b>(H_index[i],W_index[j]) = pixel;
        }
    }

//    cv::Mat half_upper = tmp(cv::Rect(0,0,width,half_height));
//    cv::Mat half_below = tmp(cv::Rect(0,half_height,width,half_height));
//    cv::flip(half_upper,half_below,-1);          // Flip

    // Clone and return
    dst = tmp.clone();
}

int WatermarkEncoder::fakeRand()
{
    seed = seed * 1103515245 + 12345;
    return (unsigned int) (seed / 65536) % 32768;
}

void WatermarkEncoder::fresh(std::vector<int> &vec)
{
    int length = vec.size();
    for(int i = length-1; i>0; i--)
    {
        int tmp_index = fakeRand()%i;
        // swap
        int tmp_value = vec[tmp_index];
        vec[tmp_index] = vec[i];
        vec[i] = tmp_value;
    }
}
