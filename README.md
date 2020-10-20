# opengl-exercises

### main函数

- 初始化glad和glfw，创建opengl context，注册鼠标callback
- 给出.obj文件路径，创建Scene对象
- 给出.glsl文件路径，创建Shader对象
- 准备光源的有关信息，即其位置和rgb强度，以uniform形式写入Shader中
- 设定光源的三个衰减系数，写入Shader的uniform中
- 在game loop中：
  - 处理输入
  - 计算每个物体的MVP矩阵，写入Shader的uniform
  - 绘制Scene对象

（Scene对象在准备完毕后可以通过draw函数自我绘制，需要为draw函数提供一个Shader对象）
（Shader对象代表整个渲染管线，在为它提供glsl代码后还需要设定其中的uniform，然后Shader才能工作）

