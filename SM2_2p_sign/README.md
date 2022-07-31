# implement sm2 2P sign with real network communication



## 文件说明

#### ecc.py

实现了椭圆曲线的基本算法（基于SM2），重载了相关运算符

#### ALIce.py Bob.py

使用socket完成真实网络的模拟



## 算法过程

![image-20220731135021052](../image/image-20220731135021052.png)





## 运行结果

#### Alice

![image-20220731135236126](../image/image-20220731135236126.png)

#### Bob

![image-20220731135246992](../image/image-20220731135246992.png)



## 运行指导

先执行Bob.py  再执行Alice.py