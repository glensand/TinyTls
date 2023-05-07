# TinyTls
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![GitHub last commit](https://img.shields.io/github/last-commit/glensand/TinyTls?color=red&style=plastic)
![GitHub stars](https://img.shields.io/github/stars/glensand/TinyTls?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/glensand/TinyTls?style=social)
![GitHub followers](https://img.shields.io/github/followers/glensand?style=social)
## A few words about
This repository contains an implementation of the TLS Stream Plugin for Unreal Engine. The TLS Stream Plugin is a secure communication protocol that is used to encrypt data transmission between two endpoints over the internet. This plugin provides developers with a simple and efficient way to add secure communication capabilities to their Unreal Engine applications

The plugin is designed to work with Unreal Engine 4.20 - 4.27.2, 5.0 - 5.2 and provides a wrapper around the OpenSSL library, which is widely used for implementing secure communication protocols. The plugin supports both the TLS 1.2 and 1.3 protocols, which are the latest and most secure versions of the TLS protocol.

The implementation of the plugin provides a simple API for establishing secure connections between endpoints, sending and receiving encrypted data, and managing TLS session state. 

The plugin is open source and can be used for both commercial and non-commercial projects under the MIT license. The repository contains the complete source code for the plugin, along with sample applications and documentation on how to use the plugin in your own projects.
## Example
```c++
#include "TlsStream.h" // the only include you need

void Example1()
{
	auto* Stream = TinyTls::CreateTlsStream(); // Obttain the stream implementation
	Stream->Connect("127.0.0.1", "6969");

	const FString Message = "Disaster is Here";
	
	Stream->Send( Message.Len()); // chars count first
	Stream->Send(GetData(Message), Message.GetCharArray().Num()); // And the data
	
	TCHAR Buffer[1024];
	const auto Length = Stream->Receive<int32_t>();
	Stream->Receive(Buffer, Length);

	// Do some stuff
	
	Stream->Disconnect();
}

```

Good luck!;D
