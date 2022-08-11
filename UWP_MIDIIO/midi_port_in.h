//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_port_in.h:
//   MIDI IN port class `uwp_midiio_port_in`
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

#pragma once

#include "pch.h"
#include "config.h"

#include "midi_port.h"
#include "uwp_midiio.h"

namespace uwp_midiio
{
	class uwp_midiio_port_in :
		public uwp_midiio_port<uwp_midiio_port_in, MIDIIn,
		winrt::Windows::Devices::Midi::MidiInPort,
		winrt::Windows::Devices::Midi::MidiInPort>
	{
	public:
		uwp_midiio_port_in() :
			message_queue_(
				std::deque<winrt::Windows::Devices::Midi::IMidiMessage>
				{DEFAULT_MIDI_IN_QUEUE_SIZE})
		{
			message_queue_.clear();
		}
		~uwp_midiio_port_in() override
		{
			if (before_token_)
				port().MessageReceived(before_token_);
		}

		std::wstring find_id_from_display_name(
			std::wstring_view display_name) override;
		void open_from_id(std::wstring_view id) override;

		void midi_in_callback(
			const winrt::Windows::Devices::Midi::MidiInPort&,
			const winrt::Windows::Devices::Midi::MidiMessageReceivedEventArgs&
			e);
		size_t pop_message(unsigned char* buff, size_t capacity);

	private:
		std::deque<winrt::Windows::Devices::Midi::IMidiMessage> message_queue_;
		winrt::event_token before_token_;
		std::mutex mtx_;
	};
}
