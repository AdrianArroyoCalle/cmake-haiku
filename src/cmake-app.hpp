/*
 * Copyright 2012-2013 Adrián Arroyo Calle <adrian.arroyocalle@gmail.com>
 * All rights reserved. Distributed under the terms of the Apache v2 license.
 */
#include "main.hpp"
#include "cmake-window.hpp"

#ifndef CMAKE_APP_HPP
#define CMAKE_APP_HPP

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
#endif
