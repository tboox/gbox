<p>
<div align="center">
  <h1>gbox</h1>

  <div>
    <a href="https://github.com/tboox/vm86/blob/master/LICENSE.md">
      <img src="https://img.shields.io/github/license/tboox/vm86.svg?colorB=f48041&style=flat-square" alt="license" />
    </a>
    <a href="https://gitter.im/tboox/tboox?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge">
      <img src="https://img.shields.io/gitter/room/tboox/tboox.svg?style=flat-square&colorB=96c312" alt="Gitter" />
    </a>
    <a href="https://t.me/joinchat/F2dIAw5iTkDUMwCQ_8CStw">
      <img src="https://img.shields.io/badge/chat-on%20telegram-blue.svg?style=flat-square" alt="Telegram" />
    </a>
    <a href="https://tboox.bearychat.com/signup/98bf6970b9f889d6ae3fbc3d50ee8a36">
      <img src="https://img.shields.io/badge/chat-on%20bearychat-brightgreen.svg?style=flat-square" alt="Bearychat" />
    </a>
    <a href="https://jq.qq.com/?_wv=1027&k=5hpwWFv">
      <img src="https://img.shields.io/badge/chat-on%20QQ-ff69b4.svg?style=flat-square" alt="QQ" />
    </a>
    <a href="http://tboox.org/donation/">
      <img src="https://img.shields.io/badge/donate-us-orange.svg?style=flat-square" alt="Donate" />
    </a>
  </div>

  <p>The Graphic Box Library</p>
</div>
</p>

## Introduction

gbox is a mutli-platform c graphic library. 

It is now in the early stages of development only for reference and learning.

If you are interesting to this project, please view the source code.

#### Features

- Support 2d vector drawing
- Support matrix transform
- Support fill and stroke for solid, gradient and image
- Support custom path and drawing
- Support float and fixed point computation
- Provide custom drawing devices
- Support OpenGL ES 1.0/2.0 
- Support soft algorithm for drawing
- Implement mesh structure and algorithm
- Implement complex polygon tessellator and triangulation
- Support odd and nonzero fill
- Implement polygon scanning algorithm
- Provide multi-platform windows(.e.g glut, sdl and x11, framebuffer, ios, android in the feature)

#### In the plans

- Draw svg images
- Implement anti-aliasing
- More drawing devices and windows
- Draw text 
- Development ui and game engine

#### Build

please install xmake first: [xmake](http://www.xmake.io)

    $ cd ./gbox
    $ xmake

#### Run

    $ xmake run core

#### Documents

* [Documents](https://github.com/tboox/gbox/wiki/documents)
* [Codes](https://github.com/tboox/gbox)

#### Contacts

- Email:   	    
	- waruqi@gmail.com
- Website: 	    
	- http://www.tboox.org


## 简介

GBOX是一个用c语言实现的多平台图形库，支持windows、linux、mac、ios、android以及其他嵌入式系统。

现在这个项目，正处于早期开发阶段，暂不提供使用，仅供参考学习

如果您感兴趣，可以阅读源码

#### 已实现的功能

- 矢量2d渲染，支持矩阵变换，画刷、画笔的设置和渲染
- 支持复杂矢量路径渲染和填充
- 支持纯色填充、梯度填充、图像填充（正在重构）
- 支持全定点或者浮点计算（可配置切换）
- 实现渲染设备扩展，目前支持gpu加速的opengl es 1.0/2.0设备、纯算法渲染的bitmap设备
- 实现复杂多边形分割（三角形分割、凸多边形分割），支持奇偶填充、非零填充等填充规则（这个算法花了我一年时间 =。=）
- 实现mesh结构和封装
- 实现多边形扫描算法
- 支持多平台窗口扩展，目前支持glut、sdl窗口，后续会支持（x11，framebuffer, ios/android，windows等原生窗口）

#### 后续工作

- svg矢量图形渲染（之前的版本实现过一整套，最近正在重构，所以暂时移除了）
- 反走样支持
- 实现更多平台窗口和渲染设备
- 实现字体渲染
- ui框架的实现
- 有时间的话，再整整游戏引擎，当然这个是后话，看心情和时间了。

#### 测试效果

- 目前基于opengl的渲染设备，绘制tiger.svg可达到60fps
- 基于bitmap的纯算法渲染设备，绘制tiger.svg可达到30-40fps（pc上测试，数据仅供参考）

#### 编译

请先安装: [xmake](http://www.xmake.io)

    cd ./gbox
    xmake

#### 运行

    $ xmake run core

#### 文档

* [Documents](https://github.com/tboox/gbox/wiki/documents)
* [Codes](https://github.com/tboox/gbox)

#### 联系

- Email:   	    
	- waruqi@gmail.com
- Website: 	    
	- http://www.tboox.org

