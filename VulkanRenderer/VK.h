#pragma once

#define MYICON		101




// Header files
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


// Macro definitions
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

// Global function declarations for Windowing
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const char* gpszAppName;
static DWORD dwStyle;
static WINDOWPLACEMENT wpPrev;

// Global variables for Windowing
HWND ghwnd;
BOOL gbFullScreen;
BOOL gbActiveWindow;
