<!-- -*- coding: utf-8 -*- -->
# UWP MIDIIO Library (DLL)

[ [Japanese (日本語)](./README.ja.md) / English ]

https://github.com/trueroad/uwp_midiio

**UWP MIDIIO Library (DLL)** that enables
using BLE MIDI (Bluetooth MIDI) devices for
[MIDI sequencer / editor software "Sekaiju"
](https://openmidiproject.osdn.jp/Sekaiju_en.html).

## Introduction

[Sekaiju
](https://openmidiproject.osdn.jp/Sekaiju_en.html)
is an open-source MIDI sequencer and MIDI editor software
that runs on Windows.
You can use it to connect musical instruments via MIDI ports
that Windows recognizes, so many people use it with a USB MIDI connection.
Windows 10 has supported BLE MIDI (Bluetooth MIDI) since around 2016.
Unfortunately, Sekaiju version 7.5 (the latest version as of August 2022)
can handle USB MIDI, but not BLE MIDI.

Sekaiju uses
[MIDIIO Library
](https://openmidiproject.osdn.jp/MIDIIOLibrary_en.html)
to handle MIDI input and output.
MIDIIO Library uses a set of APIs called MME (Multi Media Extension)
to provide MIDI input and output, but the MME APIs cannot handle BLE MIDI.
Sets of APIs for handling MIDI on Windows 10
include the UWP (Universal Windows Platform) APIs,
which were implemented later than MME, and BLE MIDI is only supported by UWP.

Sekaiju is open-source software, and MIDIIO Library
is also open-source software.
So, **UWP MIDIIO Library (DLL)** (this library)
that uses UWP is an attempt to enable using BLE MIDI devices
on Sekaiju by replacing MIDIIO Library that uses MME.

Of course, not only BLE MIDI is supported, but also conventional USB MIDI, etc.
In addition, multi-client support has been enhanced as a benefit of UWP.

This library is of course also open-source software and non-guaranteed.
If any problems arise, please resolve them yourself.

This library is developed independently of Sekaiju and Open MIDI Project members.
Therefore, please do not ask the following questions to the Open MIDI Project.
E.g., about this library, operation or usage of Sekaiju in the environment in which this library is installed, etc.

## Build

Build the release version of this library using Visual Studio Community 2019,
you get `MIDIIO.dll`.

## Install

Back up the original `MIDIIO.dll` in Sekaiju's folder and then replace it.

## Usage

Before invoking Sekaiju, do pairing BLE MIDI device by the Windows settings.

## Licence

Copyright (C) 2022 Masamichi Hosoda. All rights reserved.

License: BSD-2-Clause

See [LICENSE](./LICENSE).
