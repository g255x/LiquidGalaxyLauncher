#include"achieve.h"
#include"config.h"
#include"mod.h"
using namespace std;
void enableVTMode() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
// 全局变量
volatile bool g_bKeepRunning = false;
HANDLE hThread = NULL;

DWORD WINAPI zd(LPVOID lpParam) {
	while (g_bKeepRunning) {
		HWND hWnd = GetConsoleWindow();
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		Sleep(500);
	}
	return 0;
}

// 在命令处理函数中
void toggleTopWindow() {
	if (config("command", 2, "top") == "true") { // 当前是 true，要关闭
		printColoredString("Top The Window:[false]\n", "#87ceeb");
		config("command", 1, "top", "false");
		if (hThread != NULL) {
			g_bKeepRunning = false;
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}
	} else {  // 当前是 false，要开启
		printColoredString("Top The Window:[true]\n", "#87ceeb");
		config("command", 1, "top", "true");
		if (hThread == NULL) {
			g_bKeepRunning = true;
			hThread = CreateThread(NULL, 0, zd, NULL, 0, NULL);
		}
	}
}
int printlogo() {
	const wchar_t* logo = LR"(

	 ██████╗  █████╗ ██╗      █████╗ ██╗  ██╗██╗   ██╗
	██╔════╝ ██╔══██╗██║     ██╔══██╗╚██╗██╔╝╚██╗ ██╔╝
	██║  ███╗███████║██║     ███████║ ╚███╔╝  ╚████╔╝ 
	██║   ██║██╔══██║██║     ██╔══██║ ██╔██╗   ╚██╔╝  
	╚██████╔╝██║  ██║███████╗██║  ██║██╔╝ ██╗   ██║   
	 ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   
                                                  
    )";
	gradientPrintW(logo, "#d9afd9", "#97d9e1");
	logo = LR"(
    
    ==================================================================
                LiquidGalaxy-1.21.11 Client  -v 78.91
                                By - G255

    )";
	gradientPrintW(logo, "#97d9e1", "#d9afd9");
	cout << endl;
	return 0;
}
char path[MAX_PATH];
bool haveuser = true;
bool chooseuser = true;
bool initialization() {
	if (config("command", 2, "top") == "false") {
		printInformation(1, "The window is not in a topmost state");
		if (hThread != NULL) {
			g_bKeepRunning = false;
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}
	} else {
		printInformation(1, "The window is in topmost state");
		if (hThread == NULL) {
			g_bKeepRunning = true;
			hThread = CreateThread(NULL, 0, zd, NULL, 0, NULL);
		}
	}
	GetCurrentDirectoryA(MAX_PATH, path);
	bool ret = true;
	string th = path + string("\\.minecraft\\versions\\LiquidGalaxy-1.21.11");
	if (_chdir(th.c_str()) == 0) {
		printInformation(1, "Directory loaded successfully");
	} else {
		printInformation(3, "Failed to switch directory: " + th);
		ret = false;
	}

	printInformation(1, "Root directory: " + string(path));
	if (config("user", 2, "username1") == "error") {
		haveuser = false;
		printInformation(3, "User does not exist!");
	} else {
		haveuser = true;
	}
	if (config("user", 2, "nowuser") == "null") {
		chooseuser = false;
		printInformation(3, "No user selected!");
	} else {
		chooseuser = true;
	}
	return ret;
}
void printHelp() {
	/*
	Output help
	helper
	*/
	printColoredString("[user]", "#9370db");
	printColoredString(" Switch User   ", "#dda0dd");
	printColoredString("[start]", "#9370db");
	printColoredString(" Start Client   ", "#dda0dd");
	printColoredString("[set]", "#9370db");
	printColoredString(" Settings   ", "#dda0dd");
	printColoredString("[mod]", "#9370db");
	printColoredString(" Mods\n", "#dda0dd");
	printColoredString("[exit]", "#9370db");
	printColoredString(" Exit Program\n", "#dda0dd");
}
stringstream command() {
	stringstream com;


	com << "-Xmx" << config("command", 2, "Xmx");
	com << ' ';
	com << "-Dfile.encoding=GB18030";
	com << ' ';
	com << "-Dstdout.encoding=GB18030";
	com << ' ';
	com << "-Dstderr.encoding=GB18030";
	com << ' ';
	com << "-Djava.rmi.server.useCodebaseOnly=true";
	com << ' ';
	com << "-Dcom.sun.jndi.rmi.object.trustURLCodebase=false";
	com << ' ';
	com << "-Dcom.sun.jndi.cosnaming.object.trustURLCodebase=false";
	com << ' ';
	com << "-Dlog4j2.formatMsgNoLookups=true";
	com << ' ';
	com << "-Dlog4j.configurationFile=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\log4j2.xml";
	com << ' ';
	com << "-Dminecraft.client.jar=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\LiquidGalaxy-1.21.11.jar";
	com << ' ';
	com << "-Djava.net.useSystemProxies=true";
	com << ' ';
	com << "-XX:+UnlockExperimentalVMOptions";
	com << ' ';
	com << "-XX:+UnlockDiagnosticVMOptions";
	com << ' ';
	com << "-XX:+UseG1GC";
	com << ' ';
	com << "-XX:G1MixedGCCountTarget=5";
	com << ' ';
	com << "-XX:G1NewSizePercent=20";
	com << ' ';
	com << "-XX:G1ReservePercent=20";
	com << ' ';
	com << "-XX:MaxGCPauseMillis=50";
	com << ' ';
	com << "-XX:G1HeapRegionSize=32m";
	com << ' ';
	com << "-XX:-OmitStackTraceInFastThrow";
	com << ' ';
	com << "-XX:-DontCompileHugeMethods";
	com << ' ';
	com << "-XX:MaxNodeLimit=240000";
	com << ' ';
	com << "-XX:NodeLimitFudgeFactor=8000";
	com << ' ';
	com << "-XX:TieredCompileTaskTimeout=10000";
	com << ' ';
	com << "-XX:ReservedCodeCacheSize=400M";
	com << ' ';
	com << "-XX:NonNMethodCodeHeapSize=12M";
	com << ' ';
	com << "-XX:ProfiledCodeHeapSize=194M";
	com << ' ';
	com << "-XX:NmethodSweepActivity=1";
	com << ' ';
	com << "-Dfml.ignoreInvalidMinecraftCertificates=true";
	com << ' ';
	com << "-Dfml.ignorePatchDiscrepancies=true";
	com << ' ';
	com << "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump";
	com << ' ';
	com << "-Djava.library.path=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\natives-windows-x86_64";
	com << ' ';
	com << "-Djna.tmpdir=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\natives-windows-x86_64";
	com << ' ';
	com << "-Dorg.lwjgl.system.SharedLibraryExtractPath=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\natives-windows-x86_64";
	com << ' ';
	com << "-Dio.netty.native.workdir=" << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\natives-windows-x86_64";
	com << ' ';
	com << "-Dminecraft.launcher.brand=G255";
	com << ' ';
	com << "-Dminecraft.launcher.version=2.55.5";
	com << ' ';

	com << "-cp";
	com << ' ';
	string pa = path;
	string cp[255] = {
		pa + "\\.minecraft\\libraries\\org\\ow2\\asm\\asm\\9.9\\asm-9.9.jar;",
		pa + "\\.minecraft\\libraries\\org\\ow2\\asm\\asm-analysis\\9.9\\asm-analysis-9.9.jar;",
		pa + "\\.minecraft\\libraries\\org\\ow2\\asm\\asm-commons\\9.9\\asm-commons-9.9.jar;",
		pa + "\\.minecraft\\libraries\\org\\ow2\\asm\\asm-tree\\9.9\\asm-tree-9.9.jar;",
		pa + "\\.minecraft\\libraries\\org\\ow2\\asm\\asm-util\\9.9\\asm-util-9.9.jar;",
		pa + "\\.minecraft\\libraries\\net\\fabricmc\\sponge-mixin\\0.17.0+mixin.0.8.7\\sponge-mixin-0.17.0+mixin.0.8.7.jar;",
		pa + "\\.minecraft\\libraries\\net\\fabricmc\\intermediary\\1.21.11\\intermediary-1.21.11.jar;",
		pa + "\\.minecraft\\libraries\\net\\fabricmc\\fabric-loader\\0.18.4\\fabric-loader-0.18.4.jar;",
		pa + "\\.minecraft\\libraries\\at\\yawk\\lz4\\lz4-java\\1.8.1\\lz4-java-1.8.1.jar;",
		pa + "\\.minecraft\\libraries\\com\\azure\\azure-json\\1.4.0\\azure-json-1.4.0.jar;",
		pa + "\\.minecraft\\libraries\\com\\github\\oshi\\oshi-core\\6.9.0\\oshi-core-6.9.0.jar;",
		pa + "\\.minecraft\\libraries\\com\\google\\code\\gson\\gson\\2.13.2\\gson-2.13.2.jar;",
		pa + "\\.minecraft\\libraries\\com\\google\\guava\\failureaccess\\1.0.3\\failureaccess-1.0.3.jar;",
		pa + "\\.minecraft\\libraries\\com\\google\\guava\\guava\\33.5.0-jre\\guava-33.5.0-jre.jar;",
		pa + "\\.minecraft\\libraries\\com\\ibm\\icu\\icu4j\\77.1\\icu4j-77.1.jar;",
		pa + "\\.minecraft\\libraries\\com\\microsoft\\azure\\msal4j\\1.23.1\\msal4j-1.23.1.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\authlib\\7.0.61\\authlib-7.0.61.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\blocklist\\1.0.10\\blocklist-1.0.10.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\brigadier\\1.3.10\\brigadier-1.3.10.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\datafixerupper\\9.0.19\\datafixerupper-9.0.19.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\jtracy\\1.0.37\\jtracy-1.0.37.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\jtracy\\1.0.37\\jtracy-1.0.37-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\logging\\1.6.11\\logging-1.6.11.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\patchy\\2.2.10\\patchy-2.2.10.jar;",
		pa + "\\.minecraft\\libraries\\com\\mojang\\text2speech\\1.18.11\\text2speech-1.18.11.jar;",
		pa + "\\.minecraft\\libraries\\commons-codec\\commons-codec\\1.19.0\\commons-codec-1.19.0.jar;",
		pa + "\\.minecraft\\libraries\\commons-io\\commons-io\\2.20.0\\commons-io-2.20.0.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-buffer\\4.2.7.Final\\netty-buffer-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-codec-base\\4.2.7.Final\\netty-codec-base-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-codec-compression\\4.2.7.Final\\netty-codec-compression-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-codec-http\\4.2.7.Final\\netty-codec-http-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-common\\4.2.7.Final\\netty-common-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-handler\\4.2.7.Final\\netty-handler-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-resolver\\4.2.7.Final\\netty-resolver-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-transport-classes-epoll\\4.2.7.Final\\netty-transport-classes-epoll-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-transport-classes-kqueue\\4.2.7.Final\\netty-transport-classes-kqueue-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-transport-native-unix-common\\4.2.7.Final\\netty-transport-native-unix-common-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\io\\netty\\netty-transport\\4.2.7.Final\\netty-transport-4.2.7.Final.jar;",
		pa + "\\.minecraft\\libraries\\it\\unimi\\dsi\\fastutil\\8.5.18\\fastutil-8.5.18.jar;",
		pa + "\\.minecraft\\libraries\\net\\java\\dev\\jna\\jna-platform\\5.17.0\\jna-platform-5.17.0.jar;",
		pa + "\\.minecraft\\libraries\\net\\java\\dev\\jna\\jna\\5.17.0\\jna-5.17.0.jar;",
		pa + "\\.minecraft\\libraries\\net\\sf\\jopt-simple\\jopt-simple\\5.0.4\\jopt-simple-5.0.4.jar;",
		pa + "\\.minecraft\\libraries\\org\\apache\\commons\\commons-compress\\1.28.0\\commons-compress-1.28.0.jar;",
		pa + "\\.minecraft\\libraries\\org\\apache\\commons\\commons-lang3\\3.19.0\\commons-lang3-3.19.0.jar;",
		pa + "\\.minecraft\\libraries\\org\\apache\\logging\\log4j\\log4j-api\\2.25.2\\log4j-api-2.25.2.jar;",
		pa + "\\.minecraft\\libraries\\org\\apache\\logging\\log4j\\log4j-core\\2.25.2\\log4j-core-2.25.2.jar;",
		pa + "\\.minecraft\\libraries\\org\\apache\\logging\\log4j\\log4j-slf4j2-impl\\2.25.2\\log4j-slf4j2-impl-2.25.2.jar;",
		pa + "\\.minecraft\\libraries\\org\\jcraft\\jorbis\\0.0.17\\jorbis-0.0.17.jar;",
		pa + "\\.minecraft\\libraries\\org\\joml\\joml\\1.10.8\\joml-1.10.8.jar;",
		pa + "\\.minecraft\\libraries\\org\\jspecify\\jspecify\\1.0.0\\jspecify-1.0.0.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-freetype\\3.3.3\\lwjgl-freetype-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-freetype\\3.3.3\\lwjgl-freetype-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-freetype\\3.3.3\\lwjgl-freetype-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-freetype\\3.3.3\\lwjgl-freetype-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-glfw\\3.3.3\\lwjgl-glfw-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-glfw\\3.3.3\\lwjgl-glfw-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-glfw\\3.3.3\\lwjgl-glfw-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-glfw\\3.3.3\\lwjgl-glfw-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-jemalloc\\3.3.3\\lwjgl-jemalloc-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-jemalloc\\3.3.3\\lwjgl-jemalloc-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-jemalloc\\3.3.3\\lwjgl-jemalloc-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-jemalloc\\3.3.3\\lwjgl-jemalloc-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-openal\\3.3.3\\lwjgl-openal-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-openal\\3.3.3\\lwjgl-openal-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-openal\\3.3.3\\lwjgl-openal-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-openal\\3.3.3\\lwjgl-openal-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-opengl\\3.3.3\\lwjgl-opengl-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-opengl\\3.3.3\\lwjgl-opengl-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-opengl\\3.3.3\\lwjgl-opengl-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-opengl\\3.3.3\\lwjgl-opengl-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-stb\\3.3.3\\lwjgl-stb-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-stb\\3.3.3\\lwjgl-stb-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-stb\\3.3.3\\lwjgl-stb-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-stb\\3.3.3\\lwjgl-stb-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-tinyfd\\3.3.3\\lwjgl-tinyfd-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-tinyfd\\3.3.3\\lwjgl-tinyfd-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-tinyfd\\3.3.3\\lwjgl-tinyfd-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl-tinyfd\\3.3.3\\lwjgl-tinyfd-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\3.3.3\\lwjgl-3.3.3.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\3.3.3\\lwjgl-3.3.3-natives-windows.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\3.3.3\\lwjgl-3.3.3-natives-windows-arm64.jar;",
		pa + "\\.minecraft\\libraries\\org\\lwjgl\\lwjgl\\3.3.3\\lwjgl-3.3.3-natives-windows-x86.jar;",
		pa + "\\.minecraft\\libraries\\org\\slf4j\\slf4j-api\\2.0.17\\slf4j-api-2.0.17.jar;",
		pa + "\\.minecraft\\versions\\LiquidGalaxy-1.21.11\\LiquidGalaxy-1.21.11.jar;",
		"end_str"
	};

	for (int i = 0; cp[i] != "end_str"; i++) {
		com << cp[i];
	}
	com << ' ';
	com << "net.fabricmc.loader.impl.launch.knot.KnotClient";
	com << ' ';


	com << "--username " << config("user", 2, "nowuser");
	com << ' ';
	com << "--version LiquidGalaxy-1.21.11";
	com << ' ';
	com << "--gameDir " << path << "\\.minecraft\\versions\\LiquidGalaxy-1.21.11";
	com << ' ';
	com << "--assetsDir " << path << "\\.minecraft\\assets";
	com << ' ';
	com << "--assetIndex 29";
	com << ' ';
	com << "--uuid " << config("user", 2, "nowuuid");
	com << ' ';
	com << "--accessToken " << config("command", 2, "accessToken");
	com << ' ';
	com << "--clientId ${clientid}";
	com << ' ';
	com << "--xuid ${auth_xuid}";
	com << ' ';
	com << "--versionType G255 2.55.5";
	com << ' ';
	com << "--width " << config("command", 2, "width");
	com << ' ';
	com << "--height " << config("command", 2, "height");;


	return com;
}
int main() {
	CenterConsoleWindow();
	system("title LiquidGalaxy-1.21.11 Client");
	printlogo();
	GetCurrentDirectoryA(MAX_PATH, path);
	cout << endl;
	printColoredString("<[Preload Log]>\n", "#ee82ee");
	initialization();
	while (true) {
menu:
		printColoredString("<[Start Client]>\n", "#ee82ee");
		printHelp();
reinput:
		string input;
		printColoredString("\n\nInput > ", "#ff00ff");
		getline(cin, input);

		int inum = 0;
		if (input == "user") {
user:
			system("cls");
			printlogo();
			printColoredString("<[Switch User]>\n", "#ee82ee");
			printColoredString("[new]", "#9370db");
			printColoredString(" Add/Edit User   ", "#dda0dd");
			printColoredString("[del]", "#9370db");
			printColoredString(" Delete User   ", "#dda0dd");
			printColoredString("[sel]", "#9370db");
			printColoredString(" Select User\n", "#dda0dd");
			printColoredString("[ret]", "#9370db");
			printColoredString(" Return   ", "#dda0dd");
			printColoredString("\n<[User List]>\n", "#ee82ee");
			if (config("user", 2, "username1") == "error") {
				config("user", 1, "nowuser", "null");
				chooseuser = false;
				config("user", 1, "nowuser", "null");
				inum = 0;
			} else {
				haveuser = true;
			}
			if (config("user", 2, "nowuser") == "null" || config("user", 2, "nowuser") == "error") {
				config("user", 1, "nowuuid", "null");
				printColoredString("NowUser: Null | UUID: Null | Mode: Null\n\n", "#00bfff");
			} else {
				printColoredString("NowUser: " + config("user", 2, "nowuser") + " | UUID: " + config("user", 2, "nowuuid") + " | Mode: Offline Mode\n\n", "#00bfff");
			}
			for (int i = 1; config("user", 2, "username" + to_string(i)) != "error"; i++) {
				printColoredString("User[" + to_string(i) + "]: " + config("user", 2, "username" + to_string(i)), "#9370db");
				printColoredString(" | UUID: " + config("user", 2, "useruuid" + to_string(i)) + " | ", "#9370db");
				printColoredString("Mode: Offline Mode\n", "#dda0dd");
				inum = i;
			}
			if (!haveuser) {
				printColoredString("Null", "#9370db");
				printColoredString(" Null\n", "#dda0dd");
			}
inputuser:
			printColoredString("\n\nInput > ", "#ff00ff");
			getline(cin, input);
			if (input == "new") {
				printColoredString("Input User Name > ", "#ff00ff");
				getline(cin, input);
				inum++;
				config("user", 1, "username" + to_string(inum), input);

				printColoredString("Custom UUID?[y/n] > ", "#ff00ff");
				getline(cin, input);
				if (input == "y") {
					printColoredString("UUID > ", "#ff00ff");
					getline(cin, input);
					config("user", 1, "useruuid" + to_string(inum), input);
				} else {
					config("user", 1, "useruuid" + to_string(inum), getOfflineUUID(config("user", 2, "username" + to_string(inum))));
					printInformation(1, "UUID: " + getOfflineUUID(config("user", 2, "username" + to_string(inum))));
					setPrintColor("#b0c4de");
					system("pause");
					setPrintColor("#ffffff");
				}
				config("user", 1, "nowuser", config("user", 2, "username" + to_string(inum)));
				config("user", 1, "nowuuid", config("user", 2, "useruuid" + to_string(inum)));
				goto user;
			} else if (input == "del") {
				printColoredString("Input User [1~" + to_string(inum) + "] > ", "#ff00ff");
				getline(cin, input);
				string usernum = input;
				printColoredString("Are You Sure?[y][n] > ", "#ff00ff");
				getline(cin, input);
				if (input == "y") {
					config("user", 3, "username" + usernum);
					config("user", 3, "useruuid" + usernum);

					for (int i = stoi(usernum) + 1; config("user", 2, "username" + to_string(i)) != "error"; i++) {
						config("user", 1, "username" + to_string(i - 1), config("user", 2, "username" + to_string(i)));
						config("user", 1, "useruuid" + to_string(i - 1), config("user", 2, "useruuid" + to_string(i)));
					}
					config("user", 3, "username" + to_string(inum));
					config("user", 3, "useruuid" + to_string(inum));
					config("user", 2, "nowuser", "username" + to_string(stoi(usernum) - 1));
					config("user", 2, "useruuid", "useruuid" + to_string(stoi(usernum) - 1));
					inum--;
				} else {
					goto user;
				}
				goto user;
			} else if (input == "ret") {
				system("cls");
				printlogo();
			} else if (input == "sel") {
				if (haveuser) {
					printColoredString("\n\nChoose [1~" + to_string(inum) + "] > ", "#ff00ff");
					getline(cin, input);
					config("user", 1, "nowuser", config("user", 2, "username" + input));
					config("user", 1, "nowuuid", config("user", 2, "useruuid" + input));
				} else {
					printInformation(3, "User does not exist!");
				}
				goto user;
			} else {
				printInformation(2, "Invalid input, please enter again!");
				goto inputuser;
			}
		} else if (input == "start") {
			bool canstart = true;
			if (!haveuser) {
				printInformation(3, "User does not exist!");
				canstart = false;
			}
			if (config("user", 2, "nowuser") == "null") {
				printInformation(3, "No user selected!");
				canstart = false;
			}
			if (!canstart) {
				setPrintColor("#b0c4de");
				system("pause");
				setPrintColor("#ffffff");
				system("cls");
				printlogo();
				goto menu;
			}
			printColoredString("<[Start LBG]>\n", "#ee82ee");
			setPrintColor("#b0c4de");
			system("pause");
			setPrintColor("#ffffff");

			string run = command().str();
			cout << N;
			cout << N;
			printColoredString("<[Start Command]>\n", "#ee82ee");
			cout << N;
			setPrintColor("#add8e6");
			cout << run;
			setPrintColor("#ffffff");
			cout << N;
			string pa = path;
			cout << N;
			cout << N;
			printColoredString("<[Log]>\n", "#ee82ee");
			cout << N;
			cout << N;
			cout << N;
			comd(pa + "\\runtime\\jdk-21.0.10\\bin\\java.exe", run);
			printlogo();
			printHelp();
			goto reinput;
		} else if (input == "set") {
setting:
			system("cls");
			printlogo();
			printColoredString("<[Settings]>\n", "#ee82ee");
			printColoredString("[vie]", "#9370db");
			printColoredString(" View Startup Parameters   ", "#dda0dd");
			printColoredString("[edi]", "#9370db");
			printColoredString(" Edit Startup Settings   ", "#dda0dd");
			printColoredString("[top]", "#9370db");
			printColoredString(" Set Top Window   \n", "#dda0dd");
			printColoredString("[ret]", "#9370db");
			printColoredString(" Return   ", "#dda0dd");
inputset:
			printColoredString("\n\nInput > ", "#ff00ff");
			getline(cin, input);
			if (input == "vie") {
				string c = command().str();
				c = "\\runtime\\jdk-21.0.10\\bin\\java.exe " + c + "~";
				setPrintColor("#add8e6");
				for (int i = 0; c[i] != '~'; i++) {
					if (c[i] == ' ' || c[i] == ';') {
						cout << '\n';
					} else {
						cout << c[i];
					}
				}
				setPrintColor("#ffffff");
				setPrintColor("#b0c4de");
				cout << endl << endl;
				system("pause");
				setPrintColor("#ffffff");
				system("cls");
				printlogo();
				goto setting;
			} else if (input == "edi") {
				printColoredString("<[Edit List]>\n", "#ee82ee");
				printColoredString("[1]  --accessToken | ", "#9370db");
				printColoredString("Default value:00000000000000000000000000000000\n", "#add8e6");

				printColoredString("[2]  --width | ", "#9370db");
				printColoredString("Default value:854\n", "#add8e6");

				printColoredString("[3]  --height | ", "#9370db");
				printColoredString("Default value:480\n", "#add8e6");

				printColoredString("[4]  -Xmx | ", "#9370db");
				printColoredString("Default value:9834m\n", "#add8e6");

				printColoredString("\n\nInput [1~4]> ", "#ff00ff");
				getline(cin, input);
				string v;
				switch (stoi(input)) {
					case 1:
						printColoredString("\n\nInput [Value]> ", "#ff00ff");
						getline(cin, v);
						config("command", 1, "accessToken", v);
						break;
					case 2:
						printColoredString("\n\nInput [Value]> ", "#ff00ff");
						getline(cin, v);
						config("command", 1, "width", v);
						break;
					case 3:
						printColoredString("\n\nInput [Value]> ", "#ff00ff");
						getline(cin, v);
						config("command", 1, "height", v);
						break;
					case 4:
						printColoredString("\n\nInput [Value]> ", "#ff00ff");
						getline(cin, v);
						config("command", 1, "Xmx", v);
						break;
					default:
						printInformation(2, "Invalid input, please enter again!");
						goto setting;
				}
				printColoredString("Success!\n", "#7fffaa");
				setPrintColor("#b0c4de");
				system("pause");
				setPrintColor("#ffffff");
				goto setting;
			} else if (input == "ret") {
				system("cls");
				printlogo();
			} else if (input == "top") {
				toggleTopWindow();
				printColoredString("Success! The program takes effect after restarting!\n", "#7fffaa");
				setPrintColor("#b0c4de");
				system("pause");
				setPrintColor("#ffffff");
				goto setting;
			} else {
				printInformation(2, "Invalid input, please enter again!");
				goto inputset;
			}
		} else if (input == "exit") {
			printColoredString("Are You Sure?[y][n] > ", "#ff00ff");
			getline(cin, input);
			if (input == "y") {
				exit(0);
				/*********************************
				EXIT
				*********************************/
			} else {
				system("cls");
				printlogo();
				goto menu;
			}
		} else if (input == "mod") {
_mod_:
			system("cls");
			printlogo();
			printColoredString("<[Mods]>\n", "#ee82ee");
			printColoredString("[add]", "#9370db");
			printColoredString(" Add Mod   ", "#dda0dd");
			printColoredString("[del]", "#9370db");
			printColoredString(" Del Mod   ", "#dda0dd");
			printColoredString("[ret]", "#9370db");
			printColoredString(" Return   ", "#dda0dd");

			printColoredString("\n<[Mods List]>\n", "#ee82ee");
			mod();
modinput:
			printColoredString("\n\nInput > ", "#ff00ff");
			getline(cin, input);
			if (input == "add") {
				printColoredString("Mod Path > ", "#ff00ff");
				getline(cin, input);
				std::string filename = fs::path(input).filename().string();

				std::string targetDir = "mods/";
				std::string targetPath = targetDir + filename;

				if (!fs::exists(targetDir)) {
					fs::create_directories(targetDir);
				}

				printColoredString("Copying...\n", "#ffb90f");
				if (CopyFile(input.c_str(), targetPath.c_str(), FALSE)) {
					printColoredString("Success!\n", "#7fffaa");
					setPrintColor("#b0c4de");
					system("pause");
					setPrintColor("#ffffff");
				} else {
					DWORD err = GetLastError();
					printInformation(3, "Copy failed, Error Code: " + std::to_string(err));
					setPrintColor("#b0c4de");
					system("pause");
					setPrintColor("#ffffff");
				}
				goto _mod_;
			} else if (input == "del") {
				string n;
				printColoredString("Mod JAR Name > ", "#ff00ff");
				getline(cin, n);
				printColoredString("Are You Sure?[y][n] > ", "#ff00ff");
				getline(cin, input);
				if (input == "y") {
					n = "mods\\" + n;
					const char* file_path = n.c_str();
					if (remove(file_path) != 0) {
						DWORD err = GetLastError();
						printInformation(3, "Copy failed, Error Code: " + std::to_string(err));
						setPrintColor("#b0c4de");
						system("pause");
						setPrintColor("#ffffff");
					} else {
						printColoredString("Success!\n", "#7fffaa");
						setPrintColor("#b0c4de");
						system("pause");
						setPrintColor("#ffffff");
					}
				}
				goto _mod_;
			} else if (input == "ret") {
				system("cls");
				printlogo();
			} else {
				printInformation(2, "Invalid input, please enter again!");
				goto modinput;
			}
		} else {
			printInformation(2, "Invalid input, please enter again!");
			goto reinput;
		}
	}
}
