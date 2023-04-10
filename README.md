# AisDeployC-Demo

## Deploy Demo Code for Artificial Intelligence System（人工智能系统推理库Demo）

### Introduction and Description（介绍和说明）

<p align="center">
 <img src="./docs/assets/os_concat_logo.png">
</p>

- 本算法库发布Windows Linux 和 MacOs三个平台的库文件，目前支持C++和Python调用接口，具体接口文档在本页最下方链接中。本算法库会持续发布本算法库匹配的高精度算法模型和使用示例，算法库可以在本页ModelZoo中查找，使用示例在源文件的example当中。

- This algorithm library is released on Windows, Linux, and MacOs platforms, and currently supports C++ and Python calling interfaces. The specific interface documents can be found at the bottom link of this page. This algorithm library will continue to release high-precision algorithm models and usage examples matched with this algorithm library, which can be found in the ModelZoo section of this page. The usage examples can be found in the "example" folder of the source files### Trial and Authorization（试用和授权）

- 如果您想试用本算法库，请直接下载并进行试用。本算法库仅支持有限次数的调用，如果您超过了试用次数，将会自动保存未经授权的文件。如果您希望继续使用本算法库，请将文件和说明发送到我的电子邮箱。Email:  hit.zhou.j.h@gmail.com

- If you want to try this algorithm library, please download and try it directly. This algorithm library only supports limited number of calls. If you exceed the trial limit, unauthorized files will be automatically saved. If you want to continue using it, please send the files and instructions to my email.EMAIL: hit.zhou.j.h@gmail.com

### FAQ

[FAQ.md](docs/FAQ.md)

### Usage（使用方式)

#### 1. Environments installation（环境安装）

| Items           | Recommended version<br/>（推荐版本） | Resources<br/>（资源）                                                                                                                                                                                                                                                                                                                  |
| --------------- | ------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| System<br/>（系统） | Windows 10                     |                                                                                                                                                                                                                                                                                                                                     |
|                 | Ubuntu 18.04                   |                                                                                                                                                                                                                                                                                                                                     |
|                 | MacOS                          |                                                                                                                                                                                                                                                                                                                                     |
| IDE-Windows     | VS2017                         | 目前我们是使用VS2017编译的库，建议使用相同的版本                                                                                                                                                                                                                                                                                                         |
| CUDA            | 11.3                           | **No installation is required if you use a cpu.**<br/>**如果您使用cpu则不需要安装**.<br/>cuda_11.3.0_465.89_win10.exe: https://pan.baidu.com/s/18AvyfSq-dUl7egc0CsGYKQ?pwd=j9q2 提取码: j9q2<br/>cuda_11.3.0_465.19.01_linux.run：链接: https://pan.baidu.com/s/1cdMCnZ1FoNv9eVrun39kLQ?pwd=mh6s 提取码: mh6s                                           |
| cudnn           | 8.8                            | **No installation is required if you use a cpu.**<br/>**如果您使用cpu则不需要安装**.<br/>cudnn-linux-x86_64-8.8.0.121_cuda11-archive.tar.xz：链接: https://pan.baidu.com/s/1S_4VYj63R3zCSJyGDQwsmg?pwd=bgvl 提取码: bgvl<br/>cudnn-windows-x86_64-8.8.1.3_cuda11-archive.zip: 链接: https://pan.baidu.com/s/11P64Ks8IDfxmvhAjPaCusg?pwd=3tdl 提取码: 3tdl |

##### Cuda & Cudnn Install（Cuda Cudnn 安装）

Windows:

- Step1（步骤1）：Use cuda_11.3.0_465.89_win10.exe  to install, make sure to check if your Environment variables have the path to CUDA as belows. (使用cuda_11.3.0_465.89_win10.exe安装，确保您的环境变量设置如下)
  
  - CUDA_PATH  — -> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0
  
  - CUDA_PATH_V11_03 — → C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0

- Step2（步骤2）：Extract cudnn-windows-x86_64-8.8.1.3_cuda11-archive.zip（解压 udnn-windows-x86_64-8.8.1.3_cuda11-archive.zip）
  
  - Copy the contents of the cuDNN folder to the path where we installed CUDA in Step1. We need the contents of the **bin**, **include** & **lib** folders from cuDNN to be inside the **bin**, **include** and **lib** folders of the CUDA directory （将cuDNN中内容拷贝到步骤1中CUDA位置中，需要将bin include 和 lib文件夹中文件从cuDNN中拷贝到cuda文件夹中相应位置中）

#### 2. Downloads(下载)

Please download the **latest library files** and **Demo source code** in Releases on the right side of the github homepage.

（请在github主页右侧的Releases中下载**最新库文件**和**Demo源代码**。）

- Library file naming method（库文件命名方式）：
  
  - linux：AisDeployC-linux-x64-gpu-version.zip
  
  - windows: AisDeployC-windows-x64-gpu-version.zip

- Demo source code（源文件）：Source code (zip)

- 解压源代码，以下称为代码**工程目录**,假设目录地址为 your_dir/aisdeployc_demo

- 解压库文件，解压到代码**工程目录**下，假设解压目录为 your_dir/aisdeployc_demo/build

#### 3. Start Running (开始运行)

##### 3-a C++ Users (C++ 用户)

