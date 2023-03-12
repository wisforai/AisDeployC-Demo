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

from flask import Flask,request
app = Flask(__name__)


deploy_obj = None

@app.route("/image/objectDetect",methods=['POST'])
def imageObjectDetect():
    data = {
        "code": 0,
        "msg": "unknown error",
    }
    try:
        params = request.get_json()
    except:
        params = request.form

    # 请求参数
    # appKey = params.get("appKey")
    image_base64 = params.get("image_base64", None)  # 接收base64编码的图片

    if image_base64:
        file_json = {"type": "base64", "data": image_base64, "ch":3}
        input_json = {"data_list": [file_json]}
        ret_val = deploy_obj.process(input_json)

        data["result"] = ret_val
        data["code"] = 1000
        data["msg"] = "[INFO] success. {}".format(__name__)

    else:
        data["msg"] = "[ERROR] {}  key \'image_base64\' error, value not provided".format(__name__)

    return json.dumps(data,ensure_ascii=False)

if __name__ == "__main__":

    parse = argparse.ArgumentParser()
    parse.add_argument("--debug", type=int, default=0, help="whether to turn on debugging mode default:0")
    parse.add_argument("--processes", type=int, default=1, help="number of open processes default:1")
    parse.add_argument("--port", type=int, default=9003, help="service port default:9003")
    parse.add_argument("--lib_path", type=str, default="build/libAisDeployC.so", required=True, help="lib file path default:build/libAisDeployC.so")
    parse.add_argument("--model", type=str, default="", required=True, help="model file path")
    parse.add_argument("--license", type=str, default="", required=True, help="license file path")
    parse.add_argument("--gpu_id", type=int, default=0, help="gpu_id default:0")
    flags, unparsed = parse.parse_known_args(sys.argv[1:])

    debug = flags.debug
    processes = flags.processes
    port = flags.port
    model_path = flags.model
    license_path = flags.license
    lib_path = flags.lib_path
    gpu_id = flags.gpu_id

    debug = True if 1 == debug else False

    deploy_obj = AisDeployC(lib_path)

    ret = deploy_obj.model_initialize(model_path, gpu_id)
    assert ret == 0
    ret = deploy_obj.update_license(license_path)
    assert ret == 0, "[ERROR] update_license failed, please use a correct license file."


    app.run(host="0.0.0.0",port=port,debug=debug)