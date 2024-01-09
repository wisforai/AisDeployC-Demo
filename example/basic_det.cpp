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
    std::string model_path = current_path_str+"/../tests/assets/models/tube_det.aism";
    int gpu_id = 0;
    ptrDeploy = initialize(model_path.c_str(), gpu_id, &initRet);


    cout<<"Step [2.1] (option1): process_images. batch=1"<<endl;
    String str = current_path_str+"/../tests/assets/images/tube_55.jpg";
    Mat image = imread(str);
    std::vector<cv::Mat> imgs;
    imgs.push_back(image);
    int ret = process_images(ptrDeploy, imgs);

    cout<<"Step [2.2] (option2): process_images. batch=2"<<endl;
    String str_1 = current_path_str+"/../tests/assets/images/tube_59.jpg";
    Mat image_1 = imread(str_1);
    imgs.push_back(image_1);
    int ret_1 = process_images(ptrDeploy, imgs);

    cout<<"Optional Step [3.1]: get_categories."<<endl;
    std::vector<std::string> out_categories;
    int ret_2 = get_categories(
            ptrDeploy,
            out_categories
    );
    cout<<"\tout_categories: ";
    for(auto c : out_categories) cout<<" "<<c;
    cout<<endl;


    cout<<"Optional Step [3.1]: get_per_batch_instances."<<endl;
    std::vector<std::vector<bounding_box>> per_batch_instances;
    int ret_11 = get_per_batch_instances(ptrDeploy, per_batch_instances);
    for(auto a:per_batch_instances)
    {
        cout<<"\tper_batch_instances: "<<endl;
        for(auto b:a){
            cout<<"\tbbox:  b.xmin "<<b.xmin<<" b.ymin "<<b.ymin<<" b.xmax "<<b.xmax<<" b.ymax "<<b.ymax<<" b.score "<<b.score<<" b.cat "<<b.category<<endl;
        }
    }

    cout<<"Step [4]: release."<<endl;
    int ret_ = release(ptrDeploy);

}