# DFRobot_ISRModule
- [English Version](./README.md)

该产品是一款集成了GNSS和RTC芯片的模块。可接收北斗、GPS等多种卫星信号，获取准确的时间信息，并为RTC校时，以确保时间的高精度和稳定性。它为用户提供了一种简单、便捷的方式来校准和维护设备的时间，适用于各种需要精确时间同步的应用场景。
在许多应用领域，时间同步是非常重要的。例如大量的物联网设备之间、自动化系统之间以及做科学研究时，都需要准确且稳定的时间作为基础。

![正反面svg效果图](./resources/images/DFR0715.png) 

## 产品链接(https://www.dfrobot.com.cn/)

    SKU: DFR0715

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

  * 获取实时，初始时间需要用户设定，我们可以获取编译时间，自己设定，最小单位:s 
  * 设置闹钟，用户可以设置闹钟，可以在中断引脚处得到下降沿脉冲触发闹钟
  * 测量芯片温度，误差:±0.5℃


## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_ISRModule"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>

## 方法

```C++

```

## 兼容性

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino Uno        |      √       |                 |             |
Arduino MEGA2560   |      √       |                 |             |
Arduino Leonardo   |      √       |                 |             |
FireBeetle-ESP8266 |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |
FireBeetle-M0      |      √       |                 |             |
Micro:bit          |      √       | nonsupport uart |             |


## 历史

- 2024/01/08 - V1.0.0 版本

## 创作者

Written by qsjhyy(yihuan.huang@dfrobot.com), 2024. (Welcome to our website)