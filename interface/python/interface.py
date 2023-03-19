# -*- coding: UTF-8 -*-

import ctypes
import json
import platform

AisDeployCVersion="v0.2.1"

class AisDeployC():
    """
    AisDeployC python接口类 AisDeployC

    """
    def __init__(self, lib_pth: str):
        """! The AisDeployC class initializer.
        @see
         示例代码如下
         @code
            lib_path = "./cmake-build-release/AisDeployC.dll"
            deploy_obj = AisDeployC(lib_path)
         @endcode
        @param lib_pth AisDeployC library path.
        @return  An instance of the AisDeployC class initialized.
        """

        if "macOS" in platform.platform():
            self.lib = ctypes.cdll.LoadLibrary(lib_pth)
        elif "Windows" in platform.platform():
            self.lib = ctypes.windll.LoadLibrary(lib_pth)
        elif "Linux" in platform.platform():
            self.lib = ctypes.cdll.LoadLibrary(lib_pth)
        self.handle = None

    def __del__(self):
        """! The AisDeployC class delete function.
        """
        if self.handle != None:
            self.lib.release.argtypes = [ctypes.c_void_p]
            self.lib.release( self.handle )

    def model_initialize(self, path_str: str, gpu_id: int):
        """! The AisDeployC class model initializer.
         @see
         示例代码如下
         @code
            path_str = "tests/assets/models/det_setting_oen.aism"
            gpu_id = 0
            ret = deploy_obj.model_initialize(path_str, gpu_id)
         @endcode
        @param path_str model file path.
        @param gpu_id gpu id for model initialize
        @return  model_initialize result, 0 for success, 1 for failure.
        """

        self.lib.initialize.restype = ctypes.c_void_p
        ret = ctypes.c_int(-1)
        gpu_id = ctypes.c_int(gpu_id)

        path_char = ctypes.c_char_p(path_str.encode('utf-8'))
        self.handle = self.lib.initialize(path_char, gpu_id, ctypes.pointer(ret))

        self.lib.update_license.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.py_process_json_str.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
        self.lib.py_get_json_str_results.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_int)]
        self.lib.py_free_result.argtypes = [ctypes.c_char_p]
        self.lib.release.argtypes = [ctypes.c_void_p]
        self.lib.py_load_keys_embeddings.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
        self.lib.py_load_keys_embeddings.restype = ctypes.c_int
        self.lib.py_compare_with_ground_embeddings.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_int)]

        return ret.value

    def update_license(self, path_str: str):
        """! The AisDeployC class license update function.
        @see
         示例代码如下
         @code
            license_path = "tests/assets/licenses/registed/windows_registed_info.aisl"
            ret = deploy_obj.update_license(license_path)
         @endcode

        @param path_str license file path.
        @return  update_license result, 0 for success, 1 for failure.
        """
        path_char = ctypes.c_char_p(path_str.encode('utf-8'))
        return self.lib.update_license(self.handle, path_char)

    def process(self, input_json: dict):
        """! The AisDeployC class process function.
        @see
         batch=1 示例代码如下
         @code
            import base64
            import json

            imgPth = "tests/assets/images/1.jpg"
            f= open(imgPth, 'rb')
            qrcode = base64.b64encode(f.read()).decode()
            f.close()
            file_json = {"type": "base64", "data": qrcode, "ch":3}
            input_json = {"data_list": [file_json]}

            ret_val = deploy_obj.process(input_json)
         @endcode
        @see
         batch=2 示例代码如下
         @code
            import base64
            import json

            imgPth = "tests/assets/images/1.jpg"
            f= open(imgPth, 'rb')
            qrcode = base64.b64encode(f.read()).decode()
            f.close()
            file_json = {"type": "base64", "data": qrcode, "ch":3}
            input_json = {"data_list": [file_json]}

            imgPth = "tests/assets/images/63_1024.jpg"
            f= open(imgPth, 'rb')
            qrcode = base64.b64encode(f.read()).decode()
            f.close()
            file_json = {"type": "base64", "data": qrcode, "ch":3}
            input_json["data_list"].append(file_json)

            ret_val = deploy_obj.process(input_json)
         @endcode

        @param input_json input json like dict.
        @return  process result, output_json output json like dict. for success, None for failure.
        """
        if self.handle is None:
            print("[ERROR] Check handle failed in AisDeployC.process. maybe lack initialization.")
            return None
        data_str = json.dumps(input_json)
        data_char = ctypes.c_char_p(data_str.encode('utf-8'))
        ret = self.lib.py_process_json_str(self.handle, data_char, len(data_str))
        if ret != 0:
            print("[ERROR] py_process_json_str failed in  AisDeployC. maybe check input of process.")
            return None
        ret_char_c = ctypes.c_char_p()
        ret = self.lib.py_get_json_str_results( self.handle, ctypes.pointer(ret_char_c), None )
        if ret != 0:
            print("[ERROR] py_get_json_str_results failed in  AisDeployC. maybe lack py_process_json_str.")
            return None
        ret_str = ret_char_c.value.decode("utf-8")
        ret_value = json.loads(ret_str)
        self.lib.py_free_result(ret_char_c)
        return ret_value

    def load_keys_embeddings(self, input_json: dict):
        """! The AisDeployC class load_keys_embeddings function.
        @see
         batch=1 示例代码如下
         @code
            import json
            key = ""
            value = list()
            file_json = {"key": key, "embedding_vector":value}
            input_json = {"data_list": [file_json]}

            ret_val = deploy_obj.load_keys_embeddings(input_json)
         @endcode

        @param input_json input json like dict.
        @return  process result, 0 for success, 1 for failure.
        """
        if self.handle is None:
            print("[ERROR] Check handle failed in AisDeployC.process. maybe lack initialization.")
            return None

        data_str = json.dumps(input_json)
        data_char = ctypes.c_char_p(data_str.encode('utf-8'))

        ret = self.lib.py_load_keys_embeddings(self.handle, data_char, len(data_str))
        if ret != 0:
            print("[ERROR] load_keys_embeddings failed in  AisDeployC. maybe check input of process.")
            return None
        return ret

    def compare_with_ground_embeddings(self, input_json: dict):
        """! The AisDeployC class compare_with_ground_embeddings function.
        @see
         示例代码如下
         @code
            import json
            value = list()
            file_json = {"embedding_vector":value}
            input_json = {"data_list": [file_json]}

            ret_val = deploy_obj.process(input_json)
         @endcode

        @param input_json input json like dict.
        @return  process result, output_json output json like dict. for success, None for failure.
        """
        if self.handle is None:
            print("[ERROR] Check handle failed in AisDeployC.process. maybe lack initialization.")
            return None
        data_str = json.dumps(input_json)
        data_char = ctypes.c_char_p(data_str.encode('utf-8'))
        ret_char_c = ctypes.c_char_p()
        ret = self.lib.py_compare_with_ground_embeddings(self.handle, data_char, len(data_str), ctypes.pointer(ret_char_c), None )
        if ret != 0:
            print("[ERROR] compare_with_ground_embeddings failed in  AisDeployC. maybe check input of process.")
            return None
        ret_str = ret_char_c.value.decode("utf-8")
        ret_value = json.loads(ret_str)
        self.lib.py_free_result(ret_char_c)
        return ret_value