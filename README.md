## PCL OCTREE COMPRESSION PYTHON BINDING

It is a simple wrapper to use PCL octree pointcloud compression algorithm in python.

## Installation
Before installation,you have to make sure that the dependency libraries are installed such as [PCL](https://pointclouds.org/) and [Pybind11](https://github.com/pybind/pybind11)
``` cmd
cd pcl_compression_pybinding
pip install .
```
## Example
```python
import pcl_compression
import numpy as np

# Create a point cloud
pc=np.array([[1,2,3],[4,5,6],[7,8,9]],dtype=np.float32) 
pointResolution=0.1
octreeResolution=0.1
binary=pcl_compression.encode(pc,pointResolution,octreeResolution)
#the compressed file
with open("test.bin","wb") as f:
    f.write(binary)
reconstructed_pc=pcl_compression.decode(binary,pointResolution,octreeResolution)
print(reconstructed_pc)
```