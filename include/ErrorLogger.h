#pragma once
#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <Window.h>
#include <string>

class ErrorLogger
{
public:
	static void log(std::string message);
	static void log(HRESULT hr, std::string message);
};

#endif // !ERRORLOGGER_H