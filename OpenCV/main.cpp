//#include "stdafx.h"
//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//#include <fstream>
//#include <cstring>
//#include <sstream>
#include <Windows.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <deque>
#include "function.h"


// For Video
int SPEED=2;
const bool bRecode=true;
cv::Mat frameRaw,frameDetect,frameShow;
//Resolution MainFrameResolution;

cv::VideoCapture cap(1);

int main(){
	/****Com Port Set****/
	COM_PortInitialize();

	/*********************/
	printf("SPEED=%d\n",SPEED);
	printf("Recode=%d\n",bRecode);

	//strWrite_Video_File_Name_And_Path="D:\\"+ strWrite_Video_File_Name_And_Path;

	//cap(1);//啟動攝影機
	if(!cap.isOpened())return -1;

	/****Video Set****/
	//指定CaptureSize
	cv::Size CaptureSize;
	CaptureSize.width=(int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	CaptureSize.height=(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::string strWrite_Video_File_Name_And_Path;

	/****Recode Set****/
	cv::VideoWriter writer;
	if(bRecode)
	{
		short int nWriterFPS = 20;
		writer.open(strWrite_Video_File_Name_And_Path,CV_FOURCC('D','I','V','X'),nWriterFPS,CaptureSize);
	}

	/*==== 載入目標特徵 ====*/	
	cv::CascadeClassifier cascade_FrontFace;//正面臉
	std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml"; //Harr-like
	cascade_FrontFace.load(FrontFace_CascadeName);

	//cv::CascadeClassifier cascade_ProfileFace;//正面臉
	//std::string ProfileFace_CascadeName="lbpcascade_profileface.xml"; //Harr-like
	//cascade_ProfileFace.load(ProfileFace_CascadeName);
	/*==== 載入結束 ====*/


	//串流影像迴圈
	while(1){
		static int nFPS=0;
		//紀錄執行前時間
		LARGE_INTEGER m_liPerfStart={0};
		QueryPerformanceCounter(&m_liPerfStart);

		cap >> frameRaw;
		cv::resize(frameRaw,frameDetect,cv::Size(frameRaw.cols/SPEED,frameRaw.rows/SPEED));
		cv::flip(frameRaw,frameRaw,1);
		cv::flip(frameDetect,frameDetect,1);
		cv::cvtColor(frameDetect,frameDetect,cv::COLOR_BGR2GRAY);
		cv::equalizeHist(frameDetect,frameDetect);
		cv::imshow("Detect",frameDetect);

		//宣告存放臉資訊的向量
		cv::Point FaceCenter;
		std::vector<cv::Rect> faceTheLoop;
		static std::vector<cv::Rect> face;
		static std::deque<bool> bFaceExist(30); //紀錄有無偵測到臉的序列
		static bool bFaceContinuousNotExist; //太久沒偵測到臉//史密特濾波旗標 

		//偵測
		const int nDetectSize=28;

		//先偵測正臉
		cascade_FrontFace.detectMultiScale(frameDetect,faceTheLoop,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));

		bFaceExist.pop_front(); //清除最老的元素

		//有臉嗎?
		if(0!=faceTheLoop.size())
		{
			bFaceExist.push_back(true); //push一個"有"的mark
			face=faceTheLoop; //更新資訊
		}
		else
		{
			bFaceExist.push_back(false); //push一個"無"的mark

			//計算是否連續沒臉? (此處set 連續=0.5秒)
			for(int i=bFaceExist.size()-1;i>bFaceExist.size()-(nFPS*0.5);i--)
				bFaceContinuousNotExist |= bFaceExist[i];
			bFaceContinuousNotExist=!bFaceContinuousNotExist;
			
			//連續都沒臉嗎?
			if(bFaceContinuousNotExist)
				face=faceTheLoop; //更新資訊
		}

		for(int i=0;i<bFaceExist.size();i++)
		{
			std::cout << bFaceExist[i];
		}
		std::cout << ",";

		//標示臉部
		DrawFaceBox(&frameRaw,face,&FaceCenter,SPEED);

		const int COM_Num=-1;
		ShowText(FaceCenter.x,FaceCenter.y,
			(FaceCenter.x-frameRaw.cols/2),(FaceCenter.y-frameRaw.rows/2),
			"DELETE",
			&frameRaw,
			COM_Num,CaptureSize.width,CaptureSize.height,nFPS);
		//std::cout << Get_strControlHV + "\t ";

		const double dShowMAG=1.0; //展示畫面放大倍率
		cv::resize(frameRaw,frameShow,cv::Size((int)(dShowMAG*frameRaw.cols),(int)(dShowMAG*frameRaw.rows))); //放大X倍show出

		ShowLIVE_Box(&frameShow); //Show "LIVE" Box

		cv::imshow("WebCam(Engineering Mode)",frameShow);

		if(bRecode)	writer << frameRaw;
		if(cv::waitKey(1)==27)break;

		// 取得目前 CPU frequency
		LARGE_INTEGER m_liPerfFreq={0};
		QueryPerformanceFrequency(&m_liPerfFreq);

		//測量執行後時間
		LARGE_INTEGER liPerfNow={0};
		QueryPerformanceCounter(&liPerfNow);

		// 計算出 Total 所需要的時間 (millisecond)
		long long decodeDulation=( ((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)/m_liPerfFreq.QuadPart);
		nFPS=(int)(1000/decodeDulation);

		printf("FPS = %ld\n",nFPS);
	}
	return 0;
}

