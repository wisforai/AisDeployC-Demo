//
// Created by Jinghui Zhou on 2023/1/4.
//

#ifndef AISDEPLOYC_INTERFACE_H
#define AISDEPLOYC_INTERFACE_H

const std::string AisDeployCVersion="v0.1.0";

typedef struct bounding_box
{

    std::string category = "";        // 类别名称，根据配置文件得到
    float score = 0.;                  // 置信度
    float xmin = 0;                   // 目标bbox位置 左上角 横坐标
    float ymin = 0;                   // 目标bbox位置 左上角 纵坐标
    float xmax = 0;                   // 目标bbox位置 右下角 横坐标
    float ymax = 0;                   // 目标bbox位置 右下角 纵坐标

}bounding_box;

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) std::string get_version();
#else
extern "C" std::string get_version();
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) void *initialize(
        const char *model_path,
        int gpu_id,
        int *state
        );
#else
extern "C" void *initialize(
        const char *model_path,
        int gpu_id,
        int *state
        );
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int process_images(
        void *base,
        std::vector<cv::Mat> & imgs
        );
#else
extern "C" int process_images(
        void *base,
        std::vector<cv::Mat> & imgs
        );
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_categories(
        void *base,
        std::vector<std::string> & out_categories
);
#else
extern "C" int get_categories(
        void *base,
        std::vector<std::string> & out_categories
);

#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_per_category_binary_masks(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_masks
);
#else
extern "C" int get_per_category_binary_masks(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_masks
);
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_per_category_score_maps(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_score_maps
);
#else
extern "C" int get_per_category_score_maps(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_score_maps
);
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_binary_mask_from_score_map(
        cv::Mat score_map,
        cv::Mat & binary_mask,
        float thresh
);
#else
extern "C" int get_binary_mask_from_score_map(
        cv::Mat score_map,
        cv::Mat & binary_mask,
        float thresh
);
#endif


#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int erode_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_erode
);
#else
extern "C" int erode_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_erode
);
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int dilate_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_dilate
);
#else
extern "C" int dilate_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_dilate
);
#endif


#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int cal_per_category_instances(
        void *base,
        std::vector<std::vector<cv::Mat> >  per_category_score_maps,
        std::vector<std::vector<cv::Mat> > & per_category_masks
);
#else
extern "C" int cal_per_category_instances(
        void *base,
        std::vector<std::vector<cv::Mat> >  per_category_score_maps,
        std::vector<std::vector<cv::Mat> > per_category_masks
);
#endif

#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_per_batch_instance_number(
    void *base,
    std::vector<int> & per_batch_instance_number
);
#else
extern "C" int get_per_batch_instance_number(
        void *base,
        std::vector<int> & per_batch_instance_number
);
#endif


#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_per_batch_instances(
        void *base,
        std::vector<std::vector<bounding_box>> & per_batch_instances
);
#else
extern "C"  int get_per_batch_instances(
        void *base,
        std::vector<std::vector<bounding_box>> & per_batch_instances
);
#endif




#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int get_per_batch_instance_mask_areas(
        void *base,
        std::vector<std::vector<float>> & per_batch_instance_mask_areas
);
#else
extern "C"  int get_per_batch_instance_mask_areas(
        void *base,
        std::vector<std::vector<float>> & per_batch_instance_mask_areas
);
#endif


#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) int release(
        void *base
);
#else
extern "C" int release(
        void *base
);
#endif

#endif //AISDEPLOYC_INTERFACE_H
