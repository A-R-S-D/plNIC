/*
 * PLModem.h
 *
 *  Created on: Jul 28, 2022
 *      Author: lidoma
 */

#ifndef SRC_PLMODEM_H_
#define SRC_PLMODEM_H_


#include <stddef.h>
#include <stdint.h>
#include <functional>
#include <string.h>

#include "stm32f0xx_hal.h"

#include "Encryption.hpp"

#define DBG(msg) HAL_UART_Transmit(_dbugbCom, (uint8_t*)#msg, sizeof(#msg), 200);

// stop & wait ARQ
// DMA
// RX & TX Interrupt
// CRC Check
// AES128
namespace PLM {
	typedef enum {
		NONE = 0,
		RSA
	} EncMode;

	typedef enum {
		OK,
		SpaceNotAvailable,
		MaxRetriesExceeded,

	} NICStatus;

	typedef std::function<void(uint8_t* frame, const uint8_t len)> rxFunc;

	template <size_t N>
	class PLModem {
	private:
		/* Asynchronous Character Based transmission (Byte Stuffing) TODO HEADER and TAIL should be in ALL
		 * Master/Modem packets
		 *  handshake
		 *  	header(rxNetID|txNetID, packetLen MSGFLAGS) | NetID(1-254) => may yield error or automatic, encMode, keys(pub, prv) or automatic
		 *  data transmission
		 * 		header(rxNetID|txNetID, packetLen, MSGFLAGS) | (data) | tail(padding) // should be PACKET_MAX_SIZE bytes
		 *
		 * Modem/Modem packets
		 * 	handshake
		 * 		header(rxNetID|txNetID, packetLen, MSGFLAGS) | public key |
		 * 	data transmission
		 * 		header(rxNetID|txNetID, packetLen, MSGFLAGS) | (len, data) | tail(CRC)	 // should be atleast 3 bytes and max of PACKET_MAX_SIZE bytes
		 */


