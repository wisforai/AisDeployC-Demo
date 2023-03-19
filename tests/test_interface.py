# !/usr/bin/env python
# -*- encoding: utf-8 -*-
"""
@File       :   test_interface.py
@Time       :   2022/2/8
@Author     :   jinghuizhou
@Description:
    接口测试
"""
import os
import sys
import ctypes
import platform
import base64
import json

sys.path.append(os.path.join(os.path.dirname(__file__), "../"))
from interface.python.interface import AisDeployC

lib_path = None
license_path = None

if "macOS" in platform.platform():
    lib_path = "./cmake-build-debug/libAisDeployC.dylib"
elif "Windows" in platform.platform():
    lib_path = "./cmake-build-release/AisDeployC.dll"
elif "Linux" in platform.platform():
    lib_path = "./build/libAisDeployC.so"

if "macOS" in platform.platform():
    license_path = "tests/assets/licenses/registed/macos_registed_info.aisl"
elif "Windows" in platform.platform():
    license_path = "tests/assets/licenses/registed/windows_registed_info.aisl"
elif "Linux" in platform.platform():
    license_path = "tests/assets/licenses/registed/linux_registed_info.aisl"

def test_interface_sem():


    deploy_obj = AisDeployC(lib_path)

    path_str = "tests/assets/models/epoch_200_segmentor_setting_oen.aism"
    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    ret = deploy_obj.update_license(license_path)

    # process batch=1
    imgPth = "tests/assets/images/1.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}

    ret_val = deploy_obj.process(input_json)

    # process batch=2
    imgPth = "tests/assets/images/63_1024.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    ret_val = deploy_obj.process(input_json)

def test_interface_det():


    deploy_obj = AisDeployC(lib_path)

    path_str = "tests/assets/models/det_setting_oen.aism"
    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    ret = deploy_obj.update_license(license_path)


    # process batch=1
    imgPth = "tests/assets/images/0_Parade_marchingband_1_100.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}

    ret_val = deploy_obj.process(input_json)

    # process batch=2
    imgPth = "tests/assets/images/0_Parade_marchingband_1_1004.jpeg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    ret_val = deploy_obj.process(input_json)

def test_interface_image_embedding():
    path_str = None
    if "macOS" in platform.platform():
        path_str = "/Users/zhoujinghui/CLionProjects/LargeFiles/face_embedding_r27_setting.aism"
    elif "Windows" in platform.platform():
        path_str = "E:\\LargeFiles\\face_embedding_r27_setting.aism"
    elif "Linux" in platform.platform():
        path_str = "/home/N3_3090U5/data/LargeFiles/face_embedding_r27_setting.aism"

    deploy_obj = AisDeployC(lib_path)


    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    ret = deploy_obj.update_license(license_path)


    # process batch=1
    imgPth = "tests/assets/images/Salma_Hayek_0001.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}

    # ret_val = deploy_obj.process(input_json)

    # process batch=3
    imgPth = "tests/assets/images/Salma_Hayek_0002.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    imgPth = "tests/assets/images/Martina_McBride_0004.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    ret_val = deploy_obj.process(input_json)


    data_list=list()
    key_list = ["Salma_Hayek_0001", "Salma_Hayek_0002", "Martina_McBride_0004"]

    for key, single_res in zip(key_list, ret_val):
        value = single_res[0]["embedding_vector"]
        file_json = {"key": key, "embedding_vector":value}
        data_list.append(file_json)
    input_json = {"data_list": data_list}

    ret_val = deploy_obj.load_keys_embeddings(input_json)

    ret_val = deploy_obj.compare_with_ground_embeddings(input_json)

    print(ret_val)




