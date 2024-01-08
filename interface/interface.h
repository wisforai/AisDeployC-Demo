//
// Created by Jinghui Zhou on 2023/1/4.
//

#ifndef AISDEPLOYC_INTERFACE_H
#define AISDEPLOYC_INTERFACE_H

const std::string AisDeployCVersion="v0.3.3b";

#ifdef DEPLOY_ON_WINDOWS
#define AisDeployC_API extern "C" __declspec(dllexport)
#else
#define AisDeployC_API extern "C"
#endif


typedef struct bounding_box
{

    std::string category = "";        // category name, get from config file
    float score = 0.;                  // confidence
    float xmin = 0;                   // target bbox position, left top x
    float ymin = 0;                   // target bbox position, left top y
    float xmax = 0;                   // target bbox position, right bottom x
    float ymax = 0;                   // target bbox position, right bottom y

}bounding_box;

typedef struct classification
{

    std::string category = "";        // category name, get from config file
    float score = 0.;                  // confidence

}classification;

/**
*  @brief 检查当前库版本号与头文件版本号是否一致
*
*  @details
*   检查当前库版本号与头文件版本号是否一致
 *  @see
 *  示例代码如下
 *  @code
 *  int ret = check_version();
 *  @endcode
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int check_version();

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
AisDeployC_API std::string get_version();
/**
*  @brief 模型初始化
*
*  @details
*   根据模型路径、GPU ID 初始化模型
*  @see
*  c++接口示例代码如下
*  @code
    int initRet = 0;
    string current_path_str = ".";
    std::string model_path = current_path_str+"/../tests/assets/models/epoch_200_segmentor_setting_oen.aism";
    int gpu_id = 0;
    ptrDeploy = initialize(model_path.c_str(), gpu_id, &initRet);
*  @endcode
*  python接口示例代码如下
*  @code
    import ctypes
    import platform
    lib = None
    handle = None
    if "macOS" in platform.platform():
        # sys.path.append(os.path.join(os.path.dirname(__file__), "../cmake-build-debug"))
        lib = ctypes.cdll.LoadLibrary("./cmake-build-debug/libAisDeployC.dylib")
    elif "Windows" in platform.platform():
        lib = ctypes.windll.LoadLibrary( "./cmake-build-release/AisDeployC.dll" )
    elif "Linux" in platform.platform():
        lib = ctypes.cdll.LoadLibrary( "./build/libAisDeployC.so" )
        lib.initialize.restype = ctypes.c_void_p
    ret = ctypes.c_int(-1)
    gpu_id = ctypes.c_int(-1)
    path_str = "tests/assets/models/epoch_200_segmentor_setting_oen.aism"
    path_char = ctypes.c_char_p(path_str.encode('utf-8'))
    handle = lib.initialize(path_char, gpu_id, ctypes.pointer(ret))
    lib.py_process_json_str.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
    lib.release.argtypes = [ctypes.c_void_p]
*  @endcode
*  @param model_path  模型路径
*  @param gpu_id  显卡(GPU) ID，0代表0号显卡；-1代表不使用GPU
 *  @param state 执行状态，0代表执行成功
*  @return 返回模型对象指针 void*
*/

AisDeployC_API void *initialize(
        const char *model_path,
        int gpu_id,
        int *state
        );

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

AisDeployC_API int process_images(
        void *base,
        std::vector<cv::Mat> & imgs
        );

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
AisDeployC_API int get_categories(
        void *base,
        std::vector<std::string> & out_categories
);

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
AisDeployC_API int get_per_category_binary_masks(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_masks
);

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
AisDeployC_API int get_per_category_score_maps(
        void *base,
        std::vector<std::vector<cv::Mat> > & per_category_score_maps
);

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
AisDeployC_API int get_binary_mask_from_score_map(
        cv::Mat score_map,
        cv::Mat & binary_mask,
        float thresh
);


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
AisDeployC_API int erode_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_erode
);

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

AisDeployC_API int dilate_binary_mask(
        cv::Mat & binary_mask,
        int kernel_size,
        int iter_dilate
);


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
AisDeployC_API int cal_per_category_instances(
        void *base,
        std::vector<std::vector<cv::Mat> >  per_category_score_maps,
        std::vector<std::vector<cv::Mat> > per_category_masks
);

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
AisDeployC_API int get_per_batch_instance_number(
        void *base,
        std::vector<int> & per_batch_instance_number
);

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
AisDeployC_API  int get_per_batch_instances(
        void *base,
        std::vector<std::vector<bounding_box>> & per_batch_instances
);



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
AisDeployC_API  int get_per_batch_instance_mask_areas(
        void *base,
        std::vector<std::vector<float>> & per_batch_instance_mask_areas
);

