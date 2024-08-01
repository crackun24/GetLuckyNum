#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <memory>
#include <spdlog/spdlog.h>
#include <mutex>

using namespace cv;
using namespace std;

class ImgGenerator;

typedef shared_ptr<ImgGenerator> P_ImgGenerator;

class ImgGenerator
{
private:
	recursive_mutex mLocker; //线程锁
	FT_Library mLibrary; //字体库对象
	FT_Face mFace; //字体对象
	ImgGenerator() = default; //禁止使用默认的构造函数
	Mat generateSingelCharacter(const char c, int f_height); //渲染单个字符

public:
	static P_ImgGenerator getInstance(); //获取实例
	Mat generateImg(const string& text, int f_width, int f_height, int fontSize); //生成图片
};
