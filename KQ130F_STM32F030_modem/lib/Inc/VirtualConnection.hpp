#pragma once

#include "Encryption.hpp"
#include "MessageHandler.hpp"


// master first should start a connection (rx net id + enc method + timeout)
class VirtualConnection {
private:
	uint32_t _rxTimeOutMics;
	uint8_t _maxRetries;

	uint8_t _txEncBuf[PACKET_MAX_SIZE];
	uint8_t _txSerBuf[PACKET_MAX_SIZE];
	uint8_t _rxDesBuf[PACKET_MAX_SIZE];

	MessageHandle _msgHndls[NUM_MESSAGES(N)];

	Enc::Encryption _enc;
	rxFunc _rxCallback;

	typedef enum {
		atInit,
		atHandshake,
		atTransferReady,
		atDestroyed
	} Connectionstate;

	Connectionstate state;

	static size_t _timePassed(uint32_t _rxOutMicSec) {
		size_t curTick = osKernelSysTick();
		if (curTick > _rxOutMicSec)
			return curTick - _rxOutMicSec;
		else
			return (UINT32_MAX - _rxOutMicSec) + curTick; // Other impls may cause overflow
	}

};
