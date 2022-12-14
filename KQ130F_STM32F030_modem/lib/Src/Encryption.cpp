/*
 * Encryption.cpp
 *
 *  Created on: Jul 29, 2022
 *      Author: lidoma
 */

#include <Encryption.hpp>


Enc::RSA::RSA() {}
Enc::RSA::RSA(RSAKey_t& pub, RSAKey_t& priv) {}
void Enc::RSA::encrypt(const uint8_t* plain, const size_t pLen,
									uint8_t* enc, size_t& eLen) {
	enc[0] = 'F';
	enc[1] = 'u';
	enc[2] = 'c';
	enc[3] = 'k';
	enc[4] = 'e';
	enc[5] = 'd';
	enc[6] = ' ';
	enc[7] = 'M';
	enc[8] = 'S';
	enc[9] = 'G';
	enc[10] = '\0';
	eLen = 11;
}
void Enc::RSA::decrypt(const uint8_t* enc, const size_t eLen,
									uint8_t* plain, size_t& pLen) {
	memcpy(plain, enc, eLen);
}

Enc::Encryption::Encryption() {}
Enc::Encryption::~Encryption() {}
void Enc::Encryption::encrypt(const uint8_t* plain, const size_t pLen,
									uint8_t* enc, size_t& eLen) {}
void Enc::Encryption::decrypt(const uint8_t* enc, const size_t eLen,
									uint8_t* plain, size_t& pLen) {}

