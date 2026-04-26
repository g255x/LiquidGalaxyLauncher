#ifndef ACHIEVE
#define ACHIEVE
#include"INCLUDE.h"
#include"getuuid.h"
/***************************************
PRINT INFORMATION
***************************************/
/*
INCLUDE COLOR CODE
*/
bool parseHexColor(const std::string& hexColor, int& r, int& g, int& b) {
	std::string color = hexColor;
	if (color.size() > 0 && color[0] == '#') {
		color = color.substr(1);
	}
	if (color.size() != 6) {
		return false;
	}
	std::istringstream iss(color);
	unsigned int rgb = 0;
	iss >> std::hex >> rgb;
	if (iss.fail()) {
		return false;
	}
	r = (rgb >> 16) & 0xFF;
	g = (rgb >> 8) & 0xFF;
	b = rgb & 0xFF;
	return true;
}
void setPrintColor(const std::string& colorCode) {
	int r, g, b;
	if (!parseHexColor(colorCode, r, g, b)) {
		return;
	}
	std::cout << "\033[38;2;" << r << ';' << g << ';' << b << 'm';
}
/*
GET TIME
*/
std::string getCurrentTime() {
	auto now = std::chrono::system_clock::now();

	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	std::tm* now_tm = std::localtime(&now_c);
	if (now_tm == nullptr) {
		return "";
	}
	std::ostringstream oss;
	oss << std::put_time(now_tm, "%H:%M:%S");

	return oss.str();
}
/*
PRINT INFORMATION
*/
void printInformation(int type, string information) {
	/*
	1:INFO
	2:WARN
	3:ERROR
	4:CRASH
	*/
	switch (type) {
			cout << '\r';
		case 1:
			setPrintColor("#add8e6");
			cout << '[' << getCurrentTime() << ']' << " [INFO]: " << information;
			break;
		case 2:
			setPrintColor("#f0e68c");
			cout << '[' << getCurrentTime() << ']' << " [WARN]: " << information;
			break;
		case 3:
			setPrintColor("#dc143c");
			cout << '[' << getCurrentTime() << ']' << " [ERROR]: " << information;
			break;
		case 4:
			setPrintColor("#00ffff");
			cout << '[' << getCurrentTime() << ']' << " [!CRASH!]: " << information;
			break;
		default:
			printInformation(3, "PrintInformationError!");
			break;
	}
	cout << endl;
	setPrintColor("#ffffff");
}

/***************************************
CONVER COLOR
***************************************/
void mout(const std::string& text, const std::string& hexColor) {
	int r, g, b;
	if (!parseHexColor(hexColor, r, g, b)) {
		std::cout << text;
		return;
	}
	std::cout << "\033[38;2;" << r << ';' << g << ';' << b << 'm'
	          << text
	          << "\033[0m";
}
void printColoredChar(char ch, const std::string& colorCode) {
	int r, g, b;
	if (!parseHexColor(colorCode, r, g, b)) {
		std::cout << ch;
		return;
	}
	std::cout << "\033[38;2;" << r << ';' << g << ';' << b << 'm'
	          << ch
	          << "\033[0m";
}
void printColoredString(string ch, const std::string& colorCode) {
	int r, g, b;
	if (!parseHexColor(colorCode, r, g, b)) {
		std::cout << ch;
		return;
	}
	std::cout << "\033[38;2;" << r << ';' << g << ';' << b << 'm'
	          << ch
	          << "\033[0m";
}
int interpolate(int start, int end, double t) {
	return static_cast<int>(std::round(start + (end - start) * t));
}
void gradientPrint(const std::string& text, const std::string& startColor, const std::string& endColor) {
	int r1, g1, b1, r2, g2, b2;
	if (!parseHexColor(startColor, r1, g1, b1) || !parseHexColor(endColor, r2, g2, b2)) {
		printInformation(2, "Failed to display color");
		std::cout << text;
		return;
	}
	size_t visibleCount = 0;
	for (char ch : text) {
		if (ch != '\n') visibleCount++;
	}
	if (visibleCount == 0) {
		std::cout << text;
		return;
	}
	size_t pos = 0;
	for (char ch : text) {
		if (ch == '\n') {
			std::cout << ch;
		} else {
			double t = (visibleCount == 1) ? 0.0 : static_cast<double>(pos) / (visibleCount - 1);
			int r = interpolate(r1, r2, t);
			int g = interpolate(g1, g2, t);
			int b = interpolate(b1, b2, t);
			std::ostringstream oss;
			oss << '#' << std::hex << std::setfill('0')
			    << std::setw(2) << r
			    << std::setw(2) << g
			    << std::setw(2) << b;
			printColoredChar(ch, oss.str());
			pos++;
		}
	}
}
DWORD WINAPI ReadPipeThread(LPVOID lpParam) {
	HANDLE hPipe = (HANDLE)lpParam;
	char buffer[4096];
	DWORD bytesRead;
	std::string leftover;

	while (true) {
		BOOL success = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
		if (!success || bytesRead == 0) break;
		buffer[bytesRead] = '\0';
		std::string data = leftover + buffer;
		leftover.clear();

		size_t pos = 0;
		size_t newline;
		while ((newline = data.find('\n', pos)) != std::string::npos) {
			std::string line = data.substr(pos, newline - pos);
			if (!line.empty() && line.back() == '\r') line.pop_back();

			//PRINT
			if (line.find("INFO") != std::string::npos) {
				setPrintColor("#add8e6");
				cout << line << endl;
			} else if (line.find("WARN") != std::string::npos) {
				setPrintColor("#f0e68c");
				cout << line << endl;
			} else if (line.find("ERROR") != std::string::npos) {
				setPrintColor("#dc143c");
				cout << line << endl;
			} else {
				setPrintColor("#add8e6");
				cout << line << endl;
			}
			pos = newline + 1;
		}
		if (pos < data.size()) leftover = data.substr(pos);
	}
	return 0;
}

