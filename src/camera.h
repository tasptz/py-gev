#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <vector>
#include <string>
#include <tuple>

#include <gevapi.h>

namespace py = pybind11;

class Camera {
public:
    typedef std::tuple<std::string, int, std::string> CameraInfo;

    Camera(const CameraInfo &info);
    ~Camera();

    void open();
    void startGrab();
    void stopGrab();
    void close();
    py::tuple recv(bool wait=false);

    py::dict properties() const;

    static std::vector<CameraInfo> list_devices();
    static void initialize();
    static void uninitialize();

private:
    template <typename T>
    std::tuple<int, T> getFeature(const char *featureName) const {
        int featureType;
        T value;
        if (GevGetFeatureValue(const_cast<GEV_CAMERA_HANDLE>(_handle), featureName, &featureType, sizeof(value), &value) != GEVLIB_OK) {
            throw std::runtime_error("Error getting feature " + std::string(featureName));
        }
        return {featureType, value};
    }
 
    CameraInfo  _info;
    GEV_CAMERA_HANDLE _handle = nullptr;
    std::vector<uint8_t*> _buffers{2, nullptr};
    uint64_t _frameIdx = 0;
};
