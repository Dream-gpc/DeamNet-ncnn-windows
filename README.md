# DeamNet-ncnn-windows
* This is an unofficial simple implementation of DeamNet based on ncnn.  
* 这是基于ncnn的DeamNet方法的非官方简单实现。

* [官方pytorch项目](https://github.com/chaoren88/DeamNet)
* [ncnn yes!](https://github.com/Tencent/ncnn)

## 环境准备
* Visual Studio 2019/2022  
* NCNN 你可以在此处[下载](https://github.com/Tencent/ncnn/releases)预编译版本  
* OpenCV 4.6.0  


## 使用方式
编译成功后使用以下命令即可  

`./<xxx.exe>  <image-path>` 


## 结果

### 灰度图

* Gray 25:  

| Input | Pytorch | ncnn |
| --- | --- | --- |
|  <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/imtest/noiseimg1.png" width="300px">   | <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/denoiseimg1.png" width="300px"> | <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/denoiseimg1-ncnn.png" width="300px"> | 
|  <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/imtest/noiseimg2.png" width="300px"> | <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/denoiseimg2.png" width="300px"> |  <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/denoiseimg2-ncnn.png" width="300px"> | 


### 彩色图

| Input | Pytorch | ncnn |
| --- | --- | --- |
|  <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/imtest/cnoiseimg1.jpg" width="300px">   | <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/cdenoiseimg1.jpg" width="300px"> | <img src="https://github.com/Dream-gpc/DeamNet-ncnn-windows/blob/main/results/cdenoiseimg1-ncnn.png" width="300px"> | 


## Running time

* 在AMD Ryzen 5 5600G上进行测试  
* 仅在Gray25上测试running time！
* 上级没给我配卡！:sob: 5555！

| 分辨率 | CPU | 核显+vulkan | 独显+vulkan:sob:|
| :---: | :---: | :---: | :---: | 
| 256x256 (Gray) | 4.63±s | 1.80±s | ?? | 
| 512x512 (Gray)| 18.35±s |3.72±s | ?? | 
| 1200x1600 (Gray)| Run failed | 24.41±s | ?? | 

## Citation

> 
    @InProceedings{Ren_2021_CVPR,  
    author    = {Ren, Chao and He, Xiaohai and Wang, Chuncheng and Zhao, Zhibo}, 
    title     = {Adaptive Consistency Prior Based Deep Network for Image Denoising},  
    booktitle = {Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (CVPR)},  
    month     = {June},  
    year      = {2021},  
    pages     = {8596-8606}
    }
  

## License
MIT © Dream-gpc
