# Frequently Asked Questions (FAQ, 常见问题集)

## Q: # [Could not locate zlibwapi.dll. Please make sure it is in your library path](https://stackoverflow.com/questions/72356588/could-not-locate-zlibwapi-dll-please-make-sure-it-is-in-your-library-path)

A:

I was able to find a copy of the missing zlib DLL in the NVIDIA Nsight directory:

> C:\Program Files\NVIDIA Corporation\Nsight Systems 2022.4.2\host-windows-x64\zlib.dll

I copied and renamed it to:

> C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\bin\zlibwapi.dll

since that was already in my PATH environment variable. Doing that resolved my error.

## Q: # OSError:[WinError 193] %1不是一个有效的Win32应用程序

A:

DLL 必须与可执行文件的位数相匹配，在这里是指 Python 可执行文件。在 64 位的 Python 中加载一个 32 位的 DLL 将会失败。同样在32 位的 Python 中加载一个 64 位的 DLL 将会失败。如前所述，会出现 193 的错误。