#include "fftimgwatermark.h"

#include <iostream>
#include <vector>

#include <QImage>
#include <QDebug>

#include "tools.h"
#include "watermarkencoder.h"


FFTImgWatermark::FFTImgWatermark():ImgWatermark ()
{
    this->_rotational_symmetry = true;
    this->_key = 10;
    this->_needSourceImg = true;
}

FFTImgWatermark::FFTImgWatermark(const ImgWatermark *imgwatermark)
    :ImgWatermark(imgwatermark)
{
    this->_key = 10;
    this->_needSourceImg = true;
}

FFTImgWatermark::FFTImgWatermark(const FFTImgWatermark *fftimgwatermark)
    :ImgWatermark (fftimgwatermark)
{
    this->_key = 10;
    this->_needSourceImg = true;
}

void FFTImgWatermark::execute(cv::Mat &src, cv::Mat &dst)
{

    // 1. Adjust the watermark
    cv::Mat tmp;            // the watermark after transform
    // 2. Calculate Position
    int dst_x=0, dst_y=0;   // the postion (left-upper pointer of watermark) in destination image.
    this->_transformWatermark(src, tmp, dst_x, dst_y);
    // 3. Merge Watermark
    src.convertTo(dst, CV_8UC4);    // Change dst mat type

    //    cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    int recommend_height = cv::getOptimalDFTSize(src.rows);
    int recommend_width = cv::getOptimalDFTSize(src.cols);

    // add zeros to larger src image for good performance
    cv::Mat src_larger;
    cv::copyMakeBorder(dst, src_larger, 0, recommend_height-src.rows, 0, recommend_width-src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // split to BGRA and only do fft in BGR channels
    std::vector<cv::Mat> src_larger_channels;
    cv::split(src_larger, src_larger_channels);

    std::vector<cv::Mat> src_larger_fft(3);     // The container to save fft image

    // Remember the max and min value of src image, in order to recover the src image from fft image.
    double min_src[3];
    double max_src[3];

    // Run fft transform
    for(int i = 0; i < 3; i++)
    {
        // Run fftSingleChannel for RGB channel of the src image
        fftSingleChannel(src_larger_channels[i], src_larger_fft[i]);

        // calculate the max and min value of src image
        cv::minMaxIdx(src_larger_channels[i], &(min_src[i]),&(max_src[i]));
    }

    // Encode watermark image
    cv::Mat watermark_encode;
    this->watermarkEncode(tmp,watermark_encode,src_larger,dst_x,dst_y,this->_key);
    DEBUG_SAVE_MAT(watermark_encode, "debug/watermark_encoded.png");

    // Add Watermark into src_larger_fft
    this->mergeWatermark(watermark_encode, src_larger_fft);

    /*--Debug output the dst added watermark--*/
//    {
//        std::vector<cv::Mat> src_large_fft_sp(3);
//        for(int i=0; i<3; i++)
//        {
//            std::vector<cv::Mat> planes;
//            cv::split(src_larger_fft[i], planes);
//            cv::magnitude(planes[0], planes[1],planes[0]);
//            planes[0] += cv::Scalar::all(1);
//            cv::log(planes[0], planes[0]);
//            cv::normalize(planes[0], planes[0],0,255,cv::NORM_MINMAX);
//            planes[0].convertTo(src_large_fft_sp[i], CV_8UC1);

//            DEBUG_SAVE_MAT(src_large_fft_sp[i],
//                std::string("debug/sp_added_watermark_") + char('0'+i) + std::string(".png"));
//        }
//        cv::Mat test;
//        cv::merge(src_large_fft_sp, test);
//        DEBUG_SAVE_MAT(test, "debug/sp_added_watermark_merge.png");
//    }
    /*----------------------------------------*/

    // IDFT - get the added watermark image
    std::vector<cv::Mat> dst_larger_channels(4);

    // Run fftSingleChannel for RGB channel of the src image
    for(int i = 0 ; i < 3; i++)
    {
        ifftSingleChannel(src_larger_fft[i], dst_larger_channels[i]);
//        dst_larger_channels[i] = dst_larger_channels[i]*(max_src[i] - min_src[i]) + min_src[i];
        cv::normalize(dst_larger_channels[i], dst_larger_channels[i], min_src[i], max_src[i], cv::NORM_MINMAX);
        dst_larger_channels[i].convertTo(dst_larger_channels[i], CV_8UC1);
    }

    dst_larger_channels[3] = src_larger_channels[3].clone();

    cv::Mat dst_larger;
    cv::merge(dst_larger_channels, dst_larger);

    // RETURN the dst value
    dst = dst_larger(cv::Rect(0,0,src.cols, src.rows)).clone();

//    // Test
//    this->setSourceImg(src);
//    checkWatermark(src,dst);
}

///
/// \brief FFTImgWatermark::checkWatermark
/// \param src
/// \param dst
/// Will find a function to calculate the different resolution
///
void FFTImgWatermark::checkWatermark(const cv::Mat &src, cv::Mat &dst)
{
    // Get the copy of two image
    cv::Mat check_img = src.clone();            // the image need to be checked
    cv::Mat origin_img = this->_src.clone();    // the origin image

    //  convert to RGBA channels, make sure it has RGB channel
    check_img.convertTo(check_img, CV_8UC4);
    origin_img.convertTo(origin_img, CV_8UC4);

    // Get the recommend_size of fft
    int origin_recommend_height = cv::getOptimalDFTSize(origin_img.rows);
    int origin_recommend_width = cv::getOptimalDFTSize(origin_img.cols);

    // expand the size of image to recommend
    cv::Mat check_large, origin_large;

    cv::copyMakeBorder(check_img, check_large,
        0, origin_recommend_height-check_img.rows,0, origin_recommend_width-check_img.cols,
        cv::BORDER_CONSTANT, cv::Scalar::all(0));
    cv::copyMakeBorder(origin_img, origin_large,
        0, origin_recommend_height-origin_img.rows,0, origin_recommend_width-origin_img.cols,
        cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // split to BGRA and only do fft in BGR channels
    std::vector<cv::Mat> check_large_channels;
    std::vector<cv::Mat> origin_large_channels;

    cv::split(check_large, check_large_channels);
    cv::split(origin_large, origin_large_channels);

    // fft RGB channels
    for (int i=0; i<3; i++)
    {
        fftSingleChannel(check_large_channels[i], check_large_channels[i]);
        fftSingleChannel(origin_large_channels[i], origin_large_channels[i]);
    }

    // Get the encode_watermark
    std::vector<cv::Mat> encode_watermark_channels(3);
    // Init watermark with the size of origin_recommend
    for(int i=0;i<3;i++)
    {
        cv::Mat tmp = cv::Mat::zeros(origin_recommend_height, origin_recommend_width, CV_32F);
        encode_watermark_channels[i] = tmp.clone();
    }
    encode_watermark_channels.push_back(cv::Mat::zeros(origin_recommend_height, origin_recommend_width, CV_8UC1));
//    encode_watermark_channels[3] = encode_watermark_channels[3] * 255;

    // Compute the difference between the Check image and origin iamge
    for(int row=0; row<origin_recommend_height; row++)
    {
        for(int col=0; col<origin_recommend_width; col++)
        {
            for (int i=0;i<3;i++)
            {
                // RGB
                cv::Vec2f check_pixel = check_large_channels[i].at<cv::Vec2f>(row, col);
                cv::Vec2f origin_pixel = origin_large_channels[i].at<cv::Vec2f>(row, col);

//                float watermark_pixel = encode_watermark_channels[i].at<float>(row, col);
                float watermark_pixel = check_pixel[0] - origin_pixel[0];
                if(fabs(watermark_pixel) > 0.00001)
                {
                    encode_watermark_channels[4].at<uchar>(row,col) = (uchar)255;
                }
                encode_watermark_channels[i].at<float>(row, col) = watermark_pixel;
            }
        }
    }


    // Convert watermark to CV_8UC
    for (int i=0; i<3; i++)
    {
        qDebug() << "mean: " << cv::mean(encode_watermark_channels[i])[0];

        cv::magnitude(encode_watermark_channels[i], encode_watermark_channels[i], encode_watermark_channels[i]);
        encode_watermark_channels[i] +=cv::Scalar::all(1);
        cv::log(encode_watermark_channels[i],encode_watermark_channels[i]);

        normalize(encode_watermark_channels[i], encode_watermark_channels[i], 0, 255, cv::NORM_MINMAX);
        encode_watermark_channels[i] = encode_watermark_channels[i] * 255;
        encode_watermark_channels[i].convertTo(encode_watermark_channels[i],CV_8UC1);
    }

    cv::Mat encode_watermark;
    cv::merge(encode_watermark_channels, encode_watermark);
    DEBUG_SAVE_MAT(encode_watermark, "debug/fft_check_encode_watermark.png");

    cv::Mat watermark;
    this->watermarkDecode(encode_watermark, watermark, this->_key);
    DEBUG_SAVE_MAT(watermark, "debug/fft_check_watermark.png");

    dst = watermark.clone();
}

bool FFTImgWatermark::rotational_symmetry() const
{
    return _rotational_symmetry;
}

void FFTImgWatermark::setRotational_symmetry(bool rotational_symmetry)
{
    _rotational_symmetry = rotational_symmetry;
}

std::string FFTImgWatermark::toString()
{
    std::string output = ImgWatermark::toString();
    output = output
            + "\trotational_symmetry: "
            + std::to_string(this->_rotational_symmetry);
    return output;
}

uint64_t FFTImgWatermark::key() const
{
    return _key;
}

void FFTImgWatermark::setKey(const uint64_t &key)
{
    _key = key;
}

///
/// \brief FFTImgWatermark::fftSingleChannel
/// \param src
/// \param dst  the return value is two channel Mat { cv::Mat_<float>(src), cv::Mat::zeros(src.size(), CV_32F) };
/// Need expand the size of the image to march the best performance of fft algorithm
/// cv::copyMakeBorder(src, src_larger, 0, recommend_height-src.rows, 0, recommend_width-src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
///
void FFTImgWatermark::fftSingleChannel(const cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() == 1);

    cv::Mat plane0;
    src.convertTo(plane0,CV_32FC1);
    cv::Mat planes[] = { plane0, cv::Mat::zeros(src.size(), CV_32F) };
    cv::Mat complete;           // two channel
    cv::merge(planes, 2, complete);
//    cv::dft(complete, complete, cv::DFT_INVERSE | cv::DFT_SCALE, 0);
    cv::dft(complete, complete);

    dst = complete.clone();
}

void FFTImgWatermark::ifftSingleChannel(const cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() == 2);

    cv::Mat complete = src.clone();
//    cv::idft(complete, complete, cv::DFT_INVERSE | cv::DFT_SCALE, 0);
    cv::idft(complete, complete);

    std::vector<cv::Mat> planes;
    cv::split(complete, planes);

//    magnitude(planes[0], planes[1], planes[0]);
//    normalize(planes[0], planes[0], 0, 1, cv::NORM_MINMAX);

    // direct multple 255, may cause a not good problem.
//    planes[0] = planes[0]*255;
//    planes[0].convertTo(dst, CV_8UC1);
    dst = planes[0].clone();
}

