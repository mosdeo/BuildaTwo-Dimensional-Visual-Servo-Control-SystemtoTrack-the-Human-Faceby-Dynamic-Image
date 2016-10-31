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
	int FrontThickness=1;
	int FrontType[] = {cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX, 
		cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 
		cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC};


	cv::Scalar
		ColorGreen(0,255,0),
		ColorOrange(51,150,255),
		ColorYellow(10,250,250);
	cv::putText(*frame,"NTNU, Lin Gao-Yuan, mosdeo.at.gmail.com",cv::Point(5,20),FrontType[FrontNum],0.8,ColorGreen,FrontThickness,8,0);
	cv::putText(*frame,"x="+std::to_string(face_x),cv::Point(5,40),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"y="+std::to_string(face_y),cv::Point(5,60),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"PixErrX="+std::to_string(p_nErrX),cv::Point(5,80),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	cv::putText(*frame,"PixErrY="+std::to_string(p_nErrY),cv::Point(5,100),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);
	//cv::putText(*frame,"PWM="+p_strControlHV,cv::Point(5,120),FrontType[FrontNum],0.6,ColorOrange,FrontThickness,8,0);

	//左下顯示資訊
	cv::Point LeftButtonPoint(3,frame->rows-3);
	cv::putText(*frame,"COM Port="+std::to_string(p_nCOM_Num),
		LeftButtonPoint-cv::Point(0,60),FrontType[FrontNum],0.5,ColorYellow,1,8,0);
	cv::putText(*frame,"FPS="+std::to_string(p_nFPS),
		LeftButtonPoint-cv::Point(0,00),FrontType[FrontNum],0.5,ColorYellow,1,8,0);
};
inline void DrawFaceBox(cv::Mat* frameFaceBox,std::vector<cv::Rect> p_face,cv::Point* p_FaceCenter,int p_SPEED)
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


void COM_PortInitialize()
{
	printf("\n\t不使用Pan Tilt平台\n");
};