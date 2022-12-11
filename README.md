# Slam-Interface
**Slam-Interface** 是一个能够与[ROS](https://www.ros.org/)系统进行交互的UI界面，支持用户将常用的ROS系统操作（如：ROS程序的打开/关闭，ROS话题的监听，ROS服务的调用等）封装为简单的UI界面操作，方便自身，尤其是方便其他对ROS系统不了解的人使用ROS系统的相关功能。

## 1.背景
**Slam-Interface** 源于作者在实验室所做的SLAM相关项目。当时，作者常常需要在实验室的手持设备上运行一些SLAM算法，每次都需要在终端里输入若干指令来开启手持设备上IMU，LiDAR，Camera等传感器的ROS驱动程序以及SLAM程序。此外，如果要把SLAM结果传送到中央监视器上更好的显示时，通常还需要在bashrc文件中进行相应的配置以实现ROS的主从机制。这一系列操作对于作者这样的ROS高手来说自然不在话下，但当作者碰巧偷懒不去实验室，而实验室碰巧有小白同学想要玩玩SLAM设备或者向其它参观者展示我们的傲人成果时，问题就来了，我该如何让他在没有我的情况下也能顺利上手呢？无疑，我可以写一个文档（我曾经也是这样做的），文档以傻瓜式的叙述一步步地告诉他如果打开终端，输入什么指令，应当看到什么结果。但这种不优雅的解决方式始终令作者耿耿于怀，终于，作者决定以自己绵薄的码力，写一个勉强能用的UI界面来简化涉及到的操作，于是就有了如今的**Slam-Interface**。

## 2. 环境要求
### 2.1 **Ubuntu** and **ROS**
- **Ubuntu >= 16.04**
- **ROS    >= Melodic**

### 2.2. **Qt**
- **Qt    >= 5.12.8**。 Qt5的其它版本可以尝试，但不保证能够正常运行。 

## 3. 安装

## 4. 使用
整个界面如下图所示，从上到下共由四大模块组成：`Roscore模块`，`Sensors模块`，`SLAM&Tools模块`和`Command模块`.

<div align="center">
    <img src="img/overview.png" width=95% >
</div>

### 4.1 基础使用

#### 4.1.1 配置Roscore
- **配置ROS安装目录**：为使程序内部能够调用ROS的`setup.bash`，配置相应的ROS环境，需设置ROS的安装目录(作者的安装目录为`/opt/ros/noetic`)。
- **配置ROS主从机**：若需配置ROS主从机，则在主机和从机上都需设置环境变量`ROS_MASTER_URI`为主机的IP。在主机上，点击按钮`USE LOCAL IP`即可填入自身IP（对于从机则可以手动输入主机IP）。而如果不设计主从机，则可以点击按钮`USE LOCALHOST`使用本地回环地址（127，0.0.1）。
- **打开ROSCORE**：Roscore是ROS运行的基础，当配置好ROS安装目录和`ROS_MASTER_URI`变量后，便可点击`Roscore模块`的滑动按钮开启Roscore。
<video id="video" controls="" preload="none" poster="封面">
      <source id="mp4" src="img/roscore.mp4" type="video/mp4">
</videos>

### 4.2 进阶使用

## 5. 致谢
