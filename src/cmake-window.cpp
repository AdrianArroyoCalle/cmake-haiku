/*
 * Copyright 2012-2013 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the Apache v2 license.
 */


//includes
#include <StorageKit.h>
#include "main.hpp"
#include "cmake-window.hpp"

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
const int32 CONF='CONF';
const int32 SRC_DIR='SRC';
const int32 OUT_DIR='OUT';
const int32 SRC_DIR_PANEL='SRC_';
const int32 OUT_DIR_PANEL='OUT_';
unsigned char color[3]={220,220,220};


CMakeWindow::CMakeWindow(BRect frame) 
	: BWindow(frame, "CMake for Haiku", B_TITLED_WINDOW,0)
{
	count=0;
	view = new BView(Bounds(), NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	AddChild(view);

	view->SetViewColor(color[0],color[1],color[2]); //SetColor of the frame
	
	BButton* srcDir=new BButton(BRect(4,10,200,20),NULL,"Source dir",
		new BMessage(SRC_DIR));
	view->AddChild(srcDir);
	
	srcTextDir=new BTextControl(BRect(200,10,600,50),NULL,NULL,NULL,NULL);
	view->AddChild(srcTextDir);
	
	
	BButton* outDir=new BButton(BRect(4,60,200,50),NULL,"Output dir",
		new BMessage(OUT_DIR));
	view->AddChild(outDir);
	
	outTextDir=new BTextControl(BRect(200,60,600,50),NULL,NULL,NULL,NULL);
	view->AddChild(outTextDir);

	BButton* configure=new BButton(BRect(4,300,200,50),NULL,"Configure",
		new BMessage(CONF));
	view->AddChild(configure);
	
	BButton* generate=new BButton(BRect(4,350,200,50),NULL,"Generate",
		new BMessage(GENERATE));
	view->AddChild(generate);
                        
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

		case SRC_DIR:
		{
			BFilePanel* panel=new BFilePanel(B_OPEN_PANEL,new BMessenger(this),NULL,
				B_DIRECTORY_NODE,false, new BMessage(SRC_DIR_PANEL));
			panel->Show();
			break;
		}
		case SRC_DIR_PANEL:
		{
			if(msg->HasRef("refs"))
			{
				entry_ref ref;
				if(msg->FindRef("refs",0,&ref) == B_OK)
				{
					BEntry entry(&ref,true);
					BPath path;
					entry.GetPath(&path);
					srcTextDir->SetText(path.Path());
				}
			}
			break;
		}
		case OUT_DIR:
		{
			BFilePanel* panel=new BFilePanel(B_OPEN_PANEL, new BMessenger(this),NULL,
				B_DIRECTORY_NODE,false, new BMessage(OUT_DIR_PANEL));
			panel->Show();
			break;
		}
		case OUT_DIR_PANEL:
		{
			if(msg->HasRef("refs"))
			{
				entry_ref ref;
				if(msg->FindRef("refs",0,&ref) == B_OK)
				{
					BEntry entry(&ref,true);
					BPath path;
					entry.GetPath(&path);
					outTextDir->SetText(path.Path());
				}	
			}	
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
		}

	}

}
