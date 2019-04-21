#include "fftimgwatermark.h"

#include <iostream>
#include <vector>

#include <QImage>
#include <QDebug>

#include "tools.h"


FFTImgWatermark::FFTImgWatermark():ImgWatermark ()
{
    this->_rotational_symmetry = true;
}

FFTImgWatermark::FFTImgWatermark(const ImgWatermark *imgwatermark)
    :ImgWatermark(imgwatermark)
{

}

FFTImgWatermark::FFTImgWatermark(const FFTImgWatermark *fftimgwatermark)
    :ImgWatermark (fftimgwatermark)
{

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
    cv::copyMakeBorder(src, src_larger, 0, recommend_height-src.rows, 0, recommend_width-src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

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

    // Remember the max and min value, to run an anti-normalize transform of watermark
//    double min_src_dft[3];
//    double max_src_dft[3];
//    for(int i=0; i <3; i++)
//    {
//        std::vector<cv::Mat> planes;
//        cv::split(src_larger_fft[i], planes);

//        cv::magnitude(planes[0], planes[1], planes[0]);
//        cv::Mat mag = planes[0];
//        mag += cv::Scalar::all(1);
//        cv::log(mag, mag);

//        cv::minMaxIdx(mag, &(min_src_dft[i]),&(max_src_dft[i]));    // Find the max and min value of log image

//        /*------------------DEBUG----------------------*/
//        // crop the spectrum, if it has an odd number of rows or columns
//        mag = mag(cv::Rect(0, 0, mag.cols & -2, mag.rows & -2));

//        int cx = mag.cols/2;
//        int cy = mag.rows/2;

//        // rearrange the quadrants of Fourier image
//        // so that the origin is at the image center
//        cv::Mat tmp;
//        cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));
//        cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));
//        cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));
//        cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy));

//        q0.copyTo(tmp);
//        q3.copyTo(q0);
//        tmp.copyTo(q3);

//        q1.copyTo(tmp);
//        q2.copyTo(q1);
//        tmp.copyTo(q2);

//        normalize(mag, mag, 0, 1, cv::NORM_MINMAX);
//        mag = mag*255;
//        mag.convertTo(mag, CV_8UC1);

//        DEBUG_SAVE_MAT(mag, std::string("debug/spectrum_src_") + char('0'+i) + std::string(".png"));
//        /*------------------DEBUG----------------------*/
//    }

    // Add Watermark into src_larger_fft
    this->singleChannelWatermark(tmp, src_larger_fft, dst_x, dst_y);

    if(this->_rotational_symmetry)
    {
        // Flip
        for(int i=0;i<3;i++)
            cv::flip(src_larger_fft[i],src_larger_fft[i], -1);
        // Add watermark
        this->singleChannelWatermark(tmp, src_larger_fft, dst_x, dst_y);
        // Recover Filp
        for(int i=0;i<3;i++)
            cv::flip(src_larger_fft[i],src_larger_fft[i], -1);
    }

    /*--Debug output the dst added watermark--*/
    {
        std::vector<cv::Mat> src_large_fft_sp(3);
        for(int i=0; i<3; i++)
        {
            std::vector<cv::Mat> planes;
            cv::split(src_larger_fft[i], planes);
            planes[0].convertTo(src_large_fft_sp[i], CV_8UC1);

            DEBUG_SAVE_MAT(src_large_fft_sp[i],
                std::string("debug/sp_added_watermark_") + char('0'+i) + std::string(".png"));
        }
        cv::Mat test;
        cv::merge(src_large_fft_sp, test);
        DEBUG_SAVE_MAT(test, "debug/sp_added_watermark_merge.png");
    }
    /*----------------------------------------*/

    // IDFT - get the added watermark image
    std::vector<cv::Mat> dst_larger_channels(4);

    // Run fftSingleChannel for RGB channel of the src image
    for(int i = 0 ; i < 3; i++)
    {
        ifftSingleChannel(src_larger_fft[i], dst_larger_channels[i]);
        dst_larger_channels[i] = dst_larger_channels[i]*(max_src[i] - min_src[i]) + min_src[i];
        dst_larger_channels[i].convertTo(dst_larger_channels[i], CV_8UC1);
    }


    dst_larger_channels[3] = src_larger_channels[3].clone();

    cv::Mat dst_larger;
    cv::merge(dst_larger_channels, dst_larger);

    // RETURN the dst value
    dst = dst_larger(cv::Rect(0,0,src.cols, src.rows)).clone();
}

void FFTImgWatermark::checkWatermark(const cv::Mat &src, cv::Mat &dst)
{
    // 3. Merge Watermark
    src.convertTo(dst, CV_8UC4);    // Change dst mat type

    //    cv::Mat Roi = dst(cv::Rect(dst_x,dst_y,tmp.size().width,tmp.size().height));
    int recommend_height = cv::getOptimalDFTSize(src.rows);
    int recommend_width = cv::getOptimalDFTSize(src.cols);

    // add zeros to larger src image for good performance
    cv::Mat src_larger;
    cv::copyMakeBorder(src, src_larger, 0, recommend_height-src.rows, 0, recommend_width-src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // split to BGRA and only do fft in BGR channels
    std::vector<cv::Mat> src_larger_channels;
    cv::split(src_larger, src_larger_channels);

    std::vector<cv::Mat> src_larger_fft(3);         // The container to save fft image
    std::vector<cv::Mat> src_larger_normalized(3);  // Normalized to [0,255] CV_8UC1
    std::vector<cv::Mat> src_larger_direct(3);      // Directed to  CV_8UC1

    // Run fft transform
    for(int i = 0; i < 3; i++)
    {
        // Run fftSingleChannel for RGB channel of the src image
        fftSingleChannel(src_larger_channels[i], src_larger_fft[i]);

        // Normalize
        std::vector<cv::Mat> planes;
        cv::split(src_larger_fft[i], planes);

        // Direct record
        src_larger_direct[i] = planes[0].clone();
        src_larger_direct[i].convertTo(src_larger_direct[i], CV_8UC1);

        DEBUG_SAVE_MAT(src_larger_direct[i],
            std::string("debug/check_direct_spectrum_src_") + char('0'+i) + std::string(".png"));

        cv::magnitude(planes[0], planes[1], planes[0]);
        cv::Mat mag = planes[0];
        mag += cv::Scalar::all(1);
        cv::log(mag, mag);

        // crop the spectrum, if it has an odd number of rows or columns
        mag = mag(cv::Rect(0, 0, mag.cols & -2, mag.rows & -2));

        int cx = mag.cols/2;
        int cy = mag.rows/2;

        // rearrange the quadrants of Fourier image
        // so that the origin is at the image center
        cv::Mat tmp;
        cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));
        cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));
        cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));
        cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy));

        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);

        // Normalize to [0,255]
        normalize(mag, mag, 0, 1, cv::NORM_MINMAX);
        mag = mag*255;
        mag.convertTo(mag, CV_8UC1);
        src_larger_normalized[i] = mag.clone();

        DEBUG_SAVE_MAT(mag,
            std::string("debug/check_spectrum_src_") + char('0'+i) + std::string(".png"));
        /*------------------DEBUG----------------------*/
    }

    // merge and output test
    cv::Mat tmp;
    cv::merge(src_larger_direct, tmp);
    DEBUG_SAVE_MAT(tmp, std::string("debug/check_spectrum_src_merge.png"));

    cv::merge(src_larger_normalized, tmp);
    DEBUG_SAVE_MAT(tmp, std::string("debug/check_direct_spectrum_src_merge.png"));

    dst = tmp.clone();
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
    cv::dft(complete, complete);

    dst = complete.clone();
}

