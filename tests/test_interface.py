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
from interface.interface import AisDeployC

def test_interface():
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
        license_path = "tests/assets/licenses/registed/macos_registed_info.aisl"

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




