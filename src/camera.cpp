#include "camera.h"

namespace py = pybind11;

Camera::Camera(const Camera::CameraInfo &info) {
    this->_info = info;
}

Camera::~Camera() {
    for (int i = 0; i < _buffers.size(); ++i) {
        if (_buffers[i] != nullptr) {
            delete _buffers[i];
            _buffers[i] = nullptr;
        }
    }
}

void Camera::open() {
    if (GevOpenCameraBySN(const_cast<char *>(std::get<2>(_info).c_str()), GevExclusiveMode, &_handle) != GEVLIB_OK) {
        throw std::exception();
    }

    if (GevInitGenICamXMLFeatures(_handle, false) != GEVLIB_OK) {
        throw std::exception();
    }

    const size_t size =
        std::get<1>(getFeature<int>("Width")) *
        std::get<1>(getFeature<int>("Height"));

    for (int i = 0; i < _buffers.size(); ++i) {
        if (_buffers[i] != nullptr) {
            delete _buffers[i];
        }
        _buffers[i] = new uint8_t[size];
        memset(_buffers[i], 0, size);
    }

    if (GevInitializeTransfer(_handle, SynchronousNextEmpty, size, _buffers.size(), &_buffers[0]) != GEVLIB_OK) {
        throw std::exception();
    }
}

void Camera::startGrab() {
    if (GevStartTransfer(_handle, -1) != GEVLIB_OK) {
        throw std::exception();
    }
}

void Camera::stopGrab() {
    if (GevStopTransfer(_handle) != GEVLIB_OK) {
        throw std::exception();
    }

}

void Camera::close() {
    if (GevFreeTransfer(_handle) != GEVLIB_OK) {
        throw std::exception();
    }

    if (GevCloseCamera(&_handle) != GEVLIB_OK) {
        throw std::exception();
    }
    _handle = nullptr;

    for (int i = 0; i < _buffers.size(); ++i) {
        if (_buffers[i] != nullptr) {
            delete _buffers[i];
            _buffers[i] = nullptr;
        }
    }
}
    
py::tuple Camera::recv(bool wait) {
    auto tWait = wait ? timeval{10, 0} : timeval{0, 1};
    GEV_BUFFER_OBJECT *buffer = nullptr;
    if (GevGetNextFrame(_handle, &buffer, &tWait) != GEVLIB_OK) {
        throw std::exception();
    }
    if (wait && buffer == nullptr) {
        throw std::exception();
    }

    py::array_t<uint8_t> arr({buffer->h, buffer->w}, buffer->address);

    if (GevReleaseFrame(_handle, buffer) != GEVLIB_OK) {
        throw std::exception();
    }
    return py::make_tuple(_frameIdx++, arr);
}

py::dict Camera::properties() const {
    py::dict res;
    res["Fps"] = std::get<1>(getFeature<float>("AcquisitionFrameRate"));
    res["Width"] = std::get<1>(getFeature<int>("Width"));
    res["Height"] = std::get<1>(getFeature<int>("Height"));
    return res;
}

std::vector<Camera::CameraInfo> Camera::list_devices() {
    const int maxCameras = 100;
    int numCameras = 0;
    GEV_CAMERA_INFO cameras[maxCameras];
    if (GevGetCameraList(cameras, maxCameras, &numCameras) != GEVLIB_OK) {
        throw std::exception();
    }

    std::vector<CameraInfo> ret;
    for (int i = 0; i < numCameras; ++i) {
        const GEV_CAMERA_INFO gci = cameras[i];
        ret.emplace_back(gci.model, i, gci.serial);
    } 

    return ret;
}

void Camera::initialize() {
    if (GevApiInitialize() != GEVLIB_OK) {
        throw std::exception();
    }
}

void Camera::uninitialize() {
    if (GevApiUninitialize() != GEVLIB_OK) {
        throw std::exception();
    }
}
