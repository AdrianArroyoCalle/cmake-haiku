/*
 * Copyright 2012-2013 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the BSD 3-clause license.
 */


//includes
#include <StorageKit.h>
#include <InterfaceKit.h>
#include <fstream>
#include <iostream>
#include <string>
#include "main.hpp"
#include "cmake-window.hpp"

//Events for the programm
const int32 EXIT_CMAKE='EXIT';
const int32 GENERATE='GEN';
const int32 EDIT='EDIT';
const int32 RUN='RUN';
const int32 DELETE='DEL';
const int32 CONF='CONF';
const int32 SRC_DIR='SRC';
const int32 OUT_DIR='OUT';
const int32 SRC_DIR_PANEL='SRC_';
const int32 OUT_DIR_PANEL='OUT_';
const int32 SELECT='SEL';
const int32 APPLY='APPL';
unsigned char color[3]={220,220,220};

CMakeWindow::CMakeWindow(BRect frame) 
	: BWindow(frame, "CMake for Haiku", B_TITLED_WINDOW,0)
{
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
	
	options=new BListView(BRect(200,100,600,400),NULL,B_SINGLE_SELECTION_LIST);
	view->AddChild(new BScrollView(NULL,options,B_FOLLOW_LEFT|B_FOLLOW_TOP,0,false,true));
    
    edit=new BTextControl(BRect(200,450,600,50),NULL,NULL,NULL,NULL);
    view->AddChild(edit);
	apply=new BButton(BRect(600,450,700,50),NULL,"Apply",
		new BMessage(APPLY));
	view->AddChild(apply);            
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
		case GENERATE:
		{
			BAlert* wait=new BAlert("CMake for Haiku","Please, wait while we are generating the project",
				"I understand",NULL,NULL,B_WIDTH_AS_USUAL,B_IDEA_ALERT);
			wait->Go(NULL);
			BString cmd="cd ";
			cmd.Append(outTextDir->Text());
			cmd.Append(" && cmake ");
			cmd.Append(srcTextDir->Text());
			system(cmd.String());
			wait->Quit();
			BAlert* ok=new BAlert("CMake for Haiku","Generated! Now you can build as usual",
				"OK",NULL,NULL,B_WIDTH_AS_USUAL,B_INFO_ALERT);
			ok->Go();
			break;
		}
		case CONF:
		{
			BAlert* wait=new BAlert("CMake for Haiku","Please, wait while we are configurating the project",
				"I understand",NULL,NULL,B_WIDTH_AS_USUAL,B_IDEA_ALERT);
			wait->Go(NULL);
			BString cmd="cd ";
			cmd.Append(outTextDir->Text());
			cmd.Append(" && cmake ");
			cmd.Append(srcTextDir->Text());
			system(cmd.String());
			wait->Quit();
			BString cacheFile=outTextDir->Text();
			cacheFile.Append("/CMakeCache.txt");
			std::ifstream cache;
			cache.open(cacheFile.String());
			std::string line;
			while(std::getline(cache,line))
			{
				if(line.find("//")!=0 && line.find("#")!=0 && line.compare("\n")!=0)
				{
					BString item=line.c_str();
					options->AddItem(new BStringItem(item));
				}
			}
			cache.close();
			options->SetSelectionMessage(new BMessage(SELECT));
			break;
		}
		case SELECT:
		{
			int32 index=options->CurrentSelection();
			edit->SetText(((BStringItem*)options->ItemAt(index))->Text());
			break;
		}
		case APPLY:
		{
			int32 index=options->CurrentSelection();
			options->ReplaceItem(index,new BStringItem(edit->Text()));
			std::fstream cache;
			BString cacheFile=outTextDir->Text();
			cacheFile.Append("/CMakeCache.txt");
			cache.open(cacheFile, std::fstream::out | std::fstream::trunc);
			for(int32 i=0;i<options->CountItems();i++)
			{
				cache << ((BStringItem*)options->ItemAt(i))->Text() << "\n";
			}
			cache.close();
			
		}
		default:
		{
			BWindow::MessageReceived(msg);
		}

	}

}
