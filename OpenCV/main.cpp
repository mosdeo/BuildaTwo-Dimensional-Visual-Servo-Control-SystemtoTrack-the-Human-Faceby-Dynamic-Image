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

	//cap(1);//�Ұ���v��
	if(!cap.isOpened())return -1;

	/****Video Set****/
	//���wCaptureSize
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

	/*==== ���J�ؼЯS�x ====*/	
	cv::CascadeClassifier cascade_FrontFace;//�����y
	std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml"; //Harr-like
	cascade_FrontFace.load(FrontFace_CascadeName);

	//cv::CascadeClassifier cascade_ProfileFace;//�����y
	//std::string ProfileFace_CascadeName="lbpcascade_profileface.xml"; //Harr-like
	//cascade_ProfileFace.load(ProfileFace_CascadeName);
	/*==== ���J���� ====*/


	//��y�v���j��
	while(1){
		static int nFPS=0;
		//��������e�ɶ�
		LARGE_INTEGER m_liPerfStart={0};
		QueryPerformanceCounter(&m_liPerfStart);

		cap >> frameRaw;
		cv::resize(frameRaw,frameDetect,cv::Size(frameRaw.cols/SPEED,frameRaw.rows/SPEED));
		cv::flip(frameRaw,frameRaw,1);
		cv::flip(frameDetect,frameDetect,1);
		cv::cvtColor(frameDetect,frameDetect,cv::COLOR_BGR2GRAY);
		cv::equalizeHist(frameDetect,frameDetect);
		cv::imshow("Detect",frameDetect);

		//�ŧi�s���y��T���V�q
		cv::Point FaceCenter;
		std::vector<cv::Rect> faceTheLoop;
		static std::vector<cv::Rect> face;
		static std::deque<bool> bFaceExist(30); //�������L�������y���ǦC
		static bool bFaceContinuousNotExist; //�Ӥ[�S�������y//�v�K�S�o�i�X�� 

		//����
		const int nDetectSize=28;

		//���������y
		cascade_FrontFace.detectMultiScale(frameDetect,faceTheLoop,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));

		bFaceExist.pop_front(); //�M���̦Ѫ�����

		//���y��?
		if(0!=faceTheLoop.size())
		{
			bFaceExist.push_back(true); //push�@��"��"��mark
			face=faceTheLoop; //��s��T
		}
		else
		{
			bFaceExist.push_back(false); //push�@��"�L"��mark

			//�p��O�_�s��S�y? (���Bset �s��=0.5��)
			for(int i=bFaceExist.size()-1;i>bFaceExist.size()-(nFPS*0.5);i--)
				bFaceContinuousNotExist |= bFaceExist[i];
			bFaceContinuousNotExist=!bFaceContinuousNotExist;
			
			//�s�򳣨S�y��?
			if(bFaceContinuousNotExist)
				face=faceTheLoop; //��s��T
		}

		for(int i=0;i<bFaceExist.size();i++)
		{
			std::cout << bFaceExist[i];
		}
		std::cout << ",";

		//�Х��y��
		DrawFaceBox(&frameRaw,face,&FaceCenter,SPEED);

		const int COM_Num=-1;
		ShowText(FaceCenter.x,FaceCenter.y,
			(FaceCenter.x-frameRaw.cols/2),(FaceCenter.y-frameRaw.rows/2),
			"DELETE",
			&frameRaw,
			COM_Num,CaptureSize.width,CaptureSize.height,nFPS);
		//std::cout << Get_strControlHV + "\t ";

		const double dShowMAG=1.0; //�i�ܵe����j���v
		cv::resize(frameRaw,frameShow,cv::Size((int)(dShowMAG*frameRaw.cols),(int)(dShowMAG*frameRaw.rows))); //��jX��show�X

		ShowLIVE_Box(&frameShow); //Show "LIVE" Box

		cv::imshow("WebCam(Engineering Mode)",frameShow);

		if(bRecode)	writer << frameRaw;
		if(cv::waitKey(1)==27)break;

		// ���o�ثe CPU frequency
		LARGE_INTEGER m_liPerfFreq={0};
		QueryPerformanceFrequency(&m_liPerfFreq);

		//���q�����ɶ�
		LARGE_INTEGER liPerfNow={0};
		QueryPerformanceCounter(&liPerfNow);

		// �p��X Total �һݭn���ɶ� (millisecond)
		long long decodeDulation=( ((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)/m_liPerfFreq.QuadPart);
		nFPS=(int)(1000/decodeDulation);

		printf("FPS = %ld\n",nFPS);
	}
	return 0;
}

