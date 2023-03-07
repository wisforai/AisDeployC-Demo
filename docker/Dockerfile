ARG PYTORCH="1.12.1"
ARG CUDA="11.3"
ARG CUDNN="8"

FROM pytorch/pytorch:${PYTORCH}-cuda${CUDA}-cudnn${CUDNN}-runtime

ENV TORCH_CUDA_ARCH_LIST="6.0 6.1 7.0 7.5 8.0 8.6+PTX" \
    TORCH_NVCC_FLAGS="-Xfatbin -compress-all" \
    CMAKE_PREFIX_PATH="$(dirname $(which conda))/../" \
    FORCE_CUDA="1" \
    TZ=Asia/Shanghai \
    DEBIAN_FRONTEND=noninteractive

RUN apt-get update\
    && apt-get install -y gnupg2

RUN apt-key del 7fa2af80 \
    && apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/3bf863cc.pub \
    && apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/machine-learning/repos/ubuntu1804/x86_64/7fa2af80.pub

RUN sed -i 's/http:\/\/archive.ubuntu.com\/ubuntu\//http:\/\/mirrors.aliyun.com\/ubuntu\//g' /etc/apt/sources.list

RUN apt-get update \
    && apt-get install -y wget libopencv-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple

RUN pip install --no-cache-dir pytest flask opencv-python

RUN wget https://github.com/microsoft/onnxruntime/releases/download/v1.8.1/onnxruntime-linux-x64-gpu-1.8.1.tgz \
    &&tar zxvf onnxruntime-linux-x64-gpu-1.8.1.tgz \
    &&cp -f onnxruntime-linux-x64-gpu-1.8.1/lib/libonnxruntime* /usr/lib/ \
    &&rm -rf onnxruntime-linux-x64-gpu-1.8.1.tgz \
    &&rm -rf onnxruntime-linux-x64-gpu-1.8.1

WORKDIR /docker_host