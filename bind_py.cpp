#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "image_processor.h" // Include the missing header file

namespace py = pybind11;

py::array_t<unsigned int> process_image_py(py::array_t<unsigned char> src, int scale, int lambda) {
    // Get the pointer to the data
    unsigned char* ptr = (unsigned char*)src.request().ptr;
    // Get the dimensions of the image
    int width = src.shape(1);
    int height = src.shape(0);
    // Call the C++ function
    unsigned int* res = process_image(ptr, width, height, scale, lambda);
    // Create a numpy array
    py::array_t<unsigned int> result = py::array_t<unsigned int>({height, width}, res);
    return result;
}

// Bind C++ function to Python
PYBIND11_MODULE(mpcm, m) {
    m.doc() = "Image processor module"; // Optional module documentation string
    m.def("process_image", &process_image_py, "Process an image");
}