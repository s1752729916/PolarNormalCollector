/**************************************************************************

Author: SMQ

Date:2021-10-21

Description:
完成图像的后处理，包括计算斯托克斯矢量，计算AoLP和DoLP

**************************************************************************/
#include "PolarImagesAcquistion.h"
#include <iostream>
inline double to_degrees(double radians) {
	return radians * (180.0 / 3.1415926535);
}
void AcquirePolarImages::Processing()
{
	INT32 s1, s2;
	S_0 = I_sum * 2;
	float fS1, fS2;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			s1 = I_0.at<UINT16>(row,col)-I_90.at<UINT16>(row,col);
			s2 = I_45.at<UINT16>(row, col) - I_135.at<UINT16>(row, col);
			// for 12bit pixel format, s1 and s2 possible values range here is -4095 to +4095  
			// this brings back into a 1-4094 range for display purposes
			// and sets to temporary S1, S2 images before scaling
			fS1 = double(s1);
			fS2 = double(s2) ;
			S_1.at<UINT16>(row, col) = int(fS1/2.0+2048.0);
			S_2.at<UINT16>(row, col) = int(fS2/2.0+2048.0);
			if (S_0.at<UINT16>(row, col) == 0)
			{
				DoLP.at<float>(row, col) = 1;
			}
			else
			{
				DoLP.at<float>(row, col) = sqrt(fS1 * fS1 + fS2 * fS2) / double(S_0.at<UINT16>(row, col));

			}
			//保证fS1不为0
			if (fS1 == 0.0)
				fS1 = 1.0;
			AoLP.at<float>(row, col) = atan(fS2/fS1)/2.0;  //单位是弧度
		}
	}

}

