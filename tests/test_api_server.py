import os
import cv2
import argparse
import glob
import json
import requests
import time
import base64
import cv2

def parse_args():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("--image_path", default="", type=str, required=True, help="image_path")
    args = parser.parse_args()
    return args

def work(kwargs):
    filename = kwargs.get("image_path")

    appKey = "s84dsd#7hf34r3jsk@fs$d#$dd"
    backend_host = "http://127.0.0.1:9003"

    iteration_number = 1
    for _ in range(iteration_number):
        image = cv2.imread(filename)

        encoded_image_byte = cv2.imencode(".jpg", image)[1].tobytes()  # bytes类型
        image_base64 = base64.b64encode(encoded_image_byte)
        image_base64 = image_base64.decode("utf-8")  # str类型

        url = '%s/image/objectDetect'%backend_host

        t1 = time.time()
        params = {
            "appKey": appKey,
            "image_base64":image_base64,

        }

        res=requests.post(url,data=params)
        t2 = time.time()
        t = "spend %.5f 秒"%(t2 - t1)
        # print(t,res.status_code,res.content)
        print(t,res.status_code,res.json())






def main():
    args = parse_args()
    input_kwargs = dict(
        image_path = args.image_path,
    )
    work(input_kwargs)

if __name__ == "__main__":
    main()
