//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// debug_message.h:
//   Debug message macros
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

// verbose level
// 0: none
// 1: fatal
// 2: error
// 3: warn
// 4: info
// 5: debug
// 6: trace

namespace uwp_midiio
{
#ifndef NDEBUG
    constexpr int verbose_level = VERBOSE_LEVEL_DEBUG;
#else
    constexpr int verbose_level = VERBOSE_LEVEL_NDEBUG;
#endif
}

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#ifndef __PRETTYFUNC__

#if defined _MSC_VER
#define __PRETTYFUNC__ __FUNCSIG__
#define __PRETTYFUNC_W__ WIDEN(__FUNCSIG__)
#elif defined __GNUC__
#define __PRETTYFUNC__ __PRETTY_FUNCTION__
#define __PRETTYFUNC_W__ __PRETTY_FUNCTION__
#else
#define __PRETTYFUNC__ __func__
#define __PRETTYFUNC_W__ __func__
#endif

#endif // __PRETTYFUNC__

#ifdef NDEBUG
#define DEBUG_MESSAGE_A(x)
#define DEBUG_MESSAGE_W(x)
#define DEBUG_MESSAGE_STATIC_A(x)
#define DEBUG_MESSAGE_STATIC_W(x)
#define TRACE_MESSAGE_A(x)
#define TRACE_MESSAGE_W(x)
#define TRACE_MESSAGE_STATIC_A(x)
#define TRACE_MESSAGE_STATIC_W(x)
#else
#define DEBUG_MESSAGE_A(x)                                              \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 5)                               \
        {                                                               \
          std::ostringstream ss;                                        \
          ss << "debug: " << __PRETTYFUNC__ << ": " << x;               \
          ::OutputDebugStringA (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define DEBUG_MESSAGE_W(x)                                              \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 5)                               \
        {                                                               \
          std::wostringstream ss;                                       \
          ss << L"debug: " << __PRETTYFUNC_W__ << L": " << x;           \
          ::OutputDebugStringW (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define DEBUG_MESSAGE_STATIC_A(x)                                       \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 5)                               \
        ::OutputDebugStringA ((x));                                     \
    }                                                                   \
  while (false)
#define DEBUG_MESSAGE_STATIC_W(x)                                       \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 5)                               \
        ::OutputDebugStringW ((x));                                     \
    }                                                                   \
  while (false)
#define TRACE_MESSAGE_A(x)                                              \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 6)                               \
        {                                                               \
          std::ostringstream ss;                                        \
          ss << "trace: " << __PRETTYFUNC__ << ": " << x;               \
          ::OutputDebugStringA (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define TRACE_MESSAGE_W(x)                                              \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 6)                               \
        {                                                               \
          std::wostringstream ss;                                       \
          ss << L"trace: " << __PRETTYFUNC_W__ << L": " << x;           \
          ::OutputDebugStringW (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define TRACE_MESSAGE_STATIC_A(x)                                       \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 6)                               \
        ::OutputDebugStringA ((x));                                     \
    }                                                                   \
  while (false)
#define TRACE_MESSAGE_STATIC_W(x)                                       \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 6)                               \
        ::OutputDebugStringW ((x));                                     \
    }                                                                   \
  while (false)
#endif

#define WARNING_MESSAGE_A(x)                                            \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 3)                               \
        {                                                               \
          std::ostringstream ss;                                        \
          ss << "warning: " << __PRETTYFUNC__ << ": " << x;             \
          ::OutputDebugStringA (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define WARNING_MESSAGE_W(x)                                            \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 3)                               \
        {                                                               \
          std::wostringstream ss;                                       \
          ss << L"warning: " << __PRETTYFUNC_W__ << L": " << x;         \
          ::OutputDebugStringW (ss.str ().c_str ());                    \
        }                                                               \
    }                                                                   \
  while (false)
#define WARNING_MESSAGE_STATIC_A(x)                                     \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 3)                               \
        ::OutputDebugStringA ((x));                                     \
    }                                                                   \
  while (false)
#define WARNING_MESSAGE_STATIC_W(x)                                     \
  do                                                                    \
    {                                                                   \
      if (uwp_midiio::verbose_level >= 3)                               \
        ::OutputDebugStringW ((x));                                     \
    }                                                                   \
  while (false)
