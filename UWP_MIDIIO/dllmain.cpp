//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// dllmain.cpp:
//   DLL main entry point
//
// Copyright (C) 2022 Masamichi Hosoda.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include "pch.h"
#include "config.h"

#include "debug_message.h"

#if !defined(NDEBUG) && defined(_MSC_VER)
// Workaround for avoiding false positive `Detected memory leaks!`
__declspec(dllimport) HINSTANCE __stdcall AfxGetInstanceHandle();
#pragma comment(lib, "mfc140ud.lib")
#endif

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DEBUG_MESSAGE_STATIC_W(L"DllMain DLL_PROCESS_ATTACH\n");
		winrt::init_apartment();
		break;
	case DLL_THREAD_ATTACH:
		DEBUG_MESSAGE_STATIC_W(L"DllMain DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
		DEBUG_MESSAGE_STATIC_W(L"DllMain DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
		DEBUG_MESSAGE_STATIC_W(L"DllMain DLL_PROCESS_DETACH\n");
#if !defined(NDEBUG) && defined(_MSC_VER)
		// Workaround for avoiding false positive `Detected memory leaks!`
		AfxGetInstanceHandle();
#endif
		break;
	default:
		WARNING_MESSAGE_STATIC_W(L"DllMain unknown\n");
		break;
	}
	return TRUE;
}
