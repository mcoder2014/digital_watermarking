# Watermarking
将水印抽象，共同继承于一个水印基类，通过工厂模式创建多个不同类型的水印，依次添加于图片。
为了之后更方便将核心代码移植与其他平台，这里尽量不要用到Qt的相关工具，而是使用STL。为了促进自己学习，指针使用STL的智能指针`unique_ptr/shared_ptr/weak_ptr`进行代替。

# 水印基类

WaterMark

