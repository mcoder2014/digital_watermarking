# Watermarking
将水印抽象，共同继承于一个水印基类，通过工厂模式创建多个不同类型的水印，依次添加于图片。
为了之后更方便将核心代码移植与其他平台，这里尽量不要用到Qt的相关工具，而是使用STL。为了促进自己学习，指针使用STL的智能指针`unique_ptr/shared_ptr/weak_ptr`进行代替。

# 一般可见水印基类
Watermark

## 图像水印
ImgWatermark
1. 不透明
2. 调整alpha值调整透明度融合

### 泊松水印无缝融合
[Possion](https://www.cs.virginia.edu/~connelly/class/2014/comp_photo/proj2/poisson.pdf)泊松无缝融合水印(透明度设置无效)

## 文字水印
TextWatermark

# 不可见水印基类
`UnvisbleWatermark` 要求子类需要继承检查水印的功能，因为水印肉眼难以发现就必须要有检验的方法。

## LSB水印
`LSBImgWatermark`: 是利用图像低位不敏感性储存水印数据的方法，但稳定性不强，压缩图象就有可能导致水印的损失。

1. 隐藏在指定位数的低位
2. 在RGB不同的通道中隐藏不同的灰度图

## 频域水印
`FFTImgWatermark`: 将水印与傅里叶变换后的图像的频域相叠加。

