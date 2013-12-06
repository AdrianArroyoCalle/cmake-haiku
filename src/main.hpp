/*
 * Copyright 2012 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the LGPL license.
 */
#ifndef MAIN_HPP
#define MAIN_HPP

//BeAPI

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <InterfaceKit.h>
#include <SupportDefs.h>
#include <Window.h>


//ANSI C-POSIX
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


class CMakeWindow  :  public BWindow
{
public:
	CMakeWindow(BRect frame);
	BTextControl*	srcdir;
	BTextControl*	outdir;
	BTextControl*	projecttc;
	bool			QuitRequested();
	void			MessageReceived(BMessage* msg);
	void			BuildCache();
	BTextControl*	addsub;
	BTextControl*	addexe;
	BCheckBox*		linecheck[2048];
	BView*			view;
	BButton*		exit;
private:
	int				count;
	const char*		line[2048];
	const char*		dirsrc;
	const char*		dirout;
	
};




#endif // _H
