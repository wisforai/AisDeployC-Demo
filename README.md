# AisDeployC-Demo

## Deploy Demo Code for Artificial Intelligence System（人工智能系统推理库Demo）

### Introduction and Description（介绍和说明）

- 本算法库发布Windows Linux 和 MacOs三个平台的库文件，目前支持C++和Python调用接口，具体接口文档在本页最下方链接中。本算法库会持续发布本算法库匹配的高精度算法模型和使用示例，算法库可以在本页ModelZoo中查找，使用示例在源文件的example当中。

- This algorithm library is released on Windows, Linux, and MacOs platforms, and currently supports C++ and Python calling interfaces. The specific interface documents can be found at the bottom link of this page. This algorithm library will continue to release high-precision algorithm models and usage examples matched with this algorithm library, which can be found in the ModelZoo section of this page. The usage examples can be found in the "example" folder of the source files

### Trial and Authorization（试用和授权）

- 如果您想试用本算法库，请直接下载并进行试用。本算法库仅支持有限次数的调用，如果您超过了试用次数，将会自动保存未经授权的文件。如果您希望继续使用本算法库，请将文件和说明发送到我的电子邮箱。Email:  hit.zhou.j.h@gmail.com

- If you want to try this algorithm library, please download and try it directly. This algorithm library only supports limited number of calls. If you exceed the trial limit, unauthorized files will be automatically saved. If you want to continue using it, please send the files and instructions to my email.EMAIL: hit.zhou.j.h@gmail.com

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