void comd(const std::string& exe, const std::string& cmd) {
	// COMMAND
	std::string quoted_exe;
	if (exe.find(' ') != std::string::npos)
		quoted_exe = "\"" + exe + "\"";
	else
		quoted_exe = exe;
	std::string fullCmd = quoted_exe + " " + cmd;
	std::vector<char> cmdline(fullCmd.begin(), fullCmd.end());
	cmdline.push_back('\0');

	// DO
	HANDLE hStdoutRd, hStdoutWr;
	HANDLE hStderrRd, hStderrWr;
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

	if (!CreatePipe(&hStdoutRd, &hStdoutWr, &sa, 0) ||
	        !CreatePipe(&hStderrRd, &hStderrWr, &sa, 0)) {
		printInformation(4, "Failed to create pipes");
		return;
	}

	SetHandleInformation(hStdoutRd, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(hStderrRd, HANDLE_FLAG_INHERIT, 0);

	// START INF
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = hStdoutWr;
	si.hStdError  = hStderrWr;

	// MAKE
	if (!CreateProcessA(
	            NULL,
	            cmdline.data(),
	            NULL,
	            NULL,
	            TRUE,
	            0,
	            NULL,
	            NULL,
	            &si,
	            &pi)) {
		DWORD err = GetLastError();
		printInformation(4, "Start Client ERROR, code: " + std::to_string(err));
		CloseHandle(hStdoutRd);
		CloseHandle(hStdoutWr);
		CloseHandle(hStderrRd);
		CloseHandle(hStderrWr);
		return;
	}

	// F WRITE
	CloseHandle(hStdoutWr);
	CloseHandle(hStderrWr);

	// READ
	HANDLE hThreads[2];
	hThreads[0] = CreateThread(NULL, 0, ReadPipeThread, hStdoutRd, 0, NULL);
	hThreads[1] = CreateThread(NULL, 0, ReadPipeThread, hStderrRd, 0, NULL);

	if (!hThreads[0] || !hThreads[1]) {
		printInformation(4, "Failed to create reader threads");
		TerminateProcess(pi.hProcess, 1); // KILL
	} else {
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	// CLOSE
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	CloseHandle(hStdoutRd);
	CloseHandle(hStderrRd);

	if (hThreads[0]) WaitForSingleObject(hThreads[0], 5000);
	if (hThreads[1]) WaitForSingleObject(hThreads[1], 5000);
	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);
}
/*****************************************
PRINT W TEXT
*****************************************/
std::wstring widen(const std::string& s) {
	int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
	std::wstring wstr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &wstr[0], len);
	return wstr;
}

// OUTPUT COLORFUL TEXT
void writeColoredWString(const std::wstring& text) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	WriteConsoleW(hConsole, text.c_str(), text.length(), &written, nullptr);
}

std::wstring ansiColorW(int r, int g, int b) {
	char buf[32];
	snprintf(buf, sizeof(buf), "\033[38;2;%d;%d;%dm", r, g, b);
	return widen(buf);
}

// PRINT
void gradientPrintW(const std::wstring& text, const std::string& startColor, const std::string& endColor) {
	int r1, g1, b1, r2, g2, b2;
	if (!parseHexColor(startColor, r1, g1, b1) || !parseHexColor(endColor, r2, g2, b2)) {
		writeColoredWString(text); //ERROR
		return;
	}

	size_t visibleCount = 0;
	for (wchar_t ch : text) {
		if (ch != L'\n') visibleCount++;
	}
	if (visibleCount == 0) {
		writeColoredWString(text);
		return;
	}

	size_t pos = 0;
	std::wstring output;
	for (wchar_t ch : text) {
		if (ch == L'\n') {
			output += ch;
		} else {
			double t = (visibleCount == 1) ? 0.0 : static_cast<double>(pos) / (visibleCount - 1);
			int r = static_cast<int>(std::round(r1 + (r2 - r1) * t));
			int g = static_cast<int>(std::round(g1 + (g2 - g1) * t));
			int b = static_cast<int>(std::round(b1 + (b2 - b1) * t));
			output += ansiColorW(r, g, b);
			output += ch;
			pos++;
		}
	}
	output += L"\033[0m"; // RESET
	writeColoredWString(output);
}
/*******************************
*
*   TP WINDOW
*
* *****************************/
void CenterConsoleWindow() {
	HWND hWnd = GetConsoleWindow();
	if (hWnd == NULL) return;

	RECT screenRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	int screenWidth = screenRect.right - screenRect.left;
	int screenHeight = screenRect.bottom - screenRect.top;

	RECT consoleRect;
	GetWindowRect(hWnd, &consoleRect);
	int consoleWidth = consoleRect.right - consoleRect.left;
	int consoleHeight = consoleRect.bottom - consoleRect.top;

	int newX = screenRect.left + (screenWidth - consoleWidth) / 2;
	int newY = screenRect.top + (screenHeight - consoleHeight) / 2;

	SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
#endif
