/*
 * Copyright 2012 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the Apache v2 license.
 */


//includes

#include "main.hpp"

//Events for the programm
const int32 EXIT_CMAKE='EXIT';
const int32 CHANGE_SRC_DIR='R';
const int32 CHANGE_OUT_DIR='O';
const int32 SET_PROJECT='J';
const int32 ADD_SUB='SUB';
const int32 ADD_LIB='LIB';
const int32 ADD_EXE='EXE';
const int32 ADD_LINE='LIN';
const int32 INCLUDE='INC';
const int32 LINK_LIB='LINK';
const int32 GENERATE='GEN';
const int32 EDIT='EDIT';
const int32 RUN='RUN';
const int32 DELETE='DEL';
unsigned char color[3]={220,220,220};


CMakeWindow::CMakeWindow(BRect frame) 
	: BWindow(frame, "CMake for Haiku", B_TITLED_WINDOW,0)
{

	//Create GUI interface with buttons and BTextControls
	count=0;
	view = new BView(Bounds(), NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(view);

	view->SetViewColor(color[0],color[1],color[2]); //SetColor of the frame

	BButton* changesrc=new BButton(BRect(4,20,150,25),NULL,"Change source dir",
		new BMessage(CHANGE_SRC_DIR));
	view->AddChild(changesrc);

	srcdir=new BTextControl(BRect(158,20,400,25),NULL,"Write the source dir"
		,NULL,new BMessage(CHANGE_SRC_DIR));
	view->AddChild(srcdir);

	BButton* changeout=new BButton(BRect(4,60,150,25),NULL,"Change output dir"
		,new BMessage(CHANGE_OUT_DIR));
	view->AddChild(changeout);
                
	outdir=new BTextControl(BRect(158,60,400,25),NULL,"Write the output dir",
		NULL,new BMessage(CHANGE_OUT_DIR));
	view->AddChild(outdir);
			
    BButton* generate=new BButton(BRect(4,160,150,25),NULL,"Generate",
		new BMessage(GENERATE));
    view->AddChild(generate);

	BButton* seemake=new BButton(BRect(4,210,150,25),NULL,"Edit manually",
		new BMessage(EDIT));
	view->AddChild(seemake);

	BButton* run=new BButton(BRect(4,260,150,25),NULL,"Build",
		new BMessage(RUN));
	view->AddChild(run);

	projecttc=new BTextControl(BRect(158,110,400,25),NULL,"Write a line",
		NULL,new BMessage(ADD_LINE));
	view->AddChild(projecttc);

	BButton* addline=new BButton(BRect(4,110,150,25),NULL,"Add line",
		new BMessage(ADD_LINE));
	view->AddChild(addline);
                        
	exit=new BButton(BRect(4,400,200,50),NULL,"Exit CMake",
		new BMessage(EXIT_CMAKE));
	view->AddChild(exit);
                
}





bool 
CMakeWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();

}




void
CMakeWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{

		//Exit CMake
		case EXIT_CMAKE:
		{	
			printf("Exit CMake for Haiku"); 
			QuitRequested();
			break;
		}
		

		
		//Change source dir
		case CHANGE_SRC_DIR:
		{
			puts(srcdir->Text());
			dirsrc=srcdir->Text();
			break;
		}



		//Change output dir
		case CHANGE_OUT_DIR:
		{
			puts(outdir->Text());
			dirout=outdir->Text();
			break;
		}


		//Create CMakeLists.txt with the data of the char array
		case GENERATE:
		{
			FILE* cachefile;
			cachefile=fopen("CMakeLists.txt","w");
			int aux;
			fprintf(cachefile,"SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY %s)\n",outdir->Text()); // Set output dir
			for(aux=0;aux!=2048;aux++){
				if(line[aux]==NULL){   //There aren't any more lines
						break;
				}
				fprintf(cachefile,"%s\n",line[aux]); //Write in file
			}
			fclose(cachefile); //Close and save file          
			break;      
		}


		//Open StyledEdit with CMakeLists.txt
		case EDIT:
		{
			system("StyledEdit CMakeLists.txt");
			break;
		}


		//Build the project
		case RUN:
		{
			exit->SetLabel("Now building");
			char exe[2048]="cmake ";
			char make[2048]="make -C ";
			strcat(exe,dirsrc);
			system(exe);
			strcat(make,dirsrc);
			system(make);
			exit->SetLabel("Exit CMake");
			break;
		}


		//Add a line to char array and create a BCheckBox
		case ADD_LINE:
		{
			line[count]=projecttc->Text();
			linecheck[count]=new BCheckBox(BRect(158,((count+5)*25),400,25),NULL
				,projecttc->Text(),new BMessage(SET_PROJECT));
			view->AddChild(linecheck[count]);
			count++;
			puts(projecttc->Text());
			break;
		}

		default:
		{
			BWindow::MessageReceived(msg);
		}

	}

}
//Application constructor
class CMake : public BApplication
{
public:
	CMake() 
		: BApplication("application/cmake-haiku")
	{
	}   
	void
	ReadyToRun()
    {
		BWindow* win = new CMakeWindow(BRect(100,100,800,600));
		win->Show();
	}

};

int
main(void)
{
	//Create the application and run it
CMake app;
app.Run();
return 0;
}
