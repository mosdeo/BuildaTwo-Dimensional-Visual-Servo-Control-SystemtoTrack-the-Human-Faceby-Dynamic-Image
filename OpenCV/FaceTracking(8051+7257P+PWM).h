/*
�H�ʺA�v���l�ܤH�y���G����ı���A����t��
����������l�X
�{���@��:�L����,mosdeo@gmail.com
���ɱб�:�����i
�ק���:2013/11/2
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
		cv::Point((int)(frameShow->cols)-LiveBoxW-3,(int)(frameShow->rows)-13), //�o�@��O�@���cv::Point
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
	
	//���U��ܸ�T
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

		//�զ^scalar
		p_face[i].x=p_SPEED*p_face[i].x;
		p_face[i].y=p_SPEED*p_face[i].y;
		p_face[i].width=p_SPEED*p_face[i].width;
		p_face[i].height=p_SPEED*p_face[i].height;

		//�]�w�x�Υ|���I
		cv::Point cvFacePointA(p_face[i].x,p_face[i].y);
		cv::Point cvFacePointB(p_face[i].x + p_face[i].width,p_face[i].y);
		cv::Point cvFacePointC(p_face[i].x + p_face[i].width,p_face[i].y + p_face[i].height);
		cv::Point cvFacePointD(p_face[i].x,p_face[i].y + p_face[i].height);

		//FaceBox Side EVA ���rStyle ����
		cv::Point cvFacePointOffsetV(0,10);
		cv::Point cvFacePointOffsetH(10,0);

		//��r�_�I
		cv::Point cvFaceTextLine1(p_face[i].x+5 + p_face[i].width,p_face[i].y+20);
		cv::Point cvFaceTextLine2(p_face[i].x+5 + p_face[i].width,p_face[i].y+40);

		cv::Scalar cvFaceLineColor(0,76,211);

		//FaceBox�Ǥ�r
		cv::putText(*frameFaceBox,"OBJECT:",cvFaceTextLine1,0,0.8,cvFaceLineColor,2,8,0);
		cv::putText(*frameFaceBox,"HUMAN",cvFaceTextLine2,0,0.8,cvFaceLineColor,2,8,0);

		//�|���I�s�u
		int nThicknees=2;
		cv::line(*frameFaceBox,cvFacePointA-cvFacePointOffsetH,cvFacePointB+cvFacePointOffsetH,cvFaceLineColor,nThicknees);//�W��
		cv::line(*frameFaceBox,cvFacePointB-cvFacePointOffsetV,cvFacePointC+cvFacePointOffsetV,cvFaceLineColor,nThicknees);//�k��
		cv::line(*frameFaceBox,cvFacePointC+cvFacePointOffsetH,cvFacePointD-cvFacePointOffsetH,cvFaceLineColor,nThicknees);//�U��
		cv::line(*frameFaceBox,cvFacePointD+cvFacePointOffsetV,cvFacePointA-cvFacePointOffsetV,cvFaceLineColor,nThicknees);//����
	}
};
//inline void Input_TA7279P(int p_nV,int p_nH,HANDLE *p_hComm)
//{
//	unsigned long lrcTurn;
//	
//	strControlHV =int2str(p_nV) + "V" + int2str(p_nH) + "H";
//	WriteFile(*p_hComm,
//		strControlHV.c_str()/*strTurn.c_str()*/,	//���ǰe����ƪ���}
//		strControlHV.size()/*strTurn.size()*/,	//�ǤJ��ƪ��줸�ռ�
//		&lrcTurn,	//�Q�g�J���ܼƪ��줸�ռƪ��ܼƦ�}
//		NULL);	//���ϥΫD�P�B�ǿ�ɳ]��NULL
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
		/*���ϰ�ŧi*/
		unsigned long lrcTurn;
		/*�ŧi����*/

		WriteFile(*p_hComm,
		m_strControlHV.c_str(),//���ǰe����ƪ���}
		m_strControlHV.size(),//�ǤJ��ƪ��줸�ռ�
		&lrcTurn,	//�Q�g�J���ܼƪ��줸�ռƪ��ܼƦ�}
		NULL);	//���ϥΫD�P�B�ǿ�ɳ]��NULL
	};
	void Nod(HANDLE *p_hComm) //�I�Y�ʧ@
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
	
	std::cout << "�п�J���F����COM Port���X(0=���s��):"; std::cin >> COM_Num;
	if(COM_Num!=0){
		std::string COM_PortNum;

		if(COM_Num<10)
			COM_PortNum = "COM" + int2str(COM_Num) ;
		else if(COM_Num>=10)
			COM_PortNum = "\\\\.\\COM" + int2str(COM_Num) ;
    
		DCB dcb;
		hComm = CreateFile(
			COM_PortNum.c_str(),	//���V�ɮצW�٪�����
			GENERIC_READ | GENERIC_WRITE,	//�s���Ҧ�
			0,	//���ɼҦ�:���ɸ�COM Port�P�_
			NULL,//�^�Ǫ�HANDLE�O�_�i�H�Q�l�{���~��
			OPEN_EXISTING,	//���w�p��}���ɮ�
			1,	//�ɮ��ݩʤά����X��,0���P�B�ǿ�
			0);	//�ھڳW�w�]��0 (�ѤW����,�����D��])


		GetCommState(hComm,&dcb);
		BuildCommDCB("19200,n,8,1",&dcb);
		if(!SetCommState(hComm,&dcb)) exit(1);}
	else if(COM_Num==0)
		printf("\n\t���ϥ�Pan Tilt���x\n");
};


