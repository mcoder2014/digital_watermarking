# 配置文件的相关设计
使用json作为配置文件，配置文件主要用于设置水印样式，认为可以使用图片+文字作为可见的数字水印

为了能够利用 json 表示水印的组成，这里预先设计一点儿规则，用来指导后期编程。

```JSON
{
    "water_marking":[
        {
            "type":"img",
            "content":"src",
            "size": [300, 200],       // 尺寸 [width, height]
            "pos": [100, 100],        // 位置 [相对坐标]
            "anchor":"left-upper/left-middle/left-bottom/center-upper/center-middle/center-bottom/right-upper/right-middle/right-bottom"    // 坐标原点(相对于目标图片)
        },
        {
            "type":"text",
            "content":"XXX摄",
            "size":24,                    // 字体大小，初步估计使用 point size
            "font":"font-family",         // 首先允许选择特定的字体吧
            "color":[255,255,255,255],    // RGBA of Text
            "italic":false,               // 表示是否斜体
            "bold":false,                 // 表示是否加粗
            "underline":false             // 表示是否下划线
        }
    ]
}
```