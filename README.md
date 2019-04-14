# 水印工具

[![GitHub stars](https://img.shields.io/github/stars/mcoder2014/digital_watermarking.svg?style=social)](https://github.com/mcoder2014/digital_watermarking)

本项目的主要目的是提供一种方便的为创作者的作品添加水印的工具，用来帮助创作者标注版权并且提供数字水印功能来给创作者一个可以追溯盗图等行为的能力(直接的盗图：如裁剪、简单调色等，大范围的PS及重创作无法消除水印)。

数字水印：数字水印（Digital Watermarking） 技术是将一些标识信息（即数字水印）直接嵌入数字载体当中（包括多媒体、文档、软件等）或是间接表示（修改特定区域的结构），且不影响原载体的使用价值，也不容易被探知和再次修改。但可以被生产方识别和辨认。通过这些隐藏在载体中的信息，可以达到确认内容创建者、购买者、传送隐秘信息或者判断载体是否被篡改等目的。数字水印是保护信息安全、实现防伪溯源、版权保护的有效办法，是信息隐藏技术研究领域的重要分支和研究方向。

# 使用方法 && 效果展示
## 功能说明
初期考虑功能：
1. UI模式：希望软件可以有 UI 界面可供可视化调整水印的位置。
2. 命令行批处理： 如果需要一次为多张照片追加水印的话，可以使用命令行批处理添加。

### 可见水印
- 一般图像水印：可以调整位置、大小、旋转、透明度
- 一般文字水印：可以调整位置、字体、旋转、透明度

### 不可见水印
- LSB水印:将一般图像水印用LSB方式不可见融合
- 直接频域空间数字水印:[频域手段添加数字盲水印的方法](https://www.zhihu.com/question/50735753)
- 有编码频域空间数字水印:在直接频域空间数字水印的基础上追加编码方案，减少产生的噪点。

## UI界面
## 命令行

# 安装方法

# 编译方法

# 反馈
如果在使用本软件时遇到某些莫名其妙的 bug 或者是有建议进行反馈的话，请前往[ISSUES](https://github.com/mcoder2014/digital_watermarking/issues)部分进行反馈。

# Reference
1. [瞎搞搞-数字水印（待填坑）](https://mcoder.cc/2019/04/07/digital_watermarking/)
2. [频域手段添加数字盲水印的方法](https://www.zhihu.com/question/50735753)
3. [隐写术：基于2D-FFT的图像数字水印技术](https://zhuanlan.zhihu.com/p/41306815)
4. [Yuxfei/Digital-Image-Watermark](https://github.com/Yuxfei/Digital-Image-Watermark)