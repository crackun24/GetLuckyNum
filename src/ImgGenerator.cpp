#include "ImgGenerator.h"


Mat ImgGenerator::generateSingelCharacter(const char c, int f_height)
{
	lock_guard<recursive_mutex> lg(mLocker);
	FT_Error error;

	error = FT_Set_Pixel_Sizes(mFace, 0, f_height); //���������С
	if (error)
	{
		throw runtime_error("FT_Set_Pixel_Sizes failed");
	}

	error = FT_Select_Charmap(mFace, FT_ENCODING_UNICODE); //����ʹ�õ��ַ���

	if (error)
	{
		throw runtime_error("FT_Select_Charmap failed");
	}

	error = FT_Load_Char(mFace, c, FT_LOAD_RENDER); //�����ַ�

	if (error)
	{
		throw runtime_error("FT_Load_Char failed");
	}

	FT_GlyphSlot slot = mFace->glyph;
	FT_Bitmap bitmap = slot->bitmap;
	return Mat(bitmap.rows, bitmap.width, CV_8UC1, bitmap.buffer, bitmap.pitch);
}

P_ImgGenerator ImgGenerator::getInstance()
{
	P_ImgGenerator temp(new ImgGenerator());

	if (FT_Init_FreeType(&temp->mLibrary)) //��ʼ����
	{
		return nullptr;
	}

	if (FT_New_Face(temp->mLibrary, "./font.ttf", 0, &temp->mFace)) //��������
	{
		return nullptr;
	}
	return temp;
}

Mat ImgGenerator::generateImg(const string& text, int f_width, int f_height, int fontSize)
{
	lock_guard<recursive_mutex> lg(mLocker);

	vector<Mat> imgs;

	int width{0}, height{0}; //�������Ŀ��

	for (const auto ch : text)
	{
		auto img = generateSingelCharacter(ch, f_height);//���ɵ����ַ���ͼƬ
		imshow("test", img);//TODO TEST
		waitKey(0);//TODO TEST

		imgs.push_back(move(img)); //�ƶ�������������

		height = max(height, img.rows);
		width += img.cols; //������
	}


	return Mat(); //TODO FIXME
}
