import argparse
import sys
import numpy as np
import base64
import json
import cv2
import os
import ctypes
import platform

sys.path.append(os.path.join(os.path.dirname(__file__), "../"))
from interface.python.interface import AisDeployC

# convert rle to mask
def rle2mask(rle, width, height):
    mask = np.zeros(width * height, dtype=np.uint8)
    array = np.asarray(rle)
    starts = array[0::2]
    lengths = array[1::2]
    current_position = 0
    for index, start in enumerate(starts):
        current_position += start
        mask[current_position:current_position + lengths[index]] = 255
        current_position += lengths[index]
    return mask.reshape((width, height)).transpose((1, 0))

# 分割mask为多个联通区域，返回每个联通区域mask的bbox
def mask2bbox(mask):
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    bbox_list = []
    for i in range(len(contours)):
        x, y, w, h = cv2.boundingRect(contours[i])
        bbox_list.append([x, y, x + w, y + h])
    return bbox_list

# visulize bbox_list
def vis_bbox(img, bbox_list, color=(0, 255, 0), thickness=2):
    for bbox in bbox_list:
        cv2.rectangle(img, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, thickness)
    return img

if __name__ == "__main__":

    parse = argparse.ArgumentParser()
    parse.add_argument("--lib_path", type=str, default="build/libAisDeployC.so", required=True, help="lib file path default:build/libAisDeployC.so")
    parse.add_argument("--encoder_model", type=str, default="/Users/zhoujinghui/CLionProjects/LargeFiles/sam_vit_b_01ec64_encoder-quant.aism", required=True, help="model file path")
    parse.add_argument("--decoder_model", type=str, default="tests/assets/models/sam_vit_b_01ec64_decoder-quant_merge.aism", required=True, help="model file path")
    parse.add_argument("--license", type=str, default="", required=False, help="license file path")
    parse.add_argument("--gpu_id", type=int, default=-1, help="gpu_id default:0")
    parse.add_argument("--image_path", type=str, default=0, required=True, help="image_path")
    parse.add_argument("--vis_dir", type=str, default=0, required=True, help="vis_dir")

    flags, unparsed = parse.parse_known_args(sys.argv[1:])

    print("[INFO] Running with the following arguments:")
    print(flags)
    encoder_model_path = flags.encoder_model
    decoder_model_path = flags.decoder_model
    license_path = flags.license
    lib_path = flags.lib_path
    gpu_id = flags.gpu_id
    image_path = flags.image_path
    vis_dir = flags.vis_dir

    encoder_model_path = os.path.abspath(encoder_model_path)
    decoder_model_path = os.path.abspath(decoder_model_path)

    license_path = os.path.abspath(license_path)
    lib_path = os.path.abspath(lib_path)
    image_path = os.path.abspath(image_path)
    vis_dir = os.path.abspath(vis_dir)

    if not os.path.exists(encoder_model_path):
        print("[ERROR] encoder_model_path: {} is not exist".format(encoder_model_path))
        exit(0)
    if not os.path.exists(decoder_model_path):
        print("[ERROR] decoder_model_path: {} is not exist".format(decoder_model_path))
        exit(0)

    if not os.path.exists(lib_path):
        print("[ERROR] lib_path: {} is not exist".format(lib_path))
        exit(0)

    if not os.path.exists(vis_dir):
        os.makedirs(vis_dir)
    print("[INFO] load lib_path: {}".format(lib_path))
    encoder_deploy_obj = AisDeployC(lib_path)
    decoder_deploy_obj = AisDeployC(lib_path)
    print("[INFO] load encoder_model_path: {}".format(encoder_model_path))
    ret = encoder_deploy_obj.model_initialize(encoder_model_path, gpu_id)
    assert ret == 0
    print("[INFO] load decoder_model_path: {}".format(decoder_model_path))
    ret = decoder_deploy_obj.model_initialize(decoder_model_path, gpu_id)

    license_flag = False
    if os.path.exists(license_path):
        # ensure license file is not a directory
        if os.path.isfile(license_path):
            license_flag = True

    if license_flag:
        ret = encoder_deploy_obj.update_license(license_path)
        assert ret == 0, "[ERROR] update_license failed, please use a correct license file."
        ret = decoder_deploy_obj.update_license(license_path)
        assert ret == 0, "[ERROR] update_license failed, please use a correct license file."
    else:
        print("\n---------------------------------------------------------------------------------------------------------------")
        print("\t[WARN] 您尚未给入参数 \'license\', 猜测到您可能是想试用。没有提供授权文件，则处理次数会受限制")
        print("\t[WARN] Hi, input arg is not provided:  \'license\', if license not provided, process iteration number is limited.")
        print("\t[WARN] 不用担心, 待授权文件 unregisted_info.aisl，稍后若干次处理调用后会自动生成，您可以联系我获得授权 Email: hit.zhou.j.h@gmail.com")
        print("\t[WARN] Don't worry. unauthorized license file, named unregisted_info.aisl, will be saved automatically in several process usages，you can send it to me for the authorized file. Email: hit.zhou.j.h@gmail.com")
        print("\n---------------------------------------------------------------------------------------------------------------")
        ret = encoder_deploy_obj.generate_license()
        assert ret == 0, "[ERROR] generate_license failed"

    # parse image_path, if image_path is a image file, then use cv2.imread to read it
    # if image_path is a video file, then split it into image list,example is as follows
    # 1.jpg or 1.png, 2.jpg or 2.png, 3.jpg or 3.png, ...
    split_num = image_path.split(",")
    image_list = []
    if len(split_num) > 1:
        image_list = []
        for i in range(len(split_num)):
            image_list.append(split_num[i])
    else:
        image_list = [image_path]
    # process batch=N
    input_json = {"data_list": []}
    for image_path in image_list:
        assert os.path.exists(image_path), "[ERROR] image_path: {} is not exist".format(image_path)
        f= open(image_path, 'rb')
        qrcode = base64.b64encode(f.read()).decode()
        f.close()
        file_json = {"type": "base64", "data": qrcode, "ch":3}
        input_json["data_list"].append(file_json)

    ret_val = encoder_deploy_obj.process(input_json)
    # print("[INFO] encoder process ret_val: {}".format(ret_val))



