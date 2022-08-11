//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_port.cpp:
//   MIDI IN/OUT port base class `uwp_midiio_port`
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

// See
// https://gist.github.com/trueroad/9c5317af5f212b2de7c7012e76b9e66b

#include "pch.h"
#include "config.h"

#include "midi_port.h"

#include "debug_message.h"
#include "midi_port_in.h"
#include "midi_port_out.h"
#include "uwp_midiio.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Midi;

namespace uwp_midiio
{
	template class uwp_midiio_port<uwp_midiio_port_in, MIDIIn,
		MidiInPort, MidiInPort>;
	template class uwp_midiio_port<uwp_midiio_port_out, MIDIOut,
		MidiOutPort, IMidiOutPort>;

	template<class Derived, class MidiIO_T,
		class MidiPort_T, class IMidiPort_T>
	uwp_midiio_port<Derived, MidiIO_T, MidiPort_T, IMidiPort_T
		>::~uwp_midiio_port()
	{
	}

	template<class Derived, class MidiIO_T,
		class MidiPort_T, class IMidiPort_T>
	void uwp_midiio_port<Derived, MidiIO_T, MidiPort_T, IMidiPort_T
		>::open_from_id(std::wstring_view id)
	{
		DEBUG_MESSAGE_W(L"enter \"" << id << L"\"\n");

		if (port())
		{
			port().Close();
			port() = nullptr;
		}

		DEBUG_MESSAGE_W(L"  trying FromIdAsync\n");
		try
		{
			auto async = MidiPort_T::FromIdAsync(id);

			DEBUG_MESSAGE_W(L"  trying wait_for\n");
			if (async.wait_for(MIDI_PORT_OPEN_TIMEOUT) ==
				AsyncStatus::Completed)
			{
				port() = async.GetResults();
			}
		}
		catch (winrt::hresult_error const& ex)
		{
			WARNING_MESSAGE_W(L"exception 0x"
				<< std::hex << ex.code()
				<< L", "
				<< static_cast<std::wstring_view>(ex.message())
				<< L"\n");

			return;
		}

		DEBUG_MESSAGE_W(L"returns\n");
		return;
	}
}
