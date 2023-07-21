import argparse
import sys
import numpy as np
import base64
import tqdm
import cv2
import os

from PIL import Image, ImageDraw, ImageFont
sys.path.append(os.path.join(os.path.dirname(__file__), "../"))
from interface.python.interface import AisDeployC




def AddText(img, text, left, top, font_path, textColor=(255, 255, 0), textSize=20):
    if (isinstance(img, np.ndarray)): # 判断是否OpenCV图片类型
        img = Image.fromarray(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    # 创建一个可以在给定图像上绘图的对象
    draw = ImageDraw.Draw(img)
    # 字体的格式
    fontStyle = ImageFont.truetype(
        font_path, textSize, encoding="utf-8")
    # 绘制文本
    draw.text((left, top), text, textColor, font=fontStyle)
    # 转换回OpenCV格式
    return cv2.cvtColor(np.asarray(img), cv2.COLOR_RGB2BGR)

# visulize vis_list_bbox
# per_class_bbox_result is a list of dict, each dict has keys: bbox, category, score
# {'bbox': [87.80072021484375, 203.6754150390625, 139.88888549804688, 262.8230285644531], 'category': '人脸', 'score': 0.8517957925796509}
def vis_list_bbox(img, bbox_result, font_path,  color=(0, 255, 0), thickness=2):
    for per_bbox in bbox_result:
        bbox = per_bbox['bbox']
        # convert to int
        bbox = [int(x) for x in bbox]
        cv2.rectangle(img, (bbox[0], bbox[1]), (bbox[2], bbox[3]), color, thickness)
        # put text
        text = per_bbox['category'] + ': ' + str(round(per_bbox['score'],2))
        put_text_height = bbox[1] - 10
        # put text left top should be bbox[0], bbox[1] - 10, but if bbox[1] < 10, then put text at bbox[0], bbox[1] + 10
        if bbox[1] < 10:
            put_text_height = bbox[1] + 10
        img=AddText(img, text, bbox[0], put_text_height, font_path,  textSize=12)
    return img
def thereshold_list_bbox(bbox_result, thresh):
    out_result = list()
    for per_bbox in bbox_result:
        score = per_bbox["score"]
        if score > thresh:
            out_result.append(per_bbox)
    return out_result
if __name__ == "__main__":

    parse = argparse.ArgumentParser()
    parse.add_argument("--lib_path", type=str, default="build/libAisDeployC.so", required=True, help="lib file path default:build/libAisDeployC.so")
    parse.add_argument("--model", type=str, default="tests/assets/models/epoch_200_segmentor_setting_oen.aism", required=True, help="model file path")
    parse.add_argument("--license", type=str, default="", required=False, help="license file path")
    parse.add_argument("--gpu_id", type=int, default=0, help="gpu_id default:0")
    parse.add_argument("--video_path", type=str, default=0, required=True, help="video_path")
    parse.add_argument("--vis_dir", type=str, default=0, required=True, help="vis_dir")
    parse.add_argument("--font_path", type=str, default="tests/assets/fonts/Alibaba-PuHuiTi-Light.ttf", required=False, help="font_path")
    parse.add_argument("--fps", type=int, default=25, required=False, help="fps")
    flags, unparsed = parse.parse_known_args(sys.argv[1:])

    print("[INFO] Running with the following arguments:")
    print(flags)
    model_path = flags.model
    license_path = flags.license
    lib_path = flags.lib_path
    gpu_id = flags.gpu_id
    video_path = flags.video_path
    vis_dir = flags.vis_dir
    font_path = flags.font_path
    fps = flags.fps

    model_path = os.path.abspath(model_path)
    license_path = os.path.abspath(license_path)
    lib_path = os.path.abspath(lib_path)
    video_path = os.path.abspath(video_path)
    vis_dir = os.path.abspath(vis_dir)
    font_path = os.path.abspath(font_path)

    if not os.path.exists(model_path):
        print("[ERROR] model_path: {} is not exist".format(model_path))
        exit(0)

    if not os.path.exists(lib_path):
        print("[ERROR] lib_path: {} is not exist".format(lib_path))
        exit(0)

    if not os.path.exists(font_path):
        print("[ERROR] font_path: {} is not exist".format(font_path))
        exit(0)

    if not os.path.exists(vis_dir):
        os.makedirs(vis_dir)
    print("[INFO] load lib_path: {}".format(lib_path))
    deploy_obj = AisDeployC(lib_path)
    print("[INFO] load model_path: {}".format(model_path))
    ret = deploy_obj.model_initialize(model_path, gpu_id)
    assert ret == 0

    license_flag = False
    if os.path.exists(license_path):
        # ensure license file is not a directory
        if os.path.isfile(license_path):
            license_flag = True

    if license_flag:
        ret = deploy_obj.update_license(license_path)
        assert ret == 0, "[ERROR] update_license failed, please use a correct license file."
    else:
        print("\n---------------------------------------------------------------------------------------------------------------")
        print("\t[WARN] 您尚未给入参数 \'license\', 猜测到您可能是想试用。没有提供授权文件，则处理次数会受限制")
        print("\t[WARN] Hi, input arg is not provided:  \'license\', if license not provided, process iteration number is limited.")
        print("\t[WARN] 不用担心, 待授权文件 unregisted_info.aisl，稍后若干次处理调用后会自动生成，您可以联系我获得授权 Email: hit.zhou.j.h@gmail.com")
        print("\t[WARN] Don't worry. unauthorized license file, named unregisted_info.aisl, will be saved automatically in several process usages，you can send it to me for the authorized file. Email: hit.zhou.j.h@gmail.com")
        print("\n---------------------------------------------------------------------------------------------------------------")
        ret = deploy_obj.generate_license()
        assert ret == 0, "[ERROR] generate_license failed"

    # ensure the vis_dir is exist
    if not os.path.exists(vis_dir):
        os.makedirs(vis_dir)
    cap = cv2.VideoCapture(video_path)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    save_path = os.path.join(vis_dir, "output.mp4")
    image_tmp_path = os.path.join(vis_dir, "tmp.jpg")
    vis_tmp_path = os.path.join(vis_dir, "tmp_vis.jpg")

    video_writer = cv2.VideoWriter(save_path, fourcc, fps, (640, 480))
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    for i in tqdm.tqdm(range(total_frames)):
        ret, frame = cap.read()
        if not ret:
            break
        cv2.imwrite(image_tmp_path, frame)
        input_json = {"data_list": []}
        encoded_image_byte = cv2.imencode(".jpg", frame)[1].tobytes()
        image_base64 = base64.b64encode(encoded_image_byte)
        image_base64 = image_base64.decode("utf-8")  # str类型
        file_json = {"type": "base64", "data": image_base64, "ch":3}
        input_json["data_list"].append(file_json)
        ret_val = deploy_obj.process(input_json)
        image_vis = cv2.imread(image_tmp_path)
        if ret_val is None:
            continue
        res = ret_val[0]
        # print("res: ", res)
        if res is None:
            continue
        image_vis = vis_list_bbox(image_vis, res, font_path)

        image_vis = cv2.resize(image_vis, (640, 480))
        cv2.imwrite(vis_tmp_path, image_vis)
        video_writer.write(image_vis)
    video_writer.release()