		uint8_t _netId;
//		uint8_t _waitBuf[BUFFER_LEN(N)]; // Waiting space for tx and rx messages
//		uint8_t _rxEncBuf[PACKET_MAX_SIZE];
//
//		osThreadId _handleId;
//
//		bool _isTerminate;
//
//		UART_HandleTypeDef* _modemCom;
//		UART_HandleTypeDef* _dbugbCom;
//
//		int _findOccupyEmptyMessage() {
//			for (uint8_t i = 0; i < NUM_MESSAGES(N); i++)
//				if (not(_msgHndls[i].isOccupied)) {
//					_msgHndls[i].isOccupied = true;
//					return i;
//				}
//			return -1;
//		}
//		void _emptyOccupiedMessage(uint8_t index) {
//			_msgHndls[index].flags = 0; // now is Simpler than &= ~(MessageHandler::EMP_OCP_MASK);
//		}
//		void _generateTXWaitingMsg(MessageHandler& msghd, const uint8_t netId, const uint8_t* dat, const uint8_t len) {
//			msghd.TxRx = 1;
//			msghd.isOccupied = 1;
//			msghd.rxTick = UINT32_MAX;
//
//			Message& msg = msghd.msg;
//			msg.netId = netId;
//			msg.datLen = len;
//			msg.flags |= DAT_MASK;
//			memcpy(_waitBuf+(msg.datStart), dat, len);
//		}
//		bool _receivePacketRaw(uint8_t* src, Message& msg) {
//			return true;
//		}
//		void _serializeMessage(uint8_t* dest, size_t& len, Message msg) {
////			_generateCRC();
//		}
//		void _generateCRC(uint8_t* crc, const uint8_t* dat, const size_t len) {
//		}
//		bool _checkCRC(const uint8_t* crc, const uint8_t* dat, const size_t len) {
//			return true;
//		}
//		bool _send(MessageHandler& msgh) { // This is the owner of _txEncBuf
//			//			osKernelSysTickMicroSec(100000);
//			size_t serLen = 0;
//			_serializeMessage(_txSerBuf, serLen, msgh.msg); // NOTE: it starts from MSGTYPE and netid and len are in plain text
//
//			size_t encLen = 0;
//			_enc.encrypt(_txSerBuf, serLen, _txEncBuf+NETID_FEILD_LEN+MSGLEN_FEILD_LEN, encLen);
//
//			memcpy(_txEncBuf, (uint8_t*)&(msgh.msg.netId), NETID_FEILD_LEN);
//			uint8_t encLenInMsgFieldSize = (uint8_t) encLen;
//			memcpy(_txEncBuf+NETID_FEILD_LEN, (uint8_t*)&encLenInMsgFieldSize, MSGLEN_FEILD_LEN);
//			HAL_StatusTypeDef res = HAL_UART_Transmit(_modemCom, (uint8_t*)_txEncBuf,
//											encLen+NETID_FEILD_LEN+MSGLEN_FEILD_LEN, 1000);
//			if (res != HAL_OK)
//				return false;
//
//			msgh.numRetries += 1;
//			msgh.rxTick = osKernelSysTick();
//
//			return true;
//		}
//
//	public:
//		PLModem(UART_HandleTypeDef* hu, UART_HandleTypeDef* dbg, rxFunc  func) :
//			_isTerminate(false), _rxCallback(func), _modemCom(hu), _dbugbCom(dbg) {
//			for (uint8_t i = 0; i < NUM_MESSAGES(N); i++)
//				_msgHndls[i].msg.datStart = PACKET_MAX_SIZE*i;
//		}
//
//		bool begin(uint8_t netId, Enc::Encryption encMethod, uint8_t maxRetries, uint32_t rxOut) {
//			this->_netId = netId;
//			this->_maxRetries = maxRetries;
//			this->_rxOutMicSec = rxOut;
//			this->_enc = encMethod;
//
//			HAL_UART_Receive_IT(_modemCom, _rxEncBuf, 1);
//
//			return true;
//		}
//
//		// No need for separate MSGTYPE field?
//		bool send(const uint8_t netid, const uint8_t* buf, const size_t len) {
//			if (len > PAYLOAD_MAX_SIZE) {
////				_handleFragmentation();
//			}
//			int emptyMsgIdx = _findOccupyEmptyMessage();
//			if (emptyMsgIdx == -1)
//				return NICStatus::SpaceNotAvailable;
//			_generateTXWaitingMsg(_msgHndls[emptyMsgIdx], netid, buf, len);
//			osSignalSet(_handleId, 10); // Only one thread should be able to handle UART or otherwise there should be mutex
//			return true;
//		}
//
//		// This function handle receive from other devices and checks if it is handshake or data receive or auto id protocol
//		void handle(const void* args) {
//			_handleId = osThreadGetId();
//			bool lastTxAckReceived = true;
//			uint8_t currTXCheckIdx = 0; // Checks msgs in the buffer and if it was tx then if retry == 0 sends it, else
//										// checks timeout and if it has exceeded then retransmits the packet and retry += 1
//			while (not _isTerminate) {
////				osEvent ev = osSignalWait(10, 0);
////				if (ev.status != osEventSignal || ev.value.signals != 10)
////					continue;
//				osThreadYield();
//
//				// Check for the sent packet
//				if (_timePassed(_msgHndls[currTXCheckIdx].rxTick) > _rxOutMicSec) {
//					if (_msgHndls[currTXCheckIdx].numRetries > _maxRetries) {
//						DBG(Packet_fucked_up_discarding_it);
//						_msgHndls[currTXCheckIdx].isOccupied = false;
//						lastTxAckReceived = true;
//					}
//					else
//					_send(_msgHndls[currTXCheckIdx]);
//				}
//
//
//				for (uint8_t i = 0; i < NUM_MESSAGES(N); i++) {
//					if (not _msgHndls[i].isOccupied)
//						continue;
//
//					if (lastTxAckReceived & currTXCheckIdx > i & _msgHndls[i].TxRx) { // Valid TX packet
//						currTXCheckIdx = i; // Check if next packet is TX (MAYBE RX IF THAT THEN currTXCheckIdx++)
//
//						if (currTXCheckIdx = i & ) {
//
//
//						} else {
//
//						}
//					} else { // Valid RX packet
//						lastTxAckReceived
//					}
//
//
//					// Then we should wait for acknowledgment
//
//
//				}
//				if (_handleId) {
//
//				}
//
//				if (HAL_UART_Receive(_modemCom, _waitBuf, 5, 200) == HAL_StatusTypeDef::HAL_OK)
//					this->_rxCallback(_waitBuf, 5);
//				HAL_UART_Transmit(_dbugbCom, _waitBuf, 5, 200);
//
//				osThreadYield();
//			}
//
//			osThreadTerminate(_handleId);
//		}
//
//		void rxInterrupt(UART_HandleTypeDef* huart) {
//			if (huart == _modemCom) {
//				// Try to parse the packet and copy it into _msgHndlrs, then call the signal function
//				osSignalSet(_handleId, 10);
//			}
//		}
//
	};

}



#endif /* SRC_PLMODEM_H_ */
