#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <chrono>
#include <thread>

// =============================================================
//  View : コンソール画面そのものの操作（消去・待ち時間）
// =============================================================
namespace ConsoleView
{
	inline void Clear()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		if (h == INVALID_HANDLE_VALUE) return;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(h, &csbi)) return;

		DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
		COORD homeCoords = { 0, 0 };
		DWORD count;

		FillConsoleOutputCharacterA(h, ' ', cellCount, homeCoords, &count);
		FillConsoleOutputAttribute(h, csbi.wAttributes, cellCount, homeCoords, &count);
		SetConsoleCursorPosition(h, homeCoords);
	}

	// 演出用の待ち時間
	inline void Pause(int ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
}
