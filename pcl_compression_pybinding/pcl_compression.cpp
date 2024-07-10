#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
#include <iostream>
#include <pcl/compression/octree_pointcloud_compression.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <string.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;
int maybe_main()
{
    pcl::PointCloud<pcl::PointXYZRGB> sourceCloud;
    pcl::PLYReader reader;
    if (pcl::io::loadPLYFile("/home/zoratt/cat_ws/src/record_livox_avia/pcd_seq1/cloud-20.ply", sourceCloud) == -1) // 输入点云文件
    {
        PCL_ERROR("Failed to load PLYFile!");
        return -1;
    }
    bool showStatistics = true;
    pcl::io::compression_Profiles_e compressionProfile = pcl::io::MANUAL_CONFIGURATION;
    pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB> *PointCloudEncoder;
    PointCloudEncoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>(compressionProfile, showStatistics, 0.01, 0.1,
                                                                                   false, 100, false, 8); // 输入参数
    std::stringstream compressedData;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZRGB>());

    PointCloudEncoder->encodePointCloud(sourceCloud.makeShared(), compressedData);
    PointCloudEncoder->decodePointCloud(compressedData, cloudOut);
    pcl::PLYWriter writer;
    writer.write("high_true.ply", *cloudOut, false, false);
    system("pause");
    return 0;
}
// 使用pcl压缩点云，返回python binary
//  src_pc: numpy array, shape=(n, 3), dtype=float32
py::bytes encode(py::array_t<float> input, double pointResolution, double octreeResolution)
{
    pcl::PointCloud<pcl::PointXYZRGB> sourceCloud;
    auto buf = input.request();
    float *ptr = (float *)buf.ptr;
    for (int i = 0; i < buf.size; i += 3)
    {
        pcl::PointXYZRGB point;
        point.x = ptr[i];
        point.y = ptr[i + 1];
        point.z = ptr[i + 2];
        sourceCloud.push_back(point);
    }
    pcl::io::compression_Profiles_e compressionProfile = pcl::io::MANUAL_CONFIGURATION;
    auto PointCloudEncoder = std::make_shared<pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>>(
        compressionProfile, false, pointResolution, octreeResolution, false, 100, false, 8); // 使用智能指针
    std::stringstream compressedData;
    PointCloudEncoder->encodePointCloud(sourceCloud.makeShared(), compressedData);
    std::string compressedData_str = compressedData.str();
    py::bytes output(compressedData_str);
    return output;
}
// 使用pcl解压点云，返回numpy array
//  src_bin: binary data
py::array_t<float> decode(py::bytes input, double pointResolution, double octreeResolution)
{
    pcl::io::compression_Profiles_e compressionProfile = pcl::io::MANUAL_CONFIGURATION;
    auto PointCloudEncoder = std::make_shared<pcl::io::OctreePointCloudCompression<pcl::PointXYZRGB>>(
        compressionProfile, false, pointResolution, octreeResolution, false, 100, false, 8); // 使用智能指针
    std::stringstream compressedData;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudOut(new pcl::PointCloud<pcl::PointXYZRGB>());
    // write py::bytes to compressedData
    std::string compressedData_str = input;
    compressedData << compressedData_str;
    PointCloudEncoder->decodePointCloud(compressedData, cloudOut);
    py::array_t<float> output({static_cast<int>(cloudOut->size()), 3});
    auto buf = output.request();
    float *ptr = (float *)buf.ptr;
    for (int i = 0; i < cloudOut->size(); i++)
    {
        ptr[i * 3] = cloudOut->points[i].x;
        ptr[i * 3 + 1] = cloudOut->points[i].y;
        ptr[i * 3 + 2] = cloudOut->points[i].z;
    }
    return output;
}
PYBIND11_MODULE(_pcl_compression, m)
{
    m.doc() = "pybind11 pcl octree pointcloud compression plugin"; // optional module docstring
    m.def("encode", &encode, "A function which encode the pointcloud and ouput the binary data.");
    m.def("decode", &decode, "A function which decode the binary and ouput the numpy array.");
}