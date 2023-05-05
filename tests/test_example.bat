set lib_path=cmake-build-release/AisDeployC.dll

set model=tests/assets/models/epoch_200_segmentor_setting_oen.aism
set image_path=tests/assets/images/1.jpg
set vis_dir=tests/assets/images/vis
echo "[INOF] Run example run_sem"
python example/run_sem.py --lib_path %lib_path% --model %model% --image_path %image_path% --vis_dir %vis_dir%

set model="./tests/assets/models/det_setting_oen.aism"
set image_path="./tests/assets/images/0_Parade_marchingband_1_100.jpg"
set vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_det"
python example/run_det.py --lib_path %lib_path% --model %model% --image_path %image_path% --vis_dir %vis_dir%

set model="./tests/assets/models/sft_recog_compose.aism"
set image_path="./tests/assets/images/A.jpeg"
set vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_cls"
python example/run_cls.py --lib_path %lib_path% --model %model% --image_path %image_path% --vis_dir %vis_dir%

set model="E:\\LargeFiles\\human_pose_est_17p_r50.aism"
set image_path="tests/assets/images/human-pose.jpg"
set vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_pose"
python example/run_pose.py --lib_path %lib_path% --model %model% --image_path %image_path% --vis_dir %vis_dir%


set model="E:\\LargeFiles\\face_embedding_r27_setting.aism"
set image_path="tests/assets/images/Salma_Hayek_0001.jpg,tests/assets/images/Salma_Hayek_0002.jpg,tests/assets/images/Martina_McBride_0004.jpg"
set vis_dir="./tests/assets/images/vis"
echo "[INOF] Run example run_face_recog"
python example/run_face_recog.py --lib_path %lib_path% --model %model% --image_path %image_path% --vis_dir %vis_dir%