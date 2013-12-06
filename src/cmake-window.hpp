/*
 * Copyright 2012-2013 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the Apache v2 license.
 */
#ifndef CMAKE_WINDOW_HPP
#define CMAKE_WINDOW_HPP

class CMakeWindow  :  public BWindow
{
public:
	CMakeWindow(BRect frame);
	BTextControl*	srcTextDir;
	BTextControl*	outTextDir;
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
	BEntry*			srcEntry;
	BEntry*			outEntry;
	
};

#endif
