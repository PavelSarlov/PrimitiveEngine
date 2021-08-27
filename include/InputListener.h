// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <Windows.h>

class InputListener
{
public:
	InputListener()
	{}

	~InputListener() 
	{}

public:
	// keyboard events
	virtual void onKeyDown(USHORT key) = 0;
	virtual void onKeyUp(USHORT key) = 0;

public:
	// mouse events
	virtual void onMouseMove(const POINT &mouse_pos) = 0;
	virtual void onLeftMouseDown(const POINT &mouse_pos) = 0;
	virtual void onLeftMouseUp(const POINT &mouse_pos) = 0;
	virtual void onRightMouseDown(const POINT &mouse_pos) = 0;
	virtual void onRightMouseUp(const POINT &mouse_pos) = 0;
};


#endif // !INPUTLISTENER_H
