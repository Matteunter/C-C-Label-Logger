/****** KEYLOGGER WITH TIMESTAMPS IN MS - AUTHOR: MATTEO UNTERHORST 04-03-2020 Version 1.2******/
// added remote 1 to 5 control
// added sound feedback

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono> //for 2nd timestamp utc

using namespace std;

//int time() { // Local Time [C] have to write in C because MRM C app doesn't support C++ commands, and I had to incorporate this function in the MRM app
//	SYSTEMTIME st;
//	GetSystemTime(&st);
//	int time = st.wHour * 3600000 + st.wMinute * 60000 + st.wSecond * 1000 + st.wMilliseconds;
//	return time;
//}

unsigned __int64 time() { // Time in UTC since 1970 [C++]
	unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//std::cout << now << std::endl;
	return now;
}

int mapkey(int key) {
	switch (key) {
	case 9:
		return 5;
	case 38:
		return 4;
	case 40:
		return 3;
	case 175:
		return 2;
	case 174:
		return 1;
	default:
		return 0;
	}
}

int main(int argc, char* argv[])
{
	char fileprefix[256];
	char filename[256]="keylog.txt";
	bool logging = true;
	int KEY = 0;
	int lastKEY = 0;

	if (argc > 1) {
		strcpy_s(fileprefix, argv[1]);
		sprintf_s(filename, "%s.txt", fileprefix);
	}

	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	
	while (logging == true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {
				if (KEY != lastKEY) {
					lastKEY = KEY;
					if (char(KEY) == 'C') {
						logging = false;
						break;
					}
					fstream LogFile;
					LogFile.open(filename, fstream::app);
					if (LogFile.is_open()) {
						LogFile << mapkey(KEY) << "," << unsigned __int64(time()) << endl;
						LogFile.close();
						cout << mapkey(KEY) << endl;
						cout << '\a'; // SOUND
					}
					while (GetAsyncKeyState(KEY) != 0) {
						//cout << "waiting for key release" << endl;
						Sleep(10);
					}
				}
			}
		}
	}
	return 0;
}