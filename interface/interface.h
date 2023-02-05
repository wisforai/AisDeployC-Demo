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


/**
*  @brief 获得当前库版本号
*
*  @details
*   获得当前库的版本号，格式为v0.0.0
 *  @see
 *  示例代码如下
 *  @code
 *  std::string version = get_version();
 *  @endcode
*
*  @return     std::string 返回版本号
*/
#ifdef DEPLOY_ON_WINDOWS
extern "C" __declspec(dllexport) std::string get_version();
#else
extern "C" std::string get_version();
#endif

/**
*  @brief 模型初始化
*
*  @details
*   根据模型路径、GPU ID 初始化模型
*  @see
*  示例代码如下
*  @code
    int initRet = 0;
    string current_path_str = ".";
    std::string model_path = current_path_str+"/../tests/assets/models/epoch_200_segmentor_setting_oen.aism";
    int gpu_id = 0;
    ptrDeploy = initialize(model_path.c_str(), gpu_id, &initRet);
*  @endcode
*  @param model_path  模型路径
*  @param gpu_id  显卡(GPU) ID，0代表0号显卡；-1代表不使用GPU
 *  @param state 执行状态，0代表执行成功
*  @return 返回模型对象指针 void*
*/

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


/**
*  @brief 处理图片
*
*  @details
*   使用模型处理图像或图像批次（前向推理）
*  @see
*  单一批次示例代码如下
*  @code
    std::vector<cv::Mat> imgs;
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/1.jpg";
    Mat image = imread(str);
    imgs.push_back(image);
    int ret = process_images(ptrDeploy, imgs);
*  @endcode
*  多批次示例代码如下
*  @code
    std::vector<cv::Mat> imgs;
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/1.jpg";
    Mat image = imread(str);
    imgs.push_back(image);
    String str_1 = current_path_str+"/../tests/assets/images/63_1024.jpg";
    Mat image_1 = imread(str_1);
    imgs.push_back(image_1);
    int ret_1 = process_images(ptrDeploy, imgs);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param imgs  输入图像批次
*  @return 执行结果，0表示执行成功，否则执行失败
*/

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

/**
*  @brief 获得类别列表
*
*  @details
*   获得当前模型的类别列表
*  @see
*  示例代码如下
*  @code
    std::vector<std::string> out_categories;
    int ret = get_categories(
            ptrDeploy,
            out_categories
    );
*  @endcode
*  @param base  initialize返回的模型指针
*  @param out_categories  用来接收类别列表变量
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 获得每类别的二值掩模
*
*  @details
*   获得当前模型的处理图像批次（前向推理）后的每类别的二值掩模结果
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<cv::Mat> > per_category_masks;
    int ret = get_per_category_binary_masks(
            ptrDeploy,
            per_category_masks
    );
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_category_masks  用来接收每类别的二值掩模结果，第一层vector代表类别（与get_categories的接收out_categories对应），第二层vector代表批次（与process_images输入imgs对应）
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 获得每类别的置信度图
*
*  @details
*   获得当前模型的处理图像批次（前向推理）后的每类别的置信度图
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<cv::Mat> > per_category_score_maps;
    int ret = get_per_category_score_maps(
            ptrDeploy,
            per_category_score_maps
    );
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_category_score_maps  用来接收每类别的置信度图结果，第一层vector代表类别（与get_categories的接收out_categories对应），第二层vector代表批次（与process_images输入imgs对应）
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 置信度图阈值处理获得二值掩模
*
*  @details
*   置信度图阈值处理获得二值掩模
*  @see
*  示例代码如下
*  @code
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/sem_score.jpg";
    Mat image = imread(str, IMREAD_GRAYSCALE);
    cv::Mat score_map;
    image.convertTo(score_map, CV_32FC1, 1.0 / 255, 0);

    cv::Mat binary_mask;
    float thresh = 0.1;

    int ret = get_binary_mask_from_score_map(
            score_map,
            binary_mask,
            thresh
    );
*  @endcode
*  @param score_map  输入置信度图，一般是get_per_category_score_maps接收per_category_score_maps中单个score_map，值域需要控制在[0,1]
*  @param binary_mask  用来接收处理的二值掩模结果
*  @param thresh  处理的阈值，值域(0,1)
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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


/**
*  @brief 二值掩模腐蚀处理
*
*  @details
*   二值掩模腐蚀处理
*  @see
*  示例代码如下
*  @code
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/sem_mask.jpg";

    Mat binary_mask = imread(str, IMREAD_GRAYSCALE);

    int kernel_size = 7;
    int iter_erode = 2;

    int ret = erode_binary_mask(
            binary_mask,
            kernel_size,
            iter_erode
    );
*  @endcode
*  @param binary_mask  处理的二值掩模，由于使用的是引用，所以会直接以该变量接收处理结果
*  @param kernel_size  腐蚀处理的核大小，>1
*  @param iter_erode 腐蚀处理迭代次数, >=1
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 二值掩模膨胀处理
*
*  @details
*   二值掩模膨胀处理
*  @see
*  示例代码如下
*  @code
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/sem_mask_1.jpg";

    Mat binary_mask = imread(str, IMREAD_GRAYSCALE);

    int kernel_size = 3;
    int iter_dilate = 1;

    int ret = dilate_binary_mask(
            binary_mask,
            kernel_size,
            iter_dilate
    );
*  @endcode
*  @param binary_mask  处理的二值掩模，由于使用的是引用，所以会直接以该变量接收处理结果
*  @param kernel_size  膨胀处理的核大小，>1
*  @param iter_dilate 膨胀处理迭代次数, >=1
*  @return 执行结果，0表示执行成功，否则执行失败
*/

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


