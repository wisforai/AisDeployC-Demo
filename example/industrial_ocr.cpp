//
// Created by Jinghui Zhou on 2023/4/5.
//
#include <iostream>
#include<opencv2/opencv.hpp>
#include "interface/interface.h"
using namespace std;
using namespace cv;

# define _DEBUG_INDUSTRY_OCR_INFO

// a function to expand the object detection results with ratio, use the center of the box as the center of the expanded box
// input: boxes std::vector<std::vector<bounding_box>>, the object detection results
// input: x_ratio float, the ratio of the width of the expanded box to the width of the original box
// input: y_ratio float, the ratio of the height of the expanded box to the height of the original box
// output: boxes std::vector<std::vector<bounding_box>>, the expanded object detection results
int expand_boxes(std::vector<std::vector<bounding_box>>& boxes, float x_ratio, float y_ratio) {
    std::vector<std::vector<bounding_box>> out_boxes;
    for (int i = 0; i < boxes.size(); i++) {
        std::vector<bounding_box> out_perbatch_boxes;
        for (int j = 0; j < boxes[i].size(); j++) {
            float x1 = boxes[i][j].xmin;
            float y1 = boxes[i][j].ymin;
            float x2 = boxes[i][j].xmax;
            float y2 = boxes[i][j].ymax;
            std::string category = boxes[i][j].category;

            // check origin box is valid
            if (x1 >= x2 || y1 >= y2) {
                std::cout << "[WARN]expand_boxes origin invalid box: " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
                return -1;
            }
            float w = x2 - x1;
            float h = y2 - y1;
            float cx = x1 + w / 2;
            float cy = y1 + h / 2;
            float new_w = w * x_ratio;
            float new_h = h * y_ratio;
            bounding_box out_box;
            out_box.xmin = cx - new_w / 2;
            out_box.ymin = cy - new_h / 2;
            out_box.xmax = cx + new_w / 2;
            out_box.ymax = cy + new_h / 2;
            out_box.category = category;
            // check expanded box is valid
            if (out_box.xmin >= out_box.xmax || out_box.ymin >= out_box.ymax) {
                std::cout << "[WARN]expand_boxes expanded invalid box: " << out_box.xmin << " " << out_box.ymin << " " << out_box.xmax << " " << out_box.ymax << std::endl;
                return -1;
            }
            out_perbatch_boxes.push_back(out_box);
        }
        out_boxes.push_back(out_perbatch_boxes);
    }
    boxes = out_boxes;
    return 0;
}

// a function to crop the image with the object detection results
// input: imgs std::vector<cv::Mat>, the input images
// input: boxes std::vector<std::vector<bounding_box>>, the object detection results
// output: crops std::vector<std::vector<cv::Mat> >, the cropped images in batches

int crop_images(std::vector<cv::Mat>& imgs, std::vector<std::vector<bounding_box>>& boxes, std::vector<std::vector<cv::Mat> >& crops) {

    for (int i = 0; i < imgs.size(); i++) {
        std::vector<cv::Mat> crops_i;
        for (int j = 0; j < boxes[i].size(); j++) {
            // fix bug: x1, y1, x2, y2 should in the range of the image
            float x1 = boxes[i][j].xmin;
            float y1 = boxes[i][j].ymin;
            float x2 = boxes[i][j].xmax;
            float y2 = boxes[i][j].ymax;
            if (x1 <= 0) x1 = 0;
            if (y1 <= 0) y1 = 0;
            if (x2 > imgs[i].cols-1) x2 = imgs[i].cols-1;
            if (y2 > imgs[i].rows-1) y2 = imgs[i].rows-1;
//            cout<<"batch: "<<i<<" box: "<<j<<endl;
//            cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << endl;
            if(x1 >= x2 || y1 >= y2) {
                cout << "[WARN] invalid box: " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
                continue;
            }

            cv::Rect rect(x1, y1, x2 - x1, y2 - y1);
            cv::Mat crop = imgs[i](rect);
            crops_i.push_back(crop);
        }
        crops.push_back(crops_i);
    }
    return 0;
}