///
/// \brief FFTImgWatermark::mergeWatermark
/// \param watermark
/// \param dsts
/// Add encoded watermark image to fft image dsts
void FFTImgWatermark::mergeWatermark(const cv::Mat &watermark, std::vector<cv::Mat> dsts)
{
    float alpha = this->_alpha/100.0;       // the strength of watermark
    int rows = watermark.rows;
    int cols = watermark.cols;
    qDebug() << "Add watermark on fft result of origin image, watermark width: "
             << cols
             << " watermark height: " << rows;

    for (int row=0; row < rows; row++)
    {
        for (int col=0; col < cols; col++)
        {
            cv::Vec4b watermark_pixel = watermark.at<cv::Vec4b>(row, col);

            for (int i=0; i<3; i++)
            {
                // BGR
                cv::Vec2f dst_pixel = dsts[i].at<cv::Vec2f>(row, col);
                dst_pixel[0] = dst_pixel[0] + alpha * watermark_pixel[i];
                dst_pixel[1] = dst_pixel[1] + alpha * watermark_pixel[i];
                dsts[i].at<cv::Vec2f>(row, col) = dst_pixel;
            }
        }
    }
}

///
/// \brief FFTImgWatermark::watermarkEncode
/// \param watermark
/// \param dst
/// \param src
/// \param dst_x
/// \param dst_y
/// \param key
///
void FFTImgWatermark::watermarkEncode(const cv::Mat &watermark, cv::Mat &dst, cv::Mat &src, int dst_x, int dst_y, uint64_t key)
{
    // Create canvas, and draw watermark on canvas, end with encode image
    cv::Mat watermark_canvas = cv::Mat::zeros(src.size().height, src.size().width,CV_8UC4);

    cv::Mat roi = watermark_canvas(cv::Rect(dst_x, dst_y, watermark.size().width,watermark.size().height));
    watermark.copyTo(roi);

    if(this->_rotational_symmetry)
    {
        // Flip
        cv::flip(watermark_canvas,watermark_canvas, -1);

        // Add watermark
        cv::Mat _roi = watermark_canvas(cv::Rect(dst_x, dst_y, watermark.size().width,watermark.size().height));
        watermark.copyTo(roi);

        // Recover Filp
        cv::flip(watermark_canvas,watermark_canvas, -1);
    }

    WatermarkEncoder encoder;
    encoder.encode(watermark_canvas, watermark_canvas, key);

    dst = watermark_canvas.clone();
}

void FFTImgWatermark::watermarkDecode(const cv::Mat &src, cv::Mat &dst, uint64_t key)
{
    WatermarkEncoder decoder;
    decoder.decode(src, dst, key);
}

