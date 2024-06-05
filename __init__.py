from build import mpcm


def mpcm_run(image, scale, lamda):
    assert image is not None, f"Error: Image is None"
    assert image.ndim == 2, f"Error: Image must be grayscale"
    
    # 调用 mpcm 模块中的 process_image_py 函数
    result_img = mpcm.process_image(image, scale, lamda)
   
    return result_img