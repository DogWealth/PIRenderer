# PIRenderer
PIRenderer是使用C++实现的实时的软件光栅化渲染器，该渲染器没有使用任何图形API，自己实现了完整的可编程渲染管线，可以使用C++编写着色器（顶点/像素着色器）来实现不同的渲染效果。目前实现的功能有：

### Blinn-Phong
![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/Blinn-Phong.png?raw=true)

### Shadow

* Shadow Map
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/SM.png?raw=true)

* PCF
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/PCF.png?raw=true)

* PCSS
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/PCSS.png?raw=true)

* VSSM
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/VSSM.png?raw=true)

### PBR

* PBR
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/PBR.png?raw=true)

* IBL
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL1.png?raw=true)
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL2.png?raw=true)
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL4.png?raw=true)
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL5.png?raw=true)
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL6.png?raw=true)
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/IBL7.png?raw=true)



### Texture

* MipMapping可视化
  计算公式如下，但是由于CPU没办法得到uv坐标对x和y方向上的偏导，所以仅能利用扫描线光栅化算法得到x方向的偏导数，所以这里的计算结果可能不准确，但从可视化结果（mipmap0级到7级颜色分别是：红橙黄绿青蓝紫黑）来看是有一定作用的。

  ![img](https://pic4.zhimg.com/80/v2-e42602d143ec734c8b45136cd2fb28af_1440w.webp)

  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/Mipmapping.gif?raw=true)

* Nearest Filtering
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/Nearest.png?raw=true)

* Bilinear Filtering
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/Bilinear.png?raw=true)

* Trilinear Filtering
  在IBL中，对预滤波环境贴图采样时需要三线性插值，这里将三线性插值结果绘制成天空盒

  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/Trilinear.png?raw=true)

### 后处理

* 未进行gamma矫正和tone mapping
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/noGamma.png?raw=true)

* gamma矫正和tone mapping
  ![image-20240423111835243](https://github.com/DogWealth/PIRenderer/blob/main/picture/gamma.png?raw=true)



### 其他特性

* 齐次空间裁剪
  * 直接剔除
    ![culling.gif](https://s2.loli.net/2024/03/04/i8l62cbfKqvNIdy.gif)
  * 裁剪剔除
    ![Homogeneous_clipping.gif](https://s2.loli.net/2024/03/04/fuSYRkbg3GTNJjQ.gif)




* 透视矫正
  * 无透视矫正
    ![floor.gif](https://s2.loli.net/2024/03/01/hgaBD8wJc1SWrvn.gif)
  * 正确透视
    ![correct_gloor.gif](https://s2.loli.net/2024/03/01/gsEZ3kTvWpI1lVG.gif)
