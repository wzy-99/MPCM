# MPCM Python Library

MPCM is a Python library that provides an interface to the MPCM image processing module. It allows you to process grayscale images using the MPCM algorithm.

## Installation

To install the MPCM library, you can use pip:

```bash
python setup.py install
```

## Usage
Here is a simple example of how to use the MPCM library to process an image:

```python
from mpcm import mpcm_run
import cv2

# Load an image
image_path = 'path/to/your/image.jpg'
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

# Process the image
result_img = mpcm_run(image, scale=1, lamda=1)

# Save the result
cv2.imwrite('processed_image.jpg', result_img)
```

In this example, mpcm_run is a function that takes a grayscale image, a scale factor, and a lambda parameter, and returns the processed image.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.