// warp the process in main function
// initialize the segmentation model and recognition model
// input segmentation_model_path std::string, the path of the segmentation model
// input recognition_model_path std::string, the path of the recognition model
// input license_path std::string, the path of the license
// input gpu_id int, the gpu id
// output ptrDeploySeg void*, the pointer of the segmentation model
// output ptrDeployCls void*, the pointer of the recognition model

int initialize_models(
        std::string segmentation_model_path,
        std::string recognition_model_path,
        std::string license_path,
        int gpu_id,
        void*& ptrDeploySeg,
        void*& ptrDeployCls
){

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [0]: get_version and check_version."<<endl;
#endif

    std::string version = get_version();

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"\tVersion: "<<version<<endl;
#endif
    int ret_0 = check_version();

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    // Load ocr char segmentation model
    cout<<"[INOF] Load ocr char segmentation model."<<endl;
    cout<<"Step [1.1]: segmentation model initialize."<<endl;
#endif
    int initRet = 0;
    ptrDeploySeg = initialize(segmentation_model_path.c_str(), gpu_id, &initRet);
    if (initRet != 0) {
        cout << "Failed to initialize segmentation model." << endl;
        return initRet;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [1.2]: segmentation model update_license."<<endl;
#endif
    int ret_update =  update_license(ptrDeploySeg, license_path.c_str());
    if (ret_update != 0) {
        cout << "[WARN] Failed to update license." << endl;
    }

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    // Load ocr char recognition model
    cout<<"[INOF] Load ocr char recognition model."<<endl;
    cout<<"Step [1.3]: recognition model initialize."<<endl;
#endif

    ptrDeployCls = initialize(recognition_model_path.c_str(), gpu_id, &initRet);
    if (initRet != 0) {
        cout << "Failed to initialize recognition model." << endl;
        return initRet;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [1.4]: recognition model update_license."<<endl;
#endif
    ret_update =  update_license(ptrDeployCls, license_path.c_str());
    if (ret_update != 0) {
        cout << "[WARN] Failed to update license." << endl;
    }
    return 0;

}

// a function to do the ocr process
// input: imgs std::vector<cv::Mat>, the input images
// input: ptrDeploySeg void*, the pointer of the segmentation model
// input: ptrDeployCls void*, the pointer of the recognition model
// output: results std::vector<std::vector<bounding_box>>, the ocr results, bounding_box is defined in the header file

int do_ocr(std::vector<cv::Mat>& imgs, void* ptrDeploySeg, void* ptrDeployCls, std::vector<std::vector<bounding_box>>& results) {
    int ret=0;
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [2.1]: process_images."<<endl;
#endif

    ret = process_images(ptrDeploySeg, imgs);
    if (ret != 0) {
        cout << "[INFO] Failed to ptrDeploySeg process images." << endl;
        return ret;
    }

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [3.1]: get_per_category_binary_masks."<<endl;
#endif
    std::vector<std::vector<cv::Mat> > per_category_masks;
    ret = get_per_category_binary_masks(
            ptrDeploySeg,
            per_category_masks
    );
    if (ret != 0) {
        cout << "[INFO] Failed to get per category binary masks." << endl;
        return ret;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [3.2]: per_category_score_maps."<<endl;
#endif
    std::vector<std::vector<cv::Mat> > per_category_score_maps;
    ret = get_per_category_score_maps(
            ptrDeploySeg,
            per_category_score_maps
    );
    if (ret != 0) {
        cout << "[INFO] Failed to get per category score maps." << endl;
        return ret;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [3.3]: cal_per_category_instances."<<endl;
#endif
    ret = cal_per_category_instances(
            ptrDeploySeg,
            per_category_score_maps,
            per_category_masks
    );
    if (ret != 0) {
        cout << "[INFO] Failed to cal per category instances." << endl;
        return ret;
    }

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [3.4]: get_per_batch_instances."<<endl;
#endif
    std::vector<std::vector<bounding_box>> per_batch_instances;
    ret = get_per_batch_instances(ptrDeploySeg, per_batch_instances);
    if (ret != 0) {
        cout << "[INFO] Failed to get per batch instances." << endl;
        return ret;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [3.5]: expand_per_batch_instances."<<endl;
#endif
    float x_ratio = 1.4;
    float y_ratio = 1.6;
    ret = expand_boxes(per_batch_instances, x_ratio, y_ratio);
    if (ret != 0) {
        cout << "[INFO] Failed to expand per batch instances." << endl;
        return ret;
    }

#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Optional: visuliaze the object detection results in the image and save the image."<<endl;
    for(int batch_i=0;batch_i<per_batch_instances.size();batch_i++)
    {
        for(int bbox_j=0;bbox_j<per_batch_instances[batch_i].size();bbox_j++)
        {
            cout<<"\tbatch "<<batch_i<<" bbox index "<<bbox_j<<" ";
            cout<<per_batch_instances[batch_i][bbox_j].xmin<<" "<<per_batch_instances[batch_i][bbox_j].ymin<<" "<<per_batch_instances[batch_i][bbox_j].xmax<<" "<<per_batch_instances[batch_i][bbox_j].ymax<<" "<<per_batch_instances[batch_i][bbox_j].score;
            cout<<" "<<per_batch_instances[batch_i][bbox_j].category<<endl;
        }
    }

    // visualize the object detection results in the image and save the image
    // clone the images
    std::vector<cv::Mat> imgs_clone;
    for (int i = 0; i < imgs.size(); i++) {
        imgs_clone.push_back(imgs[i].clone());
    }
    for (int i = 0; i < per_batch_instances.size(); i++) {
        for (int j = 0; j < per_batch_instances[i].size(); j++) {
            cv::rectangle(imgs_clone[i], cv::Point(per_batch_instances[i][j].xmin, per_batch_instances[i][j].ymin),
                          cv::Point(per_batch_instances[i][j].xmax, per_batch_instances[i][j].ymax),
                          cv::Scalar(0, 255, 0), 3);
            cv::putText(imgs_clone[i], std::to_string(per_batch_instances[i][j].score),
                        cv::Point(per_batch_instances[i][j].xmin, per_batch_instances[i][j].ymin),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
    }

    // save the images in batch
    for(int i=0;i<imgs_clone.size();i++)
    {
        std::string img_name = "industrial_seq_seg_result_" + std::to_string(i) + ".png";
        cv::imwrite(img_name, imgs_clone[i]);
    }

    cout<<"Step [4.1]: crop_images for char regions."<<endl;
#endif
    // use the segmentation results to crop the char regions
    std::vector<std::vector<cv::Mat> > per_batch_char_regions;
    ret = crop_images(imgs, per_batch_instances, per_batch_char_regions);
    if (ret != 0) {
        cout << "[INFO] Failed to crop images." << endl;
        return ret;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [4.2]: process_images for char regions."<<endl;
#endif
    int j = 0;
    for(auto ori_per_batch_char_regions:per_batch_char_regions)
    {
        ret = process_images(ptrDeployCls, ori_per_batch_char_regions);
        if (ret != 0) {
            cout << "[INFO] Failed to ptrDeployCls process images." << endl;
            return ret;
        }
        // get the char recognition results
        std::vector<classification> per_batch_cls;
        ret = get_per_batch_classification(ptrDeployCls, per_batch_cls);
        if (ret != 0) {
            cout << "[INFO] Failed to get per batch classification." << endl;
            return ret;
        }
        // modify the per_batch_instances with the char recognition results
        for (int i = 0; i < per_batch_cls.size(); i++) {
            per_batch_instances[j][i].category = per_batch_cls[i].category;
            per_batch_instances[j][i].score = per_batch_cls[i].score;
        }
        j++;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Optional: visuliaze the ocr results in the image and save the image."<<endl;
    for(int batch_i=0;batch_i<per_batch_instances.size();batch_i++)
    {
        for(int bbox_j=0;bbox_j<per_batch_instances[batch_i].size();bbox_j++)
        {
            cout<<"\tbatch "<<batch_i<<" bbox index "<<bbox_j<<" ";
            cout<<per_batch_instances[batch_i][bbox_j].xmin<<" "<<per_batch_instances[batch_i][bbox_j].ymin<<" "<<per_batch_instances[batch_i][bbox_j].xmax<<" "<<per_batch_instances[batch_i][bbox_j].ymax<<" "<<per_batch_instances[batch_i][bbox_j].score;
            cout<<" "<<per_batch_instances[batch_i][bbox_j].category<<endl;
        }
    }
    // visualize the object detection results in the image and save the image
    // clone the images
    std::vector<cv::Mat> imgs_clone_ocr;
    for (int i = 0; i < imgs.size(); i++) {
        imgs_clone_ocr.push_back(imgs[i].clone());
    }
    for (int i = 0; i < per_batch_instances.size(); i++) {
        for (int j = 0; j < per_batch_instances[i].size(); j++) {
            cv::rectangle(imgs_clone_ocr[i], cv::Point(per_batch_instances[i][j].xmin, per_batch_instances[i][j].ymin),
                          cv::Point(per_batch_instances[i][j].xmax, per_batch_instances[i][j].ymax),
                          cv::Scalar(0, 255, 255), 3);
            // put category text for each rectangle

            std::string text = per_batch_instances[i][j].category;
            cv::putText(imgs_clone_ocr[i], text, cv::Point(per_batch_instances[i][j].xmin, per_batch_instances[i][j].ymin),
                        cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 255, 0), 3);
            // put score text for each rectangle
            std::string score = std::to_string(per_batch_instances[i][j].score);
            cv::putText(imgs_clone_ocr[i], score, cv::Point(per_batch_instances[i][j].xmin, per_batch_instances[i][j].ymin + 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

        }
    }
    // save the images in batch
    for(int i=0;i<imgs_clone_ocr.size();i++)
    {
        std::string img_name = "industrial_seq_ocr_result_" + std::to_string(i) + ".png";
        cv::imwrite(img_name, imgs_clone_ocr[i]);
    }
#endif
    return 0;
}
// warp the release_models function
int release_models(void * ptrDeploySeg, void * ptrDeployCls) {
    int ret = 0;
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [5.1]: release segmentation model."<<endl;
#endif
    ret = release(ptrDeploySeg);
    if (ret != 0) {
        cout << "[INFO] Failed to release segmentation model." << endl;
        return ret;
    }
#ifdef _DEBUG_INDUSTRY_OCR_INFO
    cout<<"Step [5.2]: release classification model."<<endl;
#endif
    ret = release(ptrDeployCls);
    if (ret != 0) {
        cout << "[INFO] Failed to release classification model." << endl;
        return ret;
    }
    return 0;
}

int main(int argc, char** argv) {
    void * ptrDeploySeg = nullptr;
    void * ptrDeployCls = nullptr;

    cout<<"[INFO] Now Process argv[0]: "<<argv[0]<<endl;

    // intialization parameters
    string current_path_str = ".";
    std::string segmentation_model_path = current_path_str+"/../tests/assets/models/industry_ocr_seg.aism";
    std::string recognition_model_path = current_path_str+"/../tests/assets/models/sft_recog_compose.aism";
    std::string license_path = current_path_str+"/../tests/assets/licenses/registed/windows_registed_info.aisl";


    // initialize_models
    int ret = initialize_models(
            segmentation_model_path,
            recognition_model_path,
            license_path,
            0,
            ptrDeploySeg,
            ptrDeployCls
    );
    if (ret != 0) {
        cout << "[ERROR] Failed to initialize models." << endl;
        return ret;
    }

    // load the images to be processed
    String str = current_path_str+"/../tests/assets/images/industrial_seq.png";

    Mat image = imread(str);
    std::vector<cv::Mat> imgs;
    imgs.push_back(image);

    string str1 = current_path_str+"/../tests/assets/images/industrial_seq_sh.png";
    Mat image1 = imread(str1);
    imgs.push_back(image1);



    // use the do_ocr function to do the ocr
    std::vector<std::vector<bounding_box>> results;
    ret = do_ocr(imgs, ptrDeploySeg, ptrDeployCls, results);
    if (ret != 0) {
        cout << "[ERROR] Failed to do ocr." << endl;
        return ret;
    }
    cout<<"[INFO] industrial ocr process finished."<<endl;
    cout<<"[INFO] Now release the models."<<endl;
    // release_models
    return release_models(ptrDeploySeg, ptrDeployCls);
}