/**
*  @brief 释放实例
*
*  @details
*   释放实例
*  @see
*  c++接口示例代码如下
*  @code
 *  int ret = release(ptrDeploy);
*  @endcode
*  python接口示例代码如下，一般需要先经过initialize和lib.release.argtypes = [ctypes.c_void_p]的设定
*  @code
 *  ret = lib.release(handle)
*  @endcode
*  @param base  initialize返回的模型指针
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API int release(
        void *base
);


/**
*  @brief 更新授权文件信息
*
*  @details
*   更新授权文件信息
 *  @see
 *  示例代码如下
 *  @code
 *  int ret = update_license(ptrDeploy, "macos_registed_info.aisl");
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param path  授权文件的路径
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int update_license(void *base, const char* path);

/**
*  @brief 生成未授权文件
*
*  @details
*   生成授权文件
 *  @see
 *  示例代码如下
 *  @code
 *  int ret = generate_license(ptrDeploy);
 *  @endcode
 *  @param base  initialize返回的模型指针
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int generate_license(void *base);

/**
*  @brief python使用的处理json string输入格式的接口
*
*  @details
*   python使用的处理json string输入格式的接口,一般需要先经过initialize和process_json_str.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]的设定
 *  @see
 *  示例代码如下
 *  @code
    imgPth = "tests/assets/images/1.jpg"
    f= open(imgPth, 'rb')
    qrcode = base64.b64encode(f.read()).decode()
    f.close()
    file_json = {"type": "base64", "data": qrcode, "ch":3}
    input_json = {"data_list": [file_json]}
    data_str = json.dumps(input_json)
    data_char = ctypes.c_char_p(data_str.encode('utf-8'))

    ret = lib.py_process_json_str(handle, data_char, len(data_str))
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param input  输入json string (const char *)
 *  @param input_size 输入json string长度
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_process_json_str(void *base, const char *input, int input_size);

/**
*  @brief python使用的获取json string输出格式的接口
*
*  @details
*   python使用的获取json string输出格式的接口,一般需要先经过initialize, py_process_json_str
 *  @see
 *  示例代码如下
 *  @code
 *      ret_char_c = ctypes.c_char_p()
 *      ret = lib.py_get_json_str_results(handle, ret_char_c, None)
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param output  输出json string (char **)
 *  @param int 输出json string长度
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_get_json_str_results(void *base, char **output, int *output_size);

/**
*  @brief python使用的释放返回结果的接口
*
*  @details
*   python使用的释放返回结果的接口
 *  @see
 *  示例代码如下
 *  @code
 *      ret_char_c = ctypes.c_char_p()
 *      ret = lib.py_free_result(ret_char_c)
 *  @endcode
 *  @param output  输出json string (char *)
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_free_result(char *output);

/**
*  @brief 获得每个批次的图像特征嵌入
*
*  @details
*   在实例计算结束后，获得每个批次的图像特征嵌入
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<float>> & per_batch_embeddings;
    int ret = get_per_batch_embeddings(ptrDeploy, per_batch_embeddings);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_embeddings  获得每个批次的实例的图像特征嵌入
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int get_per_batch_embeddings(
        void *base,
        std::vector<std::vector<float>> & per_batch_embeddings
);

/**
*  @brief 获得每个批次的图像3D特征嵌入
*
*  @details
*   在实例计算结束后，获得每个批次的图像特征嵌入
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<std::vector<std::vector<float>>>> & per_batch_embeddings;
    int ret = get_per_batch_embeddings_3d(ptrDeploy, per_batch_embeddings);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_embeddings  获得每个批次的实例的图像特征嵌入
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int get_per_batch_embeddings_3d(
        void *base,
        std::vector<std::vector<std::vector<std::vector<float>>>> & per_batch_embeddings
);

/**
*  @brief 处理图片
*
*  @details
*   使用模型处理3维嵌入和提示词（前向推理）

*/

AisDeployC_API int process_embedding_3d_prompt(
        void *base,
        std::vector<std::vector<std::vector<std::vector<float>>>>  batch_embeddings,
        std::vector<std::vector<std::vector<float>>> point_coords,
        std::vector<std::vector<float>> point_labels,
        std::vector<std::vector<std::vector<std::vector<float>>>> batch_mask_input,
        std::vector<float> has_mask_input,
        std::vector<std::vector<float>> orig_im_size
);

