# 配置文件的相关设计
使用json作为配置文件，配置文件主要用于设置水印样式，认为可以使用图片+文字作为可见的数字水印

为了能够利用 json 表示水印的组成，这里预先设计一点儿规则，用来指导后期编程。

```json
{
    "water_marking":[
        {
            "type":"img",                   // 正常图片水印-最好使用png图片
            "content":"src",                // 水印文件的路径
            "size": [300, 200],             // 尺寸 [width, height]
            "relative":"none/width/height", // 是否使用相对大小描述方式
            "relative_size":0.3,            // 相对大小
            "pos": [100, 100],              // 位置 [相对坐标]
            "alpha":128,                    // 水印图片的合并半透明
            "relative_pos":false,
            "rotation":90,                  // 旋转值
        },
        {
            "type":"text",
            "content":"XXX摄",
            "size":24,                    // 字体大小，初步估计使用 point size
            "relative":"none/width/height", // 是否使用相对大小描述方式
            "relative_size":0.3,                 // 相对大小
            "pos": [100, 100],            // 位置 [相对坐标]
            "relative_pos":false,
            "rotation":90,                // 旋转值
            "font":"font-family",         // 首先允许选择特定的字体吧
            "color":[255,255,255,255],    // RGBA of Text
            "italic":false,               // 表示是否斜体
            "bold":false,                 // 表示是否加粗
            "underline":false,             // 表示是否下划线
        },
        {
            "type":"lsb",                   // lsb 类型隐藏水印
            "content":"src",                // 水印图片的路径
            "bits":2,                       // 水印方式使用的位数
            "size": [300, 200],             // 尺寸 [width, height]
            "relative":"none/width/height", // 是否使用相对大小描述方式
            "relative_size":0.3,            // 相对大小
            "pos": [100, 100],              // 位置 [相对坐标]
            "alpha":128,                    // 水印图片的合并半透明
            "relative_pos":false,
            "rotation":90,                  // 旋转值
        },
        {
            "type":"possion",               // possion无缝水印
            "content":"src",                // 水印文件的路径
            "size": [300, 200],             // 尺寸 [width, height]
            "relative":"none/width/height", // 是否使用相对大小描述方式
            "relative_size":0.3,            // 相对大小
            "pos": [100, 100],              // 位置 [相对坐标]
            "relative_pos":false,
            "rotation":90,                  // 旋转值
        },
    ]
}
```