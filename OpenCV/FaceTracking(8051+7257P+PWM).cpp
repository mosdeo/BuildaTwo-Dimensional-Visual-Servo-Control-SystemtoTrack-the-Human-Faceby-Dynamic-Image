/*
�H�ʺA�v���l�ܤH�y���G����ı���A����t��
����������l�X
�sĶ����:MSVC2012 + OpenCV 2.4.6
�{���@��:�L����,mosdeo@gmail.com
���ɱб�:�����i
�ק���:2013/10/20
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
	//���wCaptureSize
	cv::Size CaptureSize;
	std::string strWrite_Video_File_Name_And_Path;
	while(1)
	{
		std::cout << "\n�п�Jdown sample scale(1~10)\n";
		std::cin >> SPEED;
		if(SPEED>10||SPEED<1)
			continue;

		int nChoice=0;
		std::cout << "\n�п�ܸѪR��\n";
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
			std::cout << "\n\t���n�ë�!\n\n\a";}
		}

		if(1<=nChoice && nChoice<=5)break;
	}

	/*********************/
	printf("SPEED=%d\n",SPEED);
	printf("Recode=%d\n",bRecode);
	
	strWrite_Video_File_Name_And_Path="D:\\"+ strWrite_Video_File_Name_And_Path;

	printf("\n\t���w %dx%d\n",CaptureSize.width,CaptureSize.height);
	//�Ұ���v��
	if(!cap.isOpened())return -1;
	
	static MotorPlant Cam1;
	/****COM Port test(�I�Y�ʧ@)****/
	Cam1.Nod(&hComm);

	cap.set(CV_CAP_PROP_FRAME_WIDTH,CaptureSize.width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,CaptureSize.height);
	//cap.set(CV_CAP_PROP_FPS,100);
	//cap.set(CV_CAP_PROP_FOCUS,50);
	
	//���w��CaptureSize���@�w�Q�����A�ҥH�A�H�Q���Y����ұ�����CaptureSize��CaptureSize
	CaptureSize.width=(int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	CaptureSize.height=(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	//�̫ᬰ�F�T�{�A�L�X�̫�Q���Y����ұ�����CaptureSize
	printf("\n\t���� %dx%d\n\n",(int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	/****Recode Set****/
	cv::VideoWriter writer;
	if(bRecode)
	{
		short int nWriterFPS = 20;
		writer.open(strWrite_Video_File_Name_And_Path,CV_FOURCC('D','I','V','X'),nWriterFPS,CaptureSize);
	}

	/*==== ���J�ؼЯS�x ====*/	
	cv::CascadeClassifier cascade_FrontFace;//�����y
	//std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml"; //Harr-like
	//std::string FrontFace_CascadeName="C:\\Users\\Lin Kao-Yaun\\Documents\\Visual Studio 2012\\Projects\\��ı���A�l�ܥ��x\\OpenCV\\haarcascade_frontalface_alt2.xml";
	std::string FrontFace_CascadeName="haarcascade_frontalface_alt2.xml";
	cascade_FrontFace.load(FrontFace_CascadeName);
	//bool ReadXML = cascade_FrontFace.load(FrontFace_CascadeName);
	//std::cout << "ReadXML = " << ReadXML << std::endl;
	/*==== ���J���� ====*/

	//Track Bar Initial
	/*
	cv::namedWindow("WebCam(Engineering Mode)");
	char trackbarname[]="threshold";
	cvCreateTrackbar(trackbarname,"WebCam(Engineering Mode)",&nTrackbarCurrentValue,nTrackbarStopValue,OnTrackbar);
	cvSetTrackbarPos(trackbarname,"WebCam(Engineering Mode)",25);
	*/

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
		//cv::equalizeHist(frameDetect,frameDetect);
		cv::imshow("Detect",frameDetect);

		//�ŧi�s���y��T���V�q
		cv::Point FaceCenter;
		std::vector<cv::Rect> face;
		
		//����
		const int nDetectSize=28;

		//���������y
		cascade_FrontFace.detectMultiScale(frameDetect,face,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));
		//cascade_FrontFace.detectSingleScale(frameDetect,1,face,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(nDetectSize,nDetectSize));
			
		

		if(0!=face.capacity()){
			/*
			�p�G�������y
				1.�p��X �P Y ��V pixel error
				2.�ﰨ�F���x�e�X�R�O
				3.�q�X�y�е���T
			*/

			//�Х��y��
			DrawFaceBox(&frameRaw,face,&FaceCenter,SPEED);

			/*ĵ�ܰ����M��*/ //std::cout << "\a" ;
			int nPixelErrX=FaceCenter.x-frameRaw.cols/2; //�p�⹳���~�t
			int nPixelErrY=FaceCenter.y-frameRaw.rows/2; //�p�⹳���~�t
			int nH=(int)(dX_Kp*nPixelErrX/3.2);//P-Control
			int nV=(int)(dY_Kp*nPixelErrY/2.4);//P-Control
			Cam1.SetPWM(nV,nH,&hComm);}
		else{
			/*�p�G�S�������y�A���o�Ǩ�*/
			Cam1.SetPWM(0,0,&hComm);}

		ShowText(FaceCenter.x,FaceCenter.y,
				(FaceCenter.x-frameRaw.cols/2),(FaceCenter.y-frameRaw.rows/2),
				Cam1.Get_strControlHV(),
				&frameRaw,
				COM_Num,CaptureSize.width,CaptureSize.height,nFPS);
		std::cout << Cam1.Get_strControlHV() + "\t ";


		const double dShowMAG=1.0; //�i�ܵe����j���v
		cv::resize(frameRaw,frameShow,cv::Size((int)(dShowMAG*frameRaw.cols),(int)(dShowMAG*frameRaw.rows))); //��jX��show�X

		ShowLIVE_Box(&frameShow); //Show "LIVE" Box
		ShowLIVE_Box(&frameRaw); //Show "LIVE" Box
		
		//cv::namedWindow("WebCam(Engineering Mode)", CV_WINDOW_NORMAL);
		//cv::setWindowProperty("WebCam(Engineering Mode)", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		//cv::resizeWindow("WebCam(Engineering Mode)",640,320);
		cv::imshow("WebCam(Engineering Mode)",frameShow);
		//cv::VideoWriter TakePicture;
		

		
		int KeyPressCode=cv::waitKey(1);//�s�J����X
		if(bRecode)	writer << frameRaw;
		if(KeyPressCode==27)break;

		//��Ӫ�
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
		Cam1.SetPWM(0,0,&hComm);
		Cam1.~MotorPlant();
		CloseHandle(hComm);
		return 0;
}

