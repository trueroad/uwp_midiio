//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_ports.h:
//   MIDI ports open/close class `uwp_midiio_ports`
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

#include "midi_port_in.h"
#include "midi_port_out.h"
#include "uwp_midiio.h"

namespace uwp_midiio
{
	class uwp_midiio_ports final
	{
	public:
		uwp_midiio_ports() = delete;
		~uwp_midiio_ports() = delete;
		uwp_midiio_ports(const uwp_midiio_ports&) = delete;
		uwp_midiio_ports& operator=(const uwp_midiio_ports&) = delete;
		uwp_midiio_ports(uwp_midiio_ports&&) = delete;
		uwp_midiio_ports& operator=(uwp_midiio_ports&&) = delete;

		static MIDIIn* open_in(std::wstring_view display_name)
		{
			return open<uwp_midiio_port_in, MIDIIn>(
				display_name, ports_in_, mtx_in_);
		}
		static MIDIOut* open_out(std::wstring_view display_name)
		{
			return open<uwp_midiio_port_out, MIDIOut>(
				display_name, ports_out_, mtx_out_);
		}
		static bool close_in(MIDIIn* ptr)
		{
			return close<uwp_midiio_port_in, MIDIIn>(
				ptr, ports_in_, mtx_in_);
		}
		static bool close_out(MIDIOut* ptr)
		{
			return close<uwp_midiio_port_out, MIDIOut>(
				ptr, ports_out_, mtx_out_);
		}

	private:
		template <class uwp_midiio_port_T, class MidiIO_T>
		static MidiIO_T* open(std::wstring_view display_name,
			std::vector<std::unique_ptr<uwp_midiio_port_T>>& ports,
			std::mutex& mtx);

		template <class uwp_midiio_port_T, class MidiIO_T>
		static bool close(MidiIO_T* ptr,
			std::vector<std::unique_ptr<uwp_midiio_port_T>>& ports,
			std::mutex& mtx);

		static std::vector<std::unique_ptr<uwp_midiio_port_in>> ports_in_;
		static std::vector<std::unique_ptr<uwp_midiio_port_out>> ports_out_;

		static std::mutex mtx_in_;
		static std::mutex mtx_out_;
	};
}