- 修改CMakeLists.txt
  
  - 修改底层库ONNX地址，目前底层库打包在库文件文件夹中，所以这里选择解压后的库文件地址
    
    - 根据你的系统，修改set(ONNX_RUNTIME_PATH your_dir/aisdeployc_demo/build)
    
    - Linux修改[Linux修改处，第6行]([AisDeployC-Demo/CMakeLists.txt at master · JinghuiZhou/AisDeployC-Demo · GitHub](https://github.com/JinghuiZhou/AisDeployC-Demo/blob/master/CMakeLists.txt#L6))
    
    - Windows修改[Windows修改处，第14行](https://github.com/JinghuiZhou/AisDeployC-Demo/blob/master/CMakeLists.txt#L14)
  
  - 修改AisDeployC库链接地址，目前库打包在库文件文件夹中，所以这里选择解压后的库文件地址
    
    - 根据你的系统，修改set(LINK_DIRECTORIES your_dir/aisdeployc_demo/build)
    
    - Linux修改[Linux修改处，第9行]([[AisDeployC-Demo/CMakeLists.txt at master · JinghuiZhou/AisDeployC-Demo · GitHub](https://github.com/JinghuiZhou/AisDeployC-Demo/blob/master/CMakeLists.txt#L9)))
    
    - Windows修改[Windows修改处，第27行]([AisDeployC-Demo/CMakeLists.txt at master · JinghuiZhou/AisDeployC-Demo · GitHub](https://github.com/JinghuiZhou/AisDeployC-Demo/blob/master/CMakeLists.txt#L27))

- Run Examples（运行示例）
  
  - example code path（示例代码路径）：[repo_root/example](https://github.com/JinghuiZhou/AisDeployC-Demo/tree/master/example)
  - 可执行文件会生成在 your_dir/aisdeployc_demo/build 中

##### 3-b Python Users（Python 用户）

- Run pytest examples（运行pytest示例）
  
  ```
  python -m pytest -s tests/test_interface.py
  ```

- Optional api（可选，形成api）
  
  **Server:**
  
  ```
  python example/api_server.py \
   --lib_path build/libAisDeployC.so \
   --port 9003 \
   --model tests/assets/models/det_setting_oen.aism \
   --license tests/assets/licenses/registed/linux_registed_info.aisl
  ```
  
  **Client**
  
  ```
  python tests/test_api_server.py \
   --image_path tests/assets/images/0_Parade_marchingband_1_100.jpg \
   --gateway_host /image/objectDetect
  ```

### Issues and Bugs（问题和缺陷）

- 如果您在使用本库时发现问题和缺陷，您可以直接在github上提issue，我们会尽快排查和修复。

- If you encounter any issues or bugs while using this library, you can directly raise an issue on Github, and we will investigate and fix it as soon as possible.

## Architecture Diagram（架构图）

<p align="center">
 <img src="./docs/assets/yuque_diagram.jpg">
</p>

## Model Zoo

工业 (industry)

#### 字符识别（OCR）

##### 字符分割 (char segmentation)

<p align="center">
 <img src="./docs/assets/industry_ocr_seg.png">
</p>

| 名称 Name          | 描述 Description | 链接 Link                                                                |
| ---------------- | -------------- | ---------------------------------------------------------------------- |
| industry_ocr_seg | 工业OCR字符分割      | 链接: https://pan.baidu.com/s/1xYxZATyMScFstU3dkoHXzw?pwd=91n6 提取码: 91n6 |

##### 字符分类 (char classification)

<p align="center">
 <img src="./docs/assets/industry_ocr_cls.jpeg">
</p>

| 名称 Name            | 描述 Description | 链接 Link                                                                |
| ------------------ | -------------- | ---------------------------------------------------------------------- |
| industry_ocr_recog | 工业OCR字符分类      | 链接: https://pan.baidu.com/s/1ZhfBMARR3JmYvCg654Yhjw?pwd=hj7e 提取码: hj7e |

通用 (general)

#### 人 (human)

##### 人脸 (face)

- 检测 (det)
  
  <p align="center">
  <img src="./docs/assets/face_det.jpg">
  </p>

| 名称 Name     | 描述 Description | 链接 Link                                                       |
| ----------- | -------------- | ------------------------------------------------------------- |
| face\-wider | 通用人脸检测         | 链接: https://pan.baidu.com/s/1ePv2JWleusR9Dah-0X4P5g 提取码: hvns |

- 人脸识别（特征嵌入&比对） recognition（embedding&comparision）
  
  <p align="center">  
  <img src="./docs/assets/face_embedding.png"> 
  </p>

| 名称 Name            | 描述 Description | 链接 Link                                                                |
| ------------------ | -------------- | ---------------------------------------------------------------------- |
| face_embedding_r27 | 通用人脸比对识别       | 链接: https://pan.baidu.com/s/1kVfX7T2FRgP37GoWG8_Pxg?pwd=2bk5 提取码: 2bk5 |

##### 人体（person）

- 检测 (det)

<p align="center">  
 <img src="./docs/assets/person_det.jpg">  
</p>

| 名称 Name      | 描述 Description | 链接 Link                                                       |
| ------------ | -------------- | ------------------------------------------------------------- |
| person-wider | 通用人体检测         | 链接: https://pan.baidu.com/s/19wSYA9aEQ1wwtHjfz0Vmzw 提取码: nfaa |

- 关键点 (pose)
  
  <p align="center">
  <img src="./docs/assets/pose_estimation.jpg">
  </p>

| 名称 Name     | 描述 Description | 链接 Link                                                       |
| ----------- | -------------- | ------------------------------------------------------------- |
| person-pose | 通用人体关键点        | 链接: https://pan.baidu.com/s/1Hg1F9codS2qX3EaU0ah0tg 提取码: sfk2 |

## User's documents

### C++

[AisDeployC: interface/cpp/interface.h File Reference](https://jinghuizhou.github.io/AisDeployC-Demo/html/interface_8h.html)

### Python

[AisDeployC: interface/python/interface.py File Reference](https://jinghuizhou.github.io/AisDeployC-Demo/html/classinterface_1_1_ais_deploy_c.html)
