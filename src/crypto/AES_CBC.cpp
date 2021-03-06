/* Written in 2015 by Alexander Shishenko <alex@shishenko.com>
 *
 * LVCrypto - Cryptographic wrapper, used in Librevault.
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#include <librevault/crypto/AES_CBC.h>
#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

namespace librevault {
namespace crypto {

AES_CBC::AES_CBC(const blob& key, const blob& iv, bool padding) : key(key), iv(iv), padding(padding) {}

blob AES_CBC::encrypt(const blob& plaintext) const {
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption filter(key.data(), key.size(), iv.data());

	std::string ciphertext;
	CryptoPP::StringSource(plaintext.data(), plaintext.size(), true,
						   new CryptoPP::StreamTransformationFilter(filter,
																	new CryptoPP::StringSink(ciphertext),
																	padding ? CryptoPP::StreamTransformationFilter::PKCS_PADDING : CryptoPP::StreamTransformationFilter::NO_PADDING
						   )
	);

	return blob(std::make_move_iterator(ciphertext.begin()), std::make_move_iterator(ciphertext.end()));
}

blob AES_CBC::decrypt(const blob& ciphertext) const {
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption filter(key.data(), key.size(), iv.data());

	std::string plaintext;
	CryptoPP::StringSource(ciphertext.data(), ciphertext.size(), true,
						   new CryptoPP::StreamTransformationFilter(filter,
																	new CryptoPP::StringSink(plaintext),
																	padding ? CryptoPP::StreamTransformationFilter::PKCS_PADDING : CryptoPP::StreamTransformationFilter::NO_PADDING
						   )
	);

	return blob(std::make_move_iterator(plaintext.begin()), std::make_move_iterator(plaintext.end()));
}

} /* namespace crypto */
} /* namespace librevault */
