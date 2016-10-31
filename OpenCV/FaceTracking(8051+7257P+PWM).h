/*
以動態影像追蹤人臉之二維視覺伺服控制系統
視窗介面原始碼
程式作者:林高遠,mosdeo@gmail.com
指導教授:陳美勇
修改日期:2013/11/2
*/
inline std::string int2str(int &i){
        std::stringstream ss;
        ss << i;
        return ss.str();
};
inline void ShowLIVE_Box(cv::Mat *frameShow){
	short int LiveBoxW=80,LiveBoxH=40; 
	cv::Rect LiveBox(/*X*/(int)(frameShow->cols)-LiveBoxW-5,/*Y*/(int)(frameShow->rows)-LiveBoxH-5,LiveBoxW,LiveBoxH);
	cv::rectangle(*frameShow,LiveBox,cv::Scalar(255,255,255),2,8,0);
	cv::putText(*frameShow,"LIVE",
		cv::Point((int)(frameShow->cols)-LiveBoxW-3,(int)(frameShow->rows)-13), //這一行是一整個cv::Point
		0,1.2,cv::Scalar(255,255,255),2,8,0);
};
inline void ShowText(int face_x,int face_y,int p_nErrX,int p_nErrY,std::string p_strControlHV,cv::Mat *frame,int p_nCOM_Num,int p_nResolutionX,int p_nResolutionY,int p_nFPS){
	int FrontNum=0;
	int FrontThickness=2;
	int FrontType[] = {cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX, 
						cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 
						cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC};


	cv::Scalar
		ColorGreen(0,255,0),
		ColorOrange(51,150,255),
		ColorYellow(10,250,250);
	cv::putText(*frame,"NTNU, Lin Kao-Yuan",cv::Point(5,20),FrontType[FrontNum],0.8,ColorGreen,FrontThickness,8,0);
	cv::putText(*frame,"x="+int2str(face_x),cv::Point(5,40),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"y="+int2str(face_y),cv::Point(5,60),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"PixErrX="+int2str(p_nErrX),cv::Point(5,80),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"PixErrY="+int2str(p_nErrY),cv::Point(5,100),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"PWM="+p_strControlHV,cv::Point(5,120),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	
	//左下顯示資訊
	cv::Point LeftButtonPoint(3,frame->rows-3);
	cv::putText(*frame,"COM Port="+int2str(p_nCOM_Num),
		LeftButtonPoint-cv::Point(0,40),FrontType[FrontNum],0.5,ColorYellow,1,8,0);
	cv::putText(*frame,"Resolution="+int2str(p_nResolutionX)+"x"+int2str(p_nResolutionY),
		LeftButtonPoint-cv::Point(0,20),FrontType[FrontNum],0.5,ColorYellow,1,8,0);
	cv::putText(*frame,"FPS="+int2str(p_nFPS),
		LeftButtonPoint-cv::Point(0,00),FrontType[FrontNum],0.5,ColorYellow,1,8,0);
};
inline void DrawFaceBox(cv::Mat* frameFaceBox,std::vector<cv::Rect> &p_face,cv::Point* p_FaceCenter,int p_SPEED)
{
	for(int i=0;i<(int)p_face.capacity();i++)
	{
		p_FaceCenter->x=p_SPEED*(p_face[i].x + p_face[i].width/2);
		p_FaceCenter->y=p_SPEED*(p_face[i].y + p_face[i].height/2);

		//調回scalar
		p_face[i].x=p_SPEED*p_face[i].x;
		p_face[i].y=p_SPEED*p_face[i].y;
		p_face[i].width=p_SPEED*p_face[i].width;
		p_face[i].height=p_SPEED*p_face[i].height;

		//設定矩形四個點
		cv::Point cvFacePointA(p_face[i].x,p_face[i].y);
		cv::Point cvFacePointB(p_face[i].x + p_face[i].width,p_face[i].y);
		cv::Point cvFacePointC(p_face[i].x + p_face[i].width,p_face[i].y + p_face[i].height);
		cv::Point cvFacePointD(p_face[i].x,p_face[i].y + p_face[i].height);

		//FaceBox Side EVA 井字Style 偏移
		cv::Point cvFacePointOffsetV(0,10);
		cv::Point cvFacePointOffsetH(10,0);

		//文字起點
		cv::Point cvFaceTextLine1(p_face[i].x+5 + p_face[i].width,p_face[i].y+20);
		cv::Point cvFaceTextLine2(p_face[i].x+5 + p_face[i].width,p_face[i].y+40);

		cv::Scalar cvFaceLineColor(0,76,211);

		//FaceBox旁文字
		cv::putText(*frameFaceBox,"OBJECT:",cvFaceTextLine1,0,0.8,cvFaceLineColor,2,8,0);
		cv::putText(*frameFaceBox,"HUMAN",cvFaceTextLine2,0,0.8,cvFaceLineColor,2,8,0);

		//四個點連線
		int nThicknees=2;
		cv::line(*frameFaceBox,cvFacePointA-cvFacePointOffsetH,cvFacePointB+cvFacePointOffsetH,cvFaceLineColor,nThicknees);//上邊
		cv::line(*frameFaceBox,cvFacePointB-cvFacePointOffsetV,cvFacePointC+cvFacePointOffsetV,cvFaceLineColor,nThicknees);//右邊
		cv::line(*frameFaceBox,cvFacePointC+cvFacePointOffsetH,cvFacePointD-cvFacePointOffsetH,cvFaceLineColor,nThicknees);//下邊
		cv::line(*frameFaceBox,cvFacePointD+cvFacePointOffsetV,cvFacePointA-cvFacePointOffsetV,cvFaceLineColor,nThicknees);//左邊
	}
};
//inline void Input_TA7279P(int p_nV,int p_nH,HANDLE *p_hComm)
//{
//	unsigned long lrcTurn;
//	
//	strControlHV =int2str(p_nV) + "V" + int2str(p_nH) + "H";
//	WriteFile(*p_hComm,
//		strControlHV.c_str()/*strTurn.c_str()*/,	//欲傳送的資料的位址
//		strControlHV.size()/*strTurn.size()*/,	//傳入資料的位元組數
//		&lrcTurn,	//被寫入的變數的位元組數的變數位址
//		NULL);	//不使用非同步傳輸時設為NULL
//}

class MotorPlant
{
private:
//protected:
	int m_nPWM_V,m_nPWM_H;
	std::string m_strControlHV;
public:
	MotorPlant(){m_nPWM_V=0,m_nPWM_H=0;};
	~MotorPlant(){};
	int GetVerticalPWM(){return m_nPWM_V;};
	int GetHorizontalPWM(){return m_nPWM_H;};
	std::string Get_strControlHV(){return m_strControlHV;};
	void SetPWM(int p_nPWM_V,int p_nPWM_H,HANDLE *p_hComm)
	{
		m_nPWM_V=p_nPWM_V;
		m_nPWM_H=p_nPWM_H;
		m_strControlHV=int2str(m_nPWM_V) + "V" + int2str(m_nPWM_H) + "H";
		/*本區域宣告*/
		unsigned long lrcTurn;
		/*宣告結束*/

		WriteFile(*p_hComm,
		m_strControlHV.c_str(),//欲傳送的資料的位址
		m_strControlHV.size(),//傳入資料的位元組數
		&lrcTurn,	//被寫入的變數的位元組數的變數位址
		NULL);	//不使用非同步傳輸時設為NULL
	};
	void Nod(HANDLE *p_hComm) //點頭動作
	{
		SetPWM(0,0,p_hComm);
		SetPWM(25,0,p_hComm);
		Sleep(100);
		SetPWM(0,0,p_hComm);
		Sleep(100);
		SetPWM(-25,0,p_hComm);
		Sleep(100);
		SetPWM(0,0,p_hComm);
	};
};
class Resolution
{
public:
	//Wide
	cv::Size WideScreen160x120(){return cv::Size(160,120);};
	cv::Size WideScreen424x240(){return cv::Size(424,240);};
	cv::Size WideScreen640x360(){return cv::Size(640,360);};
	cv::Size WideScreen880x440(){return cv::Size(880,440);};
	cv::Size WideScreen960x544(){return cv::Size(960,544);};
	cv::Size WideScreen1280x720(){return cv::Size(1280,720);};
	cv::Size WideScreen1920x1080(){return cv::Size(1920,1080);};

	//4:3
	cv::Size QVGA_320x240(){return cv::Size(320,240);};
	cv::Size VGA_640x480(){return cv::Size(640,480);};
	cv::Size XGA_1024x768(){return cv::Size(1024,768);};
};

int COM_Num;
HANDLE hComm;
void COM_PortInitialize()
{
	
	std::cout << "請輸入馬達控制COM Port號碼(0=不連接):"; std::cin >> COM_Num;
	if(COM_Num!=0){
		std::string COM_PortNum;

		if(COM_Num<10)
			COM_PortNum = "COM" + int2str(COM_Num) ;
		else if(COM_Num>=10)
			COM_PortNum = "\\\\.\\COM" + int2str(COM_Num) ;
    
		DCB dcb;
		hComm = CreateFile(
			COM_PortNum.c_str(),	//指向檔案名稱的指標
			GENERIC_READ | GENERIC_WRITE,	//存取模式
			0,	//分享模式:分享該COM Port與否
			NULL,//回傳的HANDLE是否可以被子程序繼承
			OPEN_EXISTING,	//指定如何開啟檔案
			1,	//檔案屬性及相關旗標,0為同步傳輸
			0);	//根據規定設為0 (書上說的,不知道原因)


		GetCommState(hComm,&dcb);
		BuildCommDCB("19200,n,8,1",&dcb);
		if(!SetCommState(hComm,&dcb)) exit(1);}
	else if(COM_Num==0)
		printf("\n\t不使用Pan Tilt平台\n");
};


