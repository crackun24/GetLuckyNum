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
	recursive_mutex mLocker; //�߳���
	FT_Library mLibrary; //��������
	FT_Face mFace; //�������
	ImgGenerator() = default; //��ֹʹ��Ĭ�ϵĹ��캯��
	Mat generateSingelCharacter(const char c, int f_height); //��Ⱦ�����ַ�

public:
	static P_ImgGenerator getInstance(); //��ȡʵ��
	Mat generateImg(const string& text, int f_width, int f_height, int fontSize); //����ͼƬ
};