/**
*  @brief 计算每类别的实例 blob（输入掩模和置信度图）
*
*  @details
*   输入掩模和置信度图，计算每类别的实例 blob
*  @see
*  示例代码如下
*  @code
    string current_path_str = ".";
    String str = current_path_str+"/../tests/assets/images/sem_mask_1.jpg";

    Mat binary_mask = imread(str, IMREAD_GRAYSCALE);

    String str1 = current_path_str+"/../tests/assets/images/sem_score.jpg";
    Mat image = imread(str1, IMREAD_GRAYSCALE);
    cv::Mat score_map;
    image.convertTo(score_map, CV_32FC1, 1.0 / 255, 0);

    std::vector<std::vector<cv::Mat> >  per_category_score_maps;
    std::vector<std::vector<cv::Mat> > per_category_masks;

    std::vector<cv::Mat> tmp;
    tmp.push_back(score_map);
    per_category_score_maps.push_back(tmp);

    std::vector<cv::Mat> tmp1;
    tmp1.push_back(binary_mask);
    per_category_masks.push_back(tmp1);

    int ret = cal_per_category_instances(
            ptrDeploy,
            per_category_score_maps,
            per_category_masks
    );
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_category_score_maps  输入每类别的置信度图，一般是get_per_category_score_maps接收的per_category_score_maps
*  @param per_category_masks 输入每类别的置信度图，一般是get_per_category_binary_masks接收的per_category_masks，或者是它经过腐蚀膨胀处理后的二值掩模组成
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 获得每个批次的实例个数
*
*  @details
*   在实例计算结束后，获得每个批次的实例个数
*  @see
*  示例代码如下
*  @code
    std::vector<int>  per_batch_instance_number;
    int ret = get_per_batch_instance_number(ptrDeploy, per_batch_instance_number);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_instance_number  获得每个批次的实例个数，第一层vector代表批次，与process_images中输入imgs对应
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 获得每个批次的实例
*
*  @details
*   在实例计算结束后，获得每个批次的实例
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<bounding_box>> per_batch_instances;
    int ret = get_per_batch_instances(ptrDeploy, per_batch_instances);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_instances  获得每个批次的实例，实例使用结构体bounding_box表达
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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



/**
*  @brief 获得每个批次的实例掩模面积
*
*  @details
*   在实例计算结束后，获得每个批次的实例掩模面积
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<float>> per_batch_instance_mask_areas;
    int ret = get_per_batch_instance_mask_areas(ptrDeploy, per_batch_instance_mask_areas);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_instance_mask_areas  获得每个批次的实例掩模面积
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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

/**
*  @brief 释放实例
*
*  @details
*   释放实例
*  @see
*  示例代码如下
*  @code
 *  int ret = release(ptrDeploy);
*  @endcode
*  @param base  initialize返回的模型指针
*  @return 执行结果，0表示执行成功，否则执行失败
*/
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
