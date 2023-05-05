lib_path="./build/libAisDeployC.so"
#lib_path="./cmake-build-debug/libAisDeployC.dylib"

model="./tests/assets/models/epoch_200_segmentor_setting_oen.aism"
image_path="./tests/assets/images/1.jpg"
vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_sem"
work_dir=$(pwd)
cd ${work_dir} && python3 example/run_sem.py --lib_path ${lib_path} --model ${model} --image_path ${image_path} --vis_dir ${vis_dir}

model="./tests/assets/models/det_setting_oen.aism"
image_path="./tests/assets/images/0_Parade_marchingband_1_100.jpg"
vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_det"
work_dir=$(pwd)
cd ${work_dir} && python3 example/run_det.py --lib_path ${lib_path} --model ${model} --image_path ${image_path} --vis_dir ${vis_dir}

model="./tests/assets/models/sft_recog_compose.aism"
image_path="./tests/assets/images/A.jpeg"
vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_cls"
work_dir=$(pwd)
cd ${work_dir} && python3 example/run_cls.py --lib_path ${lib_path} --model ${model} --image_path ${image_path} --vis_dir ${vis_dir}

#model="/Users/zhoujinghui/CLionProjects/LargeFiles/human_pose_est_17p_r50.aism"
model="/home/tzvtc/data/LargeFiles/human_pose_est_17p_r50.aism"
image_path="tests/assets/images/human-pose.jpg"
vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_pose"
work_dir=$(pwd)
cd ${work_dir} && python3 example/run_pose.py --lib_path ${lib_path} --model ${model} --image_path ${image_path} --vis_dir ${vis_dir}

#model="/Users/zhoujinghui/CLionProjects/LargeFiles/face_embedding_r27_setting.aism"
model="/home/tzvtc/data/LargeFiles/face_embedding_r27_setting.aism"
image_path="tests/assets/images/Salma_Hayek_0001.jpg,tests/assets/images/Salma_Hayek_0002.jpg,tests/assets/images/Martina_McBride_0004.jpg"
vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_face_recog"
work_dir=$(pwd)
cd ${work_dir} && python3 example/run_face_recog.py --lib_path ${lib_path} --model ${model} --image_path ${image_path} --vis_dir ${vis_dir}