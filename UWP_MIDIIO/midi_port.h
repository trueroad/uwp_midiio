//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_port.h:
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

#pragma once

#include "pch.h"
#include "config.h"

#include "uwp_midiio.h"

namespace uwp_midiio
{
	template<class Derived, class MidiIO_T,
		class MidiPort_T, class IMidiPort_T>
	class uwp_midiio_port : public MidiIO_T
	{
	public:
		uwp_midiio_port() :
			port_(nullptr)
		{
		}
		virtual ~uwp_midiio_port() = 0;

		uwp_midiio_port(const uwp_midiio_port&) = delete;
		uwp_midiio_port& operator=(const uwp_midiio_port&) = delete;
		uwp_midiio_port(uwp_midiio_port&&) = delete;
		uwp_midiio_port& operator=(uwp_midiio_port&&) = delete;

		MidiIO_T* get_ptr()
		{
			return this;
		}
		static Derived* get_class(MidiIO_T* ptr)
		{
			return static_cast<Derived*>(ptr);
		}

		IMidiPort_T& port()
		{
			return port_;
		}
		void set_display_name(std::wstring_view display_name)
		{
			display_name_ = display_name;
			m_pDeviceName = display_name_.data();
		}

		virtual std::wstring find_id_from_display_name(
			std::wstring_view display_name) = 0;

		virtual void open_from_id(std::wstring_view id);

		void open_from_display_name(std::wstring_view display_name)
		{
			open_from_id(find_id_from_display_name(display_name));
		}

	private:
		IMidiPort_T port_;
		std::wstring display_name_;
	};
}
