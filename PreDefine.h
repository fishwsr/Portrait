#ifndef   _PRE_DEFINE_H_
#define  _PRE_DEFINE_H_

/////////////////////////////////////////////////////////////////
//文 件 名：   PreDefine.h                                     //
//文件描述：   人脸SDK预定义头文件                             //
//相关文件：   fdapi.h                                         //
//             fllapi.h                                        //
//             ftemapi.h                                       //
//作    者：   王乐珩、陈杰、牛志恒、曹波                      //
//编 辑 器：   Visual C++ 6.0                                  //
//版    权：   中科院计算所JDL实验室                           //
//版权说明：   任何使用必须保留版权信息和历史纪录信息          //
//修改纪录：                                                   //
//  日  期        版本      修改人         修改内容            //
//  07/07/2005    1.0       王乐珩、陈杰   创建                //
/////////////////////////////////////////////////////////////////

//各算法的错误返回信息
enum FaceAlgError
{
  // 1 至100 是通用的异常信息
  // 王乐珩(2005-07-07)
  errNone = 0,                             //无异常
  errNullArithmetic = 1,                   //算法未实现，空算法
  errFileNotExist = 2,                     //文件不存在
  errInvalidFileFormat = 3,                //文件格式非法
  errUnInit = 4,                           //算法没有初始化
  errNotEnoughMemory = 5,                  //内存不足
  errInvalidArgument = 6,                  //非法输入参数
  errInvalidClose = 7,                     //无法注销

  // 100 至199 是人脸检测算法异常信息
  // 陈杰 (2005-07-07)
  errDetectOther =100,                     //未知的检测算法异常(模块内必须用try 拦截所有异常)
  errIntegralOverflow = 101,               //积分图计算时出现溢出
  errComputeFeatureValue = 102,            //计算特征值时越界，就是说训练好的特征有问题
  errRemoveOverlaps = 103,                 //去除重框时出错
  errTooManyFindFace = 104,                //返回的人脸数目超过假设的最大值
  errDetectProcess = 105,                  //人脸检测过程出现问题
  errNoDetect = 106,                       //没有进行人脸检测直接调用了函数FDGetFaces

  // 200 至299 是特征点定位算法的异常信息
  // 牛志恒 (2005-07-07)
  errLocationgOther = 200,                 //未知的定位算法异常(模块内必须用try拦截所有异常)
  errIntegralImageOverflow = 201,          //积分图溢出
  errInvalidDetectionPosition = 202,       //获取检测位置失败
  errLocalizationFailed = 203,             //定位失败
  errInputImageFormatInvalid =204,         //无效的输入图像数据格式
  
  // 300 至399 是人脸模板提取和匹配算法的异常信息
  // 曹波 (2005-07-07)
  errRecognizeOther = 300,                //未知的算法异常(模块内必须用try拦截所有异常)
  errFaceOutBorder = 301                  //超出范围
};

//图象颜色深度，目前要求各个算法必须支持24位真彩色和8位灰度图象
enum BitsPerPixel
{
  Gray8 = 8,                  // 8位 灰度
  Color24 = 24                //24位 真彩
};

//特征点
enum LandmarkNames
{
  LeftEyeCenter = 1,          // 左眼中央（指图象左边）
  RightEyeCenter = 2,         // 右眼中央（指图象右边）
  NoseTip = 3,                // 鼻尖
  LeftMouthCorner = 4,        // 左嘴角（指图象左边）
  RightMouthCorner = 5,       // 右嘴角（指图象右边）
  MouthCenter = 6             // 嘴中央
};

//人脸信息
typedef struct
{
  int nLeft;                  // 人脸矩形框左上点X坐标
  int nTop;                   // 人脸矩形框左上点Y坐标
  int nWidth;                 // 人脸矩形框宽度
  int nHeight;                // 人脸矩形框高度
  double lfConfidence;        // 可信度，范围在0到+1.0之间
}FACEINFO;

//特征点统计信息
typedef struct
{
  double lfXMean;             // X均值
  double lfXVar;              // X方差
  double lfYMean;             // Y均值
  double lfYVar;              // Y方差
}FDSTATISTICS;

//特征点信息
typedef struct
{
  LandmarkNames enumLandmark; // 指定某个特征点，参考前面的枚举值LandmarkNames
  int x;                      // X坐标
  int y;                      // Y坐标
  double lfConfidence;        // 可信度，范围在0到+1.0之间
}LANDMARKINFO;
 
#endif//_PRE_DEFINE_H_