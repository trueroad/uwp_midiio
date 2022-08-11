//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// config.h:
//   Config file
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

#pragma once

#include "pch.h"

#define UWP_MIDIIO_EXPORTS

namespace uwp_midiio
{
	using namespace std::chrono_literals;
	constexpr auto MIDI_PORT_OPEN_TIMEOUT{ 3s };

	constexpr size_t DEFAULT_MIDI_IN_QUEUE_SIZE{ 8192 };
	constexpr size_t MAX_MIDI_IN_QUEUE_SIZE{ 16384 };

	// verbose level
	// 0: none
	// 1: fatal
	// 2: error
	// 3: warn
	// 4: info
	// 5: debug
	// 6: trace

	constexpr int VERBOSE_LEVEL_DEBUG{ 6 };
	constexpr int VERBOSE_LEVEL_NDEBUG{ 3 };
}