void FFTImgWatermark::ifftSingleChannel(const cv::Mat &src, cv::Mat &dst)
{
    assert(src.channels() == 2);

    cv::Mat complete = src.clone();
    cv::idft(complete, complete);

    std::vector<cv::Mat> planes;
    cv::split(complete, planes);

//    magnitude(planes[0], planes[1], planes[0]);
    normalize(planes[0], planes[0], 0, 1, cv::NORM_MINMAX);

    // direct multple 255, may cause a not good problem.
//    planes[0] = planes[0]*255;
//    planes[0].convertTo(dst, CV_8UC1);
    dst = planes[0].clone();
}

void FFTImgWatermark::singleChannelWatermark(
        const cv::Mat &watermark, std::vector<cv::Mat> &dst,
        int dst_x, int dst_y)
{
    // Cut the small part of image
    std::vector<cv::Mat> src_larger_fft(3);
    double min_src_fft[3];
    double max_src_fft[3];
    for(int i=0; i < 3; i++)
    {
        cv::Mat tmp;
        std::vector<cv::Mat> tmp_channels;
        cv::split(dst[i], tmp_channels);
        cv::magnitude(tmp_channels[0], tmp_channels[1],tmp);
        tmp += cv::Scalar::all(1);
        cv::log(tmp, tmp);

        cv::minMaxIdx(tmp,&(min_src_fft[i]), &(max_src_fft[i]));

        qDebug() << "dst[" << i << "]" << "max: " << max_src_fft[i] << " min: " << min_src_fft[i];
        src_larger_fft[i] = (dst[i])(
            cv::Rect(dst_x, dst_y,watermark.size().width, watermark.size().height));
    }

    float alpha = this->_alpha/100.0;       // 0-100
    // Merge the unchanged watermark image in the fft image
    for(int i=0; i <watermark.rows; i++)
    {
        for(int j=0; j<watermark.cols; j++)
        {
            // watermark
            cv::Vec4b wm_pixel = watermark.at<cv::Vec4b>(i,j);
            float real_alpha = alpha * (wm_pixel[3]/255);

            // B
            cv::Vec2f dst_pixel_b = src_larger_fft[0].at<cv::Vec2f>(i,j);
            float wm_b = watermarkPixel2Spectrum(wm_pixel[0],min_src_fft[0],max_src_fft[0]);
            dst_pixel_b[0] = (1-real_alpha)*dst_pixel_b[0] + real_alpha * wm_b;
            dst_pixel_b[1] = (1-real_alpha)*dst_pixel_b[1] + real_alpha * wm_b;

            src_larger_fft[0].at<cv::Vec2f>(i,j) = dst_pixel_b;

            // G
            cv::Vec2f dst_pixel_g = src_larger_fft[1].at<cv::Vec2f>(i,j);
            float wm_g = watermarkPixel2Spectrum(wm_pixel[1],min_src_fft[1],max_src_fft[1]);
            dst_pixel_g[0] = (1-real_alpha)*dst_pixel_g[0] + real_alpha * wm_g;
            dst_pixel_g[1] = (1-real_alpha)*dst_pixel_g[1] + real_alpha * wm_g;

            src_larger_fft[1].at<cv::Vec2f>(i,j) = dst_pixel_g;

            // R
            cv::Vec2f dst_pixel_r = src_larger_fft[2].at<cv::Vec2f>(i,j);
            float wm_r = watermarkPixel2Spectrum(wm_pixel[2],min_src_fft[2],max_src_fft[2]);
            dst_pixel_r[0] = (1-real_alpha)*dst_pixel_r[0] + real_alpha * wm_r;
            dst_pixel_r[1] = (1-real_alpha)*dst_pixel_r[1] + real_alpha * wm_r;

            src_larger_fft[2].at<cv::Vec2f>(i,j) = dst_pixel_r;
        }
    }
}

float FFTImgWatermark::watermarkPixel2Spectrum(unsigned char pixel, float min, float max)
{
    float watermark = (float)pixel;
    watermark = ((max-min)/255)*watermark + min;
//    watermark = cv::exp(watermark);
//    watermark = watermark/sqrt(2);

    return watermark;
}
