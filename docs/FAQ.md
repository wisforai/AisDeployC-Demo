# Frequently Asked Questions (FAQ, 常见问题集)

## Q: 无法打开文件“AisDeployC.lib”

| 严重性 | 代码      | 说明                     | 项目              | 文件                      | 行   | 禁止显示状态 |
| --- | ------- | ---------------------- | --------------- | ----------------------- | --- | ------ |
| 错误  | LNK1104 | 无法打开文件“AisDeployC.lib” | AisDeployC-Demo | C:\AisDeployC-Demo\LINK | 1   | <br>   |

A: 配置附加目录库

第一步解决方案资源管理器中切换到sln，如下图所示

第二步进入属性

第三步，配置附加库目录：

位置在链接器-常规-附加库目录

添加AisDeployC.lib库所在的文件夹（我的测试机器上是C:\AisDeployC-Demo\x64\Release）

## Q：Visual Studio 运行后直接退出，打印信息无法查看

A: 使用终端运行生成的 .exe

第一步，打开生成的exe所在位置

第二步，打开终端（按住键盘shift 在当前位置鼠标右键），选择在此处打开Powershell窗口

第三步，终端输入exe名称运行，根据提示修改路径等代码

## Q: 值“MT_StaticRelease”不匹配值“MD_DynamicRelease”

A:

visual studio 编译错误：

error LNK2038: 检测到“RuntimeLibrary”的不匹配项: 值“MT_StaticRelease”不匹配值“MD_DynamicRelease”

解决：

在工程上右键-》属性-》c/c++-》代码生成-》运行库中，

改成（release为MT，debug为MTD）,

如果有多个工程（主工程中包含动态或者静态链接库工程）都得同样设置。

## Q: # [error LNK2019: unresolved external symbol opencv](https://stackoverflow.com/questions/18360540/error-lnk2019-unresolved-external-symbol-opencv)

opencv库链接错误

第一步解决方案资源管理器中切换到sln

第二步进入属性

第三步，配置库目录

在VC++目录-库目录中

Check that your **Project -> Properties -> VC++ Directories -> Library Directories**,

增加opencv库目录路径

```
上级目录\本工程AisDeployC-Demo\thirdparty\opencv3\lib\Windows
```

第四步，添加附加依赖项

链接器-输入-附加依赖项

Next, check that the following libraries are included in your **Project -> Properties -> Linker -> Input -> Additional Dependencies** :

添加以下内容

```
AisDeployC.lib
IlmImf.lib
ippicvmt.lib
ippiw.lib
ittnotify.lib
libjasper.lib
libjpeg-turbo.lib
libpng.lib
libprotobuf.lib
libtiff.lib
libwebp.lib
opencv_imgproc3416.lib
opencv_imgcodecs3416.lib
opencv_core3416.lib
zlib.lib
```

## Q: [Could not locate zlibwapi.dll. Please make sure it is in your library path](https://stackoverflow.com/questions/72356588/could-not-locate-zlibwapi-dll-please-make-sure-it-is-in-your-library-path)

A:

I was able to find a copy of the missing zlib DLL in the NVIDIA Nsight directory:

> C:\Program Files\NVIDIA Corporation\Nsight Systems 2022.4.2\host-windows-x64\zlib.dll

I copied and renamed it to:

> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\bin\zlibwapi.dll

since that was already in my PATH environment variable. Doing that resolved my error.

## Q: # OSError:[WinError 193] %1不是一个有效的Win32应用程序

A:

DLL 必须与可执行文件的位数相匹配，在这里是指 Python 可执行文件。在 64 位的 Python 中加载一个 32 位的 DLL 将会失败。同样在32 位的 Python 中加载一个 64 位的 DLL 将会失败。如前所述，会出现 193 的错误。

## C:\Windows\System32\ntdll.dll”。无法查找或打开 PDB 文件

参考链接：[已加载“C:\Windows\System32\ntdll.dll”。无法查找或打开 PDB 文件。解决方法。_让思想去旅行的博客-CSDN博客](https://blog.csdn.net/weixin_42586210/article/details/88291438)

解决方法：  
（1）工具——选项；

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307102152742.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjU4NjIxMA==,size_16,color_FFFFFF,t_70)

（2）常规——启用源服务器支持

![在因为这里插入图片描述](https://img-blog.csdnimg.cn/20190307102358717.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjU4NjIxMA==,size_16,color_FFFFFF,t_70)

点击“是”；

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307104713701.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjU4NjIxMA==,size_16,color_FFFFFF,t_70)

（3）符号——Microsoft符号服务器

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307105839416.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjU4NjIxMA==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307105359860.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjU4NjIxMA==,size_16,color_FFFFFF,t_70)

## Q: LNK1169 找到一个或多个多重定义的符号

| 严重性 | 代码      | 说明             | 项目                                                                                       | 文件                                                                                           | 行   | 禁止显示状态 |
| --- | ------- | -------------- | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | --- | ------ |
| 错误  | LNK1169 | 找到一个或多个多重定义的符号 | C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CMakeLists.txt | C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\industrial_ocr.exe | 1   | <br>   |

A: 使用sln解决方案时发现cmakelist有多个main

可以的方式是注释CMakeLists.txt中不需要执行的example生成部分，在解决方案资源管理器中重新生成即可。

## Q: score 不属于 bounding_box 成员

A: Visual Studio对于interface.h文件的编码格式问题

解决方法是去掉其中的中文注释 或 将interface/interface.h里的换行符改成 Windows（CRLF)

## Q: [E:onnxruntime:, parallel_executor.cc:213 onnxruntime::ParallelExecutor::RunNodeAsync] Non-zero status code returned while running Loop node.

A：

输入图像有问题，可能是imread的路径不存在

## Q: # C1083: 无法打开包括文件: “opencv2/opencv.hpp”: No such file or directory

A:

项目->项目属性->C/C+±>常规->附加包含目录->输入opencv2文件夹的父目录

```
本工程目录/thirdparty/opencv3/include
```

## Q: # C1083: 无法打开包括文件: “interface/interface.h”: No such file or directory

## Q: 找不到windows sdk版本8.1

A:

参考链接：[VS2017 C++报错：找不到windows sdk版本8.1](https://blog.csdn.net/qq_41727666/article/details/84330959)

打开VS2017，点击工具——获取工具和更新

![](https://img-blog.csdnimg.cn/20181121215812984.png)

击C++，选取 c++桌面开发

右侧安装详细信息 勾选 Windows 8.1 SDK和UCRT SDK![](https://img-blog.csdnimg.cn/20181121215902729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNzI3NjY2,size_16,color_FFFFFF,t_70)

## Q: Windows Error 0xe06d7363

A:

推理库与系统不匹配，请检查您下载的release库的系统和您运行的系统是否一致。

## Q: 如何增加虚拟内存

A:

1、右键点击“我的电脑”，选择“属性”。

2、在打开的面板中依次打开“高级系统设置”—“高级”选项卡—“设置”—“高级”选项卡—“更改”，就进入了虚拟内存的设置界面。

3、将“自动管理所有驱动器的分页文件大小”选项前的方框里的勾取消，选择一个盘符（尽量是非系统盘），选择自定义大小，将初始大小和最大值尽量设置为同一数值（可以根据自己电脑的物理内存设置，如电脑物理内存是2GB，就在3GB到6GB之间选择一个数值），然后点击“设置”，最后点击“确定”并重启电脑。

以上就是有关电脑虚拟内存的内容了，其它版本的Windows系统可参考Win10来进行设置。