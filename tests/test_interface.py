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
    license_path = "E:\\licenses\\windows_registed_info.aisl"
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
        path_str = "/home/tzvtc/data/LargeFiles/face_embedding_r27_setting.aism"

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

def test_interface_gsam_embedding():
    path_str = None
    if "macOS" in platform.platform():
        path_str = "/Users/zhoujinghui/CLionProjects/LargeFiles/sam_vit_b_01ec64_encoder-quant.aism"
    elif "Windows" in platform.platform():
        path_str = "E:\\LargeFiles\\sam_vit_b_01ec64_encoder-quant.aism"
    elif "Linux" in platform.platform():
        path_str = "/home/tzvtc/data/LargeFiles/sam_vit_b_01ec64_encoder-quant.aism"

    deploy_obj = AisDeployC(lib_path)


    gpu_id = -1
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
    with open("tests/assets/images/_0_Parade_marchingband_1_100.json", "w") as f:
        json.dump(ret_val, f, indent=4)

def test_interface_gsam_decoder():
    path_str = "tests/assets/models/sam_vit_b_01ec64_decoder-quant_merge.aism"
    deploy_obj = AisDeployC(lib_path)
    gpu_id = -1
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    f = open("tests/assets/images/image_embedding.json", "r")
    image_embeddings = json.load(f)
    f.close()
    point_coords = [[[400, 150],[500, 250],[450, 200]]]
    point_labels = [[0, 3, 2]]
    # generate all zeros mask, shape is (1, 1, 256, 256), not use numpy
    mask_input = []
    tmp = []
    for i in range(256):
        tmp.append(0)
    for i in range(256):
        mask_input.append(tmp)
    mask_input = [mask_input]
    mask_input = [mask_input]


    orig_im_size = [[682, 1024]]

    has_mask_input = [0]
    input_json = {"image_embeddings": image_embeddings, "point_coords": point_coords, "point_labels": point_labels,
                    "mask_input": mask_input, "has_mask_input": has_mask_input, "orig_im_size": orig_im_size}
    ret = deploy_obj.process_decoder(input_json)
    print(ret)


def test_interface_cls():


    deploy_obj = AisDeployC(lib_path)

    path_str = "tests/assets/models/sft_recog_compose.aism"


    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    ret = deploy_obj.update_license(license_path)


    # process batch=1
    imgPth = "tests/assets/images/A.jpeg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}

    ret_val = deploy_obj.process(input_json)

    # process batch=2
    imgPth = "tests/assets/images/B.jpeg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    ret_val = deploy_obj.process(input_json)
    print(ret_val)

def test_interface_pose():


    deploy_obj = AisDeployC(lib_path)
    path_str = "tests/assets/models/human_pose_est_17p_mov2.aism"

    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0

    ret = deploy_obj.update_license(license_path)


    # process batch=1
    imgPth = "tests/assets/images/human-pose.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}

    ret_val = deploy_obj.process(input_json)

    # process batch=2
    imgPth = "tests/assets/images/skateboarder-orig.jpeg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json["data_list"].append(file_json)

    ret_val = deploy_obj.process(input_json)
    print(ret_val)

def test_generate_license():
    deploy_obj = AisDeployC(lib_path)

    path_str = "tests/assets/models/det_setting_oen.aism"
    gpu_id = 0
    ret = deploy_obj.model_initialize(path_str, gpu_id)
    assert ret == 0
    ret = deploy_obj.generate_license()
    assert ret == 0



