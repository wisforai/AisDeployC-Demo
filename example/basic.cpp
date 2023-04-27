//
// Created by Jinghui Zhou on 2023/2/3.
//
#include <iostream>
#include "opencv2/opencv.hpp"
#include "interface/interface.h"
using namespace std;
using namespace cv;
int main(int argc, char** argv) {
    void * ptrDeploy = nullptr;
    // print this program's path and name
    cout<<"[INFO] Now Process argv[0]: "<<argv[0]<<endl;
    cout<<"Step [0]: get_version and check_version."<<endl;
    std::string version = get_version();
    cout<<"\tVersion: "<<version<<endl;
    int ret_0 = check_version();

    cout<<"Step [1.1]: initialize."<<endl;
    int initRet = 0;
    string current_path_str = ".";
    std::string model_path = current_path_str+"/../tests/assets/models/epoch_200_segmentor_setting_oen.aism";
    int gpu_id = 0;
    ptrDeploy = initialize(model_path.c_str(), gpu_id, &initRet);

    cout<<"Step [1.2]: update_license."<<endl;
    int ret_update =  update_license(ptrDeploy, "../tests/assets/licenses/registed/windows_registed_info.aisl");

    cout<<"Optional Step [1.3]: generate_license."<<endl;
    int ret_generate = generate_license(ptrDeploy);

    cout<<"Step [2.1]: process_images. batch=1"<<endl;
    String str = current_path_str+"/../tests/assets/images/1.jpg";
    Mat image = imread(str);
    std::vector<cv::Mat> imgs;
    imgs.push_back(image);
    int ret = process_images(ptrDeploy, imgs);

    cout<<"Step [2.2]: process_images. batch=2"<<endl;
    String str_1 = current_path_str+"/../tests/assets/images/63_1024.jpg";
    Mat image_1 = imread(str_1);
    imgs.push_back(image_1);
    int ret_1 = process_images(ptrDeploy, imgs);
    cout<<"Step [2.3]: process_images. batch=2. try again"<<endl;
    ret_1 = process_images(ptrDeploy, imgs);
    cout<<"\t[INFO] process_images result="<<ret_1<<"\tO: process success\t400: failure."<<endl;

    cout<<"Optional Step [3.1]: get_categories."<<endl;
    std::vector<std::string> out_categories;
    int ret_2 = get_categories(
            ptrDeploy,
            out_categories
    );
    cout<<"\tout_categories: ";
    for(auto c : out_categories) cout<<" "<<c;
    cout<<endl;

    cout<<"Optional Step [3.1]: get_per_category_binary_masks."<<endl;
    std::vector<std::vector<cv::Mat> > per_category_masks;
    int ret_3 = get_per_category_binary_masks(
            ptrDeploy,
            per_category_masks
    );

    cout<<"Optional Step [3.2]: get_per_category_binary_masks."<<endl;
    std::vector<std::vector<cv::Mat> > per_category_score_maps;
    int ret_4 = get_per_category_score_maps(
            ptrDeploy,
            per_category_score_maps
    );

    cout<<"Optional Step [3.3]: get_per_category_binary_masks."<<endl;
    int ret_5 = get_per_category_score_maps(
            ptrDeploy,
            per_category_score_maps
    );

    cout<<"Optional Step [3.4]: get_binary_mask_from_score_map."<<endl;
    String str_2 = current_path_str+"/../tests/assets/images/sem_score.jpg";
    Mat image_2 = imread(str, IMREAD_GRAYSCALE);
    cv::Mat score_map;
    image_2.convertTo(score_map, CV_32FC1, 1.0 / 255, 0);

    cv::Mat binary_mask;
    float thresh = 0.1;

    int ret_6 = get_binary_mask_from_score_map(
            score_map,
            binary_mask,
            thresh
    );

    cout<<"Optional Step [3.5]: erode_binary_mask."<<endl;
    String str_3 = current_path_str+"/../tests/assets/images/sem_mask.jpg";

    Mat binary_mask_1 = imread(str, IMREAD_GRAYSCALE);

    int kernel_size = 7;
    int iter_erode = 2;

    int ret_7 = erode_binary_mask(
            binary_mask_1,
            kernel_size,
            iter_erode
    );

    cout<<"Optional Step [3.6]: dilate_binary_mask."<<endl;
    String str_4 = current_path_str+"/../tests/assets/images/sem_mask_1.jpg";

    Mat binary_mask_2 = imread(str, IMREAD_GRAYSCALE);

    int kernel_size_1 = 3;
    int iter_dilate = 1;

    int ret_8 = dilate_binary_mask(
            binary_mask,
            kernel_size,
            iter_dilate
    );

    cout<<"Optional Step [3.7]: cal_per_category_instances."<<endl;


    String str_5 = current_path_str+"/../tests/assets/images/sem_mask_1.jpg";

    Mat binary_mask_3 = imread(str_5, IMREAD_GRAYSCALE);

    String str_6 = current_path_str+"/../tests/assets/images/sem_score.jpg";
    Mat image_3 = imread(str_6, IMREAD_GRAYSCALE);
    cv::Mat score_map_1;
    image_3.convertTo(score_map_1, CV_32FC1, 1.0 / 255, 0);

    std::vector<std::vector<cv::Mat> >  per_category_score_maps_1;
    std::vector<std::vector<cv::Mat> > per_category_masks_1;

    std::vector<cv::Mat> tmp;
    tmp.push_back(score_map_1);
    per_category_score_maps_1.push_back(tmp);

    std::vector<cv::Mat> tmp1;
    tmp1.push_back(binary_mask_3);
    per_category_masks_1.push_back(tmp1);


    int ret_9 = cal_per_category_instances(
            ptrDeploy,
            per_category_score_maps_1,
            per_category_masks_1
    );

    cout<<"Optional Step [3.8]: get_per_batch_instance_number."<<endl;
    std::vector<int>  per_batch_instance_number;
    int ret_10 = get_per_batch_instance_number(ptrDeploy, per_batch_instance_number);

    cout<<"Optional Step [3.9]: get_per_batch_instances."<<endl;
    std::vector<std::vector<bounding_box>> per_batch_instances;
    int ret_11 = get_per_batch_instances(ptrDeploy, per_batch_instances);
    for(auto a:per_batch_instances)
    {
        for(auto b:a){
            cout<<"\tbbox:  b.xmin "<<b.xmin<<" b.ymin "<<b.ymin<<" b.xmax "<<b.xmax<<" b.ymax "<<b.ymax<<" b.score "<<b.score<<" b.cat "<<b.category<<endl;
        }
    }

    cout<<"Step [4]: release."<<endl;
    int ret_ = release(ptrDeploy);

}