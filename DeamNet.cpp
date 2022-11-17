#include "net.h"
#include<iostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <gpu.h>
#include "layer.h"
#include "chrono"

using namespace std;
using namespace chrono;

void visualize(const char* title, const ncnn::Mat& m)
{
    std::vector<cv::Mat> normed_feats(m.c);
    cout << m.c << endl;

    for (int i = 0; i < m.c; i++)
    {
        cv::Mat tmp(m.h, m.w, CV_32FC1, (void*)(const float*)m.channel(i));

        cv::normalize(tmp, normed_feats[i], 0, 255, cv::NORM_MINMAX, CV_8U);

        cv::cvtColor(normed_feats[i], normed_feats[i], cv::COLOR_GRAY2BGR);

        // check NaN
        for (int y = 0; y < m.h; y++)
        {
            const float* tp = tmp.ptr<float>(y);
            uchar* sp = normed_feats[i].ptr<uchar>(y);
            for (int x = 0; x < m.w; x++)
            {
                float v = tp[x];
                if (v != v)
                {
                    sp[0] = 0;
                    sp[1] = 0;
                    sp[2] = 255;
                }

                sp += 3;
            }
        }
    }

    int tw = m.w < 10 ? 32 : m.w < 20 ? 16 : m.w < 40 ? 8 : m.w < 80 ? 4 : m.w < 160 ? 2 : 1;
    int th = (m.c - 1) / tw + 1;

    cv::Mat show_map(m.h * th, m.w * tw, CV_8UC3);
    show_map = cv::Scalar(127);

    // tile
    for (int i = 0; i < m.c; i++)
    {
        int ty = i / tw;
        int tx = i % tw;

        normed_feats[i].copyTo(show_map(cv::Rect(tx * m.w, ty * m.h, m.w, m.h)));
    }

    cv::resize(show_map, show_map, cv::Size(0, 0), 2, 2, cv::INTER_NEAREST);
    cv::imshow(title, show_map);
}

void pretty_print(const ncnn::Mat& m)
{
    for (int q = 0; q < m.c; q++)
    {
        const float* ptr = m.channel(q);
        for (int y = 0; y < m.h; y++)
        {
            for (int x = 0; x < m.w; x++)
            {
                printf("%f ", ptr[x]);
            }
            ptr += m.w;
            printf("\n");
        }
        printf("------------------------\n");
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }
    // 读取图片
    const char* imagepath = argv[1];
    cv::Mat m = cv::imread(imagepath, 1);
   
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }
    auto start = system_clock::now(); 
    // 获取图片的宽
    int w = m.cols;
    //获取图片的高
    int h = m.rows;


    ncnn::Net deam;
    // 设置相关参数
    //ncnn::VulkanDevice vkdev(0); 
    //deam.opt.num_threads = 4;
    deam.opt.use_vulkan_compute = true; //可利用核显加速
    //deam.opt.use_bf16_storage = true;
    //deam.opt.use_packing_layout = true;
    //deam.opt.use_int8_inference = true;
    //deam.opt.use_fp16_packed = true;
   
    ncnn::Mat in = ncnn::Mat::from_pixels(m.data, ncnn::Mat::PIXEL_BGR2RGB,m.cols, m.rows); //opencv bgr
    //ncnn::Mat in = ncnn::Mat::from_pixels(m.data, ncnn::Mat::PIXEL_GRAY,m.cols, m.rows); //处理灰度图
    
    // 导入模型结果以及模型权重
    if (deam.load_param("DeamNet-real.ncnn.param")) //DeamNet-grayxx.ncnn.param
        exit(-1);
    if (deam.load_model("DeamNet-real.ncnn.bin"))  //DeamNet-grayxx.ncnn.bin
        exit(-1);

  
    // 设置图像预处理相关参数
    //const float mean_vals[3] = { 104.f, 117.f, 123.f }; //去噪和增强模型很少见到减去均值
    const float normal_vals[3] = { 1/255.f, 1/255.f, 1/255.f }; //彩色图归一化
    //const float normal_vals[1] = { 1/255.f }; //灰度图归一化
  
    in.substract_mean_normalize(0, normal_vals);
     //pretty_print(in); 
    ncnn::Extractor ex = deam.create_extractor();
    //ex.set_light_mode(true); 
    ex.input("in0", in);
    // 获取模型输出
    ncnn::Mat out;
    ex.extract("out0", out);
    const float normal_vals1[3] = { 255.f,255.f,255.f, };
    //const float normal_vals1[1] = { 255.f };//灰度图
    out.substract_mean_normalize(0, normal_vals1);

    cv::Mat a(out.h, out.w, CV_8UC3);
    out.to_pixels(a.data, ncnn::Mat::PIXEL_RGB2BGR);

    auto finish = system_clock::now();
    auto duration = duration_cast<microseconds>(finish - start);
    auto cost = double(duration.count()) * microseconds::period::num / microseconds::period::den;

    cout << "cost: " << cost << "s.\n" << endl;
    cout << "============\n" << endl;
    cout <<"finish!\n"<< endl;
    cv::imwrite("./test.png", a);

    return 0;
}