/**
*  @brief 批量加载 字段 和 特征嵌入 对
*
*  @details
*   批量加载 字段 和 特征嵌入 对，形成底库
*  @see
*  示例代码如下
*  @code
    int ret = load_keys_embeddings(ptrDeploy, vec_keys, vec_embeddings);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param vec_keys  批量的字段
*  @param vec_embeddings  批量的特征嵌入
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int load_keys_embeddings(
        void *base,
        std::vector<std::string> & vec_keys,
        std::vector<std::vector<float>> & vec_embeddings
);


/**
*  @brief 将特征嵌入与底库内的特征嵌入进行比较
*
*  @details
*   将特征嵌入与底库内的特征嵌入进行比较，获得比较结果（字段与对应的置信度）
*  @see
*  示例代码如下
*  @code
    int ret = compare_with_ground_embeddings(ptrDeploy, vec_keys, vec_embeddings);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param vec_embeddings  批量的特征嵌入，维度记为N维
*  @param ground_keys  底库中的字段，维度记为M维
*  @param vec_scores  比较的阈值结果，如果比较的vec_embeddings是N维，底库的长度是M维，则输出的vec_scores的维度是NxM
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int compare_with_ground_embeddings(
        void *base,
        std::vector<std::vector<float>> & vec_embeddings,
        std::vector<std::string> & ground_keys,
        std::vector<std::vector<float>> & vec_scores
);


/**
*  @brief python使用的批量 字段 和 特征嵌入 对，与底库中特征嵌入做比对，json string输入格式的接口,json string输出格式的接口
*
*  @details
*   python使用的批量 字段 和 特征嵌入 对，与底库中特征嵌入做比对，json string输入格式的接口,json string输出格式的接口
 *  @see
 *  示例代码如下
 *  @code
    key = ""
    value = list()
    file_json = {"embedding_vector":value}
    input_json = {"data_list": [file_json]}
    data_str = json.dumps(input_json)
    data_char = ctypes.c_char_p(data_str.encode('utf-8'))
    ret_char_c = ctypes.c_char_p()
    ret = lib.py_compare_with_ground_embeddings(handle, data_char, len(data_str), ret_char_c, None)
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param input  输入json string (const char *)
 *  @param input_size 输入json string长度
 *  @param output  输出json string (char **)
 *  @param output_size 输出json string长度
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_compare_with_ground_embeddings(void *base, const char *input, int input_size, char **output, int *output_size);

/**
*  @brief python使用的解码器处理接口，输入特征嵌入和提示prompt，json string输入格式的接口,json string输出格式的接口
*
*  @details
*   python使用的解码器处理接口，输入特征嵌入和提示prompt，json string输入格式的接口,json string输出格式的接口
 *  @see
 *  示例代码如下
 *  @code
    key = ""
    value = list()
    file_json = {"embedding_vector":value}
    input_json = {"data_list": [file_json]}
    data_str = json.dumps(input_json)
    data_char = ctypes.c_char_p(data_str.encode('utf-8'))
    ret_char_c = ctypes.c_char_p()
    ret = lib.py_process_decoder(handle, data_char, len(data_str), ret_char_c, None)
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param input  输入json string (const char *)
 *  @param input_size 输入json string长度
 *  @param output  输出json string (char **)
 *  @param output_size 输出json string长度
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_process_decoder(void *base, const char *input, int input_size, char **output, int *output_size);


/**
*  @brief python使用的获取json string输出格式的接口
*
*  @details
*   python使用的获取json string输出格式的接口,一般需要先经过initialize, py_process_json_str
 *  @see
 *  示例代码如下
 *  @code
 *      ret_char_c = ctypes.c_char_p()
 *      ret = lib.py_get_json_str_results(handle, ret_char_c, None)
 *  @endcode
 *  @param base  initialize返回的模型指针
 *  @param output  输出json string (char **)
 *  @param output_size 输出json string长度
*  @return 执行结果，0表示检查成功，否则检查失败
*/
AisDeployC_API int py_get_json_str_results(void *base, char **output, int *output_size);


/**
*  @brief 获得每个批次的分类结果
*
*  @details
*   在实例计算结束后，获得每个批次的分类结果
*  @see
*  示例代码如下
*  @code
    std::vector<classification> per_batch_cls;
    int ret = get_per_batch_classification(ptrDeploy, per_batch_cls);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_cls  获得每个批次的分类结果，分类结果使用结构体classification表达
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int get_per_batch_classification(
        void *base,
        std::vector<classification> & per_batch_cls
);

/**
*  @brief 获得每个批次的姿态估计结果
*
*  @details
*   在实例计算结束后，获得每个批次的姿态估计结果
*  @see
*  示例代码如下
*  @code
    std::vector<std::vector<std::vector<float>>> per_batch_pose;
    int ret = get_per_batch_pose_estimation(ptrDeploy, per_batch_pose);
*  @endcode
*  @param base  initialize返回的模型指针
*  @param per_batch_pose  获得每个批次的姿态估计结果，姿态估计结果结果使用std::vector<float>表示，其中每个float表示一个关键点的坐标{x, y}
*  @return 执行结果，0表示执行成功，否则执行失败
*/
AisDeployC_API  int get_per_batch_pose_estimation(
        void *base,
        std::vector<std::vector<std::vector<float>>>  & per_batch_pose
);

#endif //AISDEPLOYC_INTERFACE_H
