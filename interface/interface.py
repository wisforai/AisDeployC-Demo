# -*- coding: UTF-8 -*-

import ctypes
import json
import platform

AisDeployCVersion="v0.1.0"

class AisDeployC():
    """
    AisDeployC python接口类 AisDeployC

    """
    def __init__(self, lib_pth: str):
        """! The AisDeployC class initializer.
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
        return ret.value

    def update_license(self, path_str: str):
        """! The AisDeployC class license update function.
        @param path_str license file path.
        @return  update_license result, 0 for success, 1 for failure.
        """
        path_char = ctypes.c_char_p(path_str.encode('utf-8'))
        return self.lib.update_license(self.handle, path_char)

    def process(self, input_json: dict):
        """! The AisDeployC class process function.
        @param input_json input json like dict.
        @return  process result, 0 for success, 1 for failure.
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