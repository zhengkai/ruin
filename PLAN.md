# PLAN

## 2025.12.05

前两天发现 box2d 里碰撞检测的一个 edge case 跟我大概 5 年前用 golang 写的一个小东西有相同的问题  
就是连续的多个小方块，不等于一长条方块，尽管边长都是整数的、致密的，角色在经过连接处会随机卡住  
因为我当时完全不懂这方面知识，只是按我自己想的实现个最笨的效果，没想到有 18 年历史的 box2d 也还是无法避免  
有点得意啊，原来当初真的碰到了一个很难的问题。可能试着把多个物体拼接起来能解决，不过还没尝试

今天解决了一个最大的问题，daily build 彻底跑通。以后随时可以下载并在任何一台 windows 上运行最新的 commit

打算先做点简单的东西，比如做完了手柄界面

跟祥基聊天有很多收获，他给了我几个网站 [graphtoy](https://graphtoy.com/)、[desmos](https://www.desmos.com/)、[easings](https://easings.net/zh-cn)

看到一条推[讲解了一些相机的知识](https://x.com/walaber/status/1996396504240808390)，很有启发

## 2025.12.01

完成了最基本的镜头跟随、角色移动，都是最简单最粗糙的实现。角色和场景的贴图也有偏差，需要重新校对

## 2025.11.28

[editor](https://zhengkai.github.io/ruin-editor/) 可以拖拽材质并导出 `map.json`，主体可以读 `map.json` 并正确显示 tile  
接下来是 camera 类，tile 显示多大（缩放倍数）、跟随角色移动什么的，都跟这个类有关

问了下祥基 editor，我自己没找到，果然有个 [mapeditor/tiled](https://github.com/mapeditor/tiled)
看来是处理这个事情的，不过问的时候 editor 基本写完了，先用着，以后再研究这个

## 2025.11.19

前些天有事，没想断了 10 天了  
考虑再三，还是需要做一个地图编辑器。很想用个已有的，但我觉得上手+扩展自己想要的，可能时间更久。主要我理解别人东西的能力太弱了

这是个大工程，刚建了仓库 [ruin-editor](https://github.com/zhengkai/ruin-editor)

## 2025.11.09

才搞明白 cpp 里 `*` 和 `&` 的区别，golang 里的指针非常简单，cpp 里有更多控制粒度，  
于是把之前的 context 从 `std::shared_ptr` 换成 `&`

cpp 里还有 `const &`，很可惜 golang 里没有。可以进一步限制读写范围。

所以今天功能没什么进展，主要是为熟悉语法交学费

## 2025.11.08

昨天开始尝试做不同的 spirte 动作，但一来发现不同的 spirte 切换需要保存状态，  
二来不同 spirte 需要更多的配置（如间隔帧），干脆搞起了 asset 类，所有配置写 json 里

另外起了个仓库 [ruin-asset](https://github.com/zhengkai/ruin-asset) 用来装素材，没用 git submodule 也没用版本号，先凑合来

## 2025.11.06

昨晚找到个 [spirte 图片](https://craftpix.net/freebies/free-shinobi-sprites-pixel-art/) 正好是我想要的格式  
于是 pong 的练手算告一段落，开始做 ruin 了

今天把加载 spirte 做完了，但还不能顺序 step，先随机展示了
