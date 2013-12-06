/*
 * Copyright 2012-2013 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the BSD 3-clause license.
 */
#ifndef CMAKE_WINDOW_HPP
#define CMAKE_WINDOW_HPP

class CMakeWindow  :  public BWindow
{
public:
	CMakeWindow(BRect frame);
	BTextControl*	srcTextDir;
	BTextControl*	outTextDir;
	bool			QuitRequested();
	void			MessageReceived(BMessage* msg);
	BView*			view;
	BButton*		exit;
private:
	BEntry*			srcEntry;
	BEntry*			outEntry;
	BListView*	options;
	BTextControl*	edit;
	BButton*		apply;
	
};

#endif
