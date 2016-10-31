/*
以動態影像追蹤人臉之二維視覺伺服控制系統
視窗介面原始碼
編譯環境:MSVC2012 + OpenCV 2.4.6
程式作者:林高遠,mosdeo@gmail.com
指導教授:陳美勇
修改日期:2013/10/20
*/
#include "stdafx.h"
#include "FaceTracking(8051+7257P+PWM).h"

// For Motor
const double dX_Kp=0.4/1.0,
			dY_Kp=0.6/1.0;

																																																																																																																																																								
// For Video
int SPEED=2; //SPEED=3,detectSize=16@Core2
const bool bRecode=true;
cv::Mat frameRaw,frameDetect,frameShow;
Resolution MainFrameResolution;

cv::VideoCapture cap(0);
/*
static int nTrackbarCurrentValue=25;
const int nTrackbarStopValue=40;
inline void OnTrackbar(int)
{
	cap.set(CV_CAP_PROP_FOCUS,nTrackbarCurrentValue);
};
*/

int main(){
	/****Com Port Set****/
	COM_PortInitialize();

	/****Video Set****/
	//指定CaptureSize
	cv::Size CaptureSize;
	std::string strWrite_Video_File_Name_And_Path;
	while(1)
	{
		std::cout << "\n請輸入down sample scale(1~10)\n";
		std::cin >> SPEED;
		if(SPEED>10||SPEED<1)
			continue;

		int nChoice=0;
		std::cout << "\n請選擇解析度\n";
		std::cout << "1. 320x240(QVGA)\n";
		std::cout << "2. 640x360\n";
		std::cout << "3. 640x480(VGA)\n";
		std::cout << "4. 1280x720\n";
		std::cout << "5. 1920x1080(FULL HD)\n";
		std::cin >> nChoice;

		switch(nChoice)
		{
		case 1:{
			CaptureSize=MainFrameResolution.QVGA_320x240();
			strWrite_Video_File_Name_And_Path="capture_QVGA.avi";
			SPEED=1;
			break;}
		case 2:{
			CaptureSize=MainFrameResolution.WideScreen640x360();
			strWrite_Video_File_Name_And_Path="capture_640x360.avi";
			break;}
		case 3:{
			CaptureSize=MainFrameResolution.VGA_640x480();
			strWrite_Video_File_Name_And_Path="capture_640x480.avi";
			break;}
		case 4:{
			CaptureSize=MainFrameResolution.WideScreen1280x720();
			strWrite_Video_File_Name_And_Path="capture_1280x720.avi";
			break;}
		case 5:{
			CaptureSize=MainFrameResolution.WideScreen1920x1080();
			strWrite_Video_File_Name_And_Path="capture_FULL_HD.avi";
			break;}
		default:{
			system("Cls");
			std::cout << "\n\t不要亂按!\n\n\a";}
		}

		if(1<=nChoice && nChoice<=5)break;
	}

	/*********************/
	printf("SPEED=%d\n",SPEED);
	printf("Recode=%d\n",bRecode);
	
	strWrite_Video_File_Name_And_Path="D:\\"+ strWrite_Video_File_Name_And_Path;

	printf("\n\t指定 %dx%d\n",CaptureSize.width,CaptureSize.height);
	//啟動攝影機
	if(!cap.isOpened())return -1;
	
	static MotorPlant Cam1;
	/****COM Port test(點頭動作)****/
	Cam1.Nod(&hComm);

	cap.set(CV_CAP_PROP_FRAME_WIDTH,CaptureSize.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,CaptureSize.height);
	//cap.set(CV_CAP_PROP_FPS,100);
	//cap.set(CV_CAP_PROP_FOCUS,50);
	
	//指定的CaptureSize不一定被接受，所以，以被鏡頭韌體所接受的CaptureSize為CaptureSize
	CaptureSize.width=(int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	CaptureSize.height=(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	//最後為了確認，印出最後被鏡頭韌體所接受的CaptureSize
	printf("\n\t接受 %dx%d\n\n",(int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	/****Recode Set****/
	cv::VideoWriter writer;
	if(bRecode)
	{
		short int nWriterFPS = 20;
		writer.open(strWrite_Video_File_Name_And_Path,CV_FOURCC('D','I','V','X'),nWriterFPS,CaptureSize);
	}

	/*==== 載入目標特徵 ====*/	
	cv::CascadeClassifier cascade_FrontFace;//正面臉
	//std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml"; //Harr-like
	//std::string FrontFace_CascadeName="C:\\Users\\Lin Kao-Yaun\\Documents\\Visual Studio 2012\\Projects\\視覺伺服追蹤平台\\OpenCV\\haarcascade_frontalface_alt2.xml";
	std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml";
	cascade_FrontFace.load(FrontFace_CascadeName);
	//bool ReadXML = cascade_FrontFace.load(FrontFace_CascadeName);
	//std::cout << "ReadXML = " << ReadXML << std::endl;
	/*==== 載入結束 ====*/

	//Track Bar Initial
	/*
	cv::namedWindow("WebCam(Engineering Mode)");
	char trackbarname[]="threshold";
	cvCreateTrackbar(trackbarname,"WebCam(Engineering Mode)",&nTrackbarCurrentValue,nTrackbarStopValue,OnTrackbar);
	cvSetTrackbarPos(trackbarname,"WebCam(Engineering Mode)",25);
	*/

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
		//cv::equalizeHist(frameDetect,frameDetect);
		cv::imshow("Detect",frameDetect);

		//宣告存放臉資訊的向量
		cv::Point FaceCenter;
		std::vector<cv::Rect> face;
		
		//偵測
		const int nDetectSize=28;

		//先偵測正臉
		cascade_FrontFace.detectMultiScale(frameDetect,face,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));
		//cascade_FrontFace.detectSingleScale(frameDetect,1,face,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));
			
		

		if(0!=face.capacity()){
			/*
			如果偵測到臉
				1.計算X 與 Y 方向 pixel error
				2.對馬達平台送出命令
				3.秀出座標等資訊
			*/

			//標示臉部
			DrawFaceBox(&frameRaw,face,&FaceCenter,SPEED);

			/*警示偵測專用*/ //std::cout << "\a" ;
			int nPixelErrX=FaceCenter.x-frameRaw.cols/2; //計算像素誤差
			int nPixelErrY=FaceCenter.y-frameRaw.rows/2; //計算像素誤差
			int nH=(int)(dX_Kp*nPixelErrX/3.2);//P-Control
			int nV=(int)(dY_Kp*nPixelErrY/2.4);//P-Control
			Cam1.SetPWM(nV,nH,&hComm);}
		else{
			/*如果沒偵測到臉，做這些事*/
			Cam1.SetPWM(0,0,&hComm);}

		ShowText(FaceCenter.x,FaceCenter.y,
				(FaceCenter.x-frameRaw.cols/2),(FaceCenter.y-frameRaw.rows/2),
				Cam1.Get_strControlHV(),
				&frameRaw,
				COM_Num,CaptureSize.width,CaptureSize.height,nFPS);
		std::cout << Cam1.Get_strControlHV() + "\t ";


		const double dShowMAG=1.0; //展示畫面放大倍率
		cv::resize(frameRaw,frameShow,cv::Size((int)(dShowMAG*frameRaw.cols),(int)(dShowMAG*frameRaw.rows))); //放大X倍show出

		ShowLIVE_Box(&frameShow); //Show "LIVE" Box
		ShowLIVE_Box(&frameRaw); //Show "LIVE" Box
		
		//cv::namedWindow("WebCam(Engineering Mode)", CV_WINDOW_NORMAL);
		//cv::setWindowProperty("WebCam(Engineering Mode)", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		//cv::resizeWindow("WebCam(Engineering Mode)",640,320);
		cv::imshow("WebCam(Engineering Mode)",frameShow);
		//cv::VideoWriter TakePicture;
		

		
		int KeyPressCode=cv::waitKey(1);//存入按鍵碼
		if(bRecode)	writer << frameRaw;
		if(KeyPressCode==27)break;

		//拍照的
		std::vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		if(KeyPressCode==32)
		{
			static unsigned int PicNumber=0;
			std::string PicFilePath="Pic.jpg";
			
			cv::Mat ExistTest;
			do{
				PicFilePath="Pic.jpg";
				PicNumber++;
				PicFilePath.insert(3,std::to_string(PicNumber));
				ExistTest=cv::imread(PicFilePath);
			}while(!ExistTest.empty());

			std::cout<<"\a";
			cv::imwrite(PicFilePath,frameShow);
		}

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
		Cam1.SetPWM(0,0,&hComm);
		Cam1.~MotorPlant();
		CloseHandle(hComm);
		return 0;
}

