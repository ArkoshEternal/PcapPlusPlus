#pragma once

#include "Layer.h"

/// @file

/// @namespace pcpp
/// @brief The main namespace for the PcapPlusPlus lib
namespace pcpp
{
	/// @class PayloadLayer
	/// Represents a generic or unknown layer or a packet payload
	class PayloadLayer : public Layer
	{
	public:
		/// A constructor that creates the layer from an existing packet raw data
		/// @param[in] data A pointer to the raw data
		/// @param[in] dataLen Size of the data in bytes
		/// @param[in] prevLayer A pointer to the previous layer
		/// @param[in] packet A pointer to the Packet instance where layer will be stored in
		PayloadLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet)
		    : Layer(data, dataLen, prevLayer, packet, GenericPayload)
		{}

		/// A constructor that allocates a new payload
		/// @param[in] data A raw buffer that will be used as a payload. This data will be copied to the layer
		/// @param[in] dataLen The raw buffer length
		PayloadLayer(const uint8_t* data, size_t dataLen);

		/// A constructor that allocates a new payload from an hex stream
		/// @param[in] payloadAsHexStream A string that represents an hex stream of the payload. For example:
		/// 0001080006040002842b2b774c56c0a80078000000000000c0a8. In order for the hex stream to be valid it has to
		/// contain valid hex chars only (which means, for example, that it can't begin with "0x") and it also has to
		/// have an even number of chars (each char represents one nibble). If the string is not a valid hex stream an
		/// error will be printed to log and the payload layer will be empty (no data)
		explicit PayloadLayer(const std::string& payloadAsHexStream);

		~PayloadLayer() override = default;

		/// Get a pointer to the payload data
		/// @return A pointer to the payload data
		uint8_t* getPayload() const
		{
			return m_Data;
		}

		/// Get the payload data length
		/// @return The payload data length in bytes
		size_t getPayloadLen() const
		{
			return m_DataLen;
		}

		// implement abstract methods

		/// Does nothing for this layer (PayloadLayer is always last)
		void parseNextLayer() override
		{}

		/// @return Payload data length in bytes
		size_t getHeaderLen() const override
		{
			return m_DataLen;
		}

		/// Does nothing for this layer
		void computeCalculateFields() override
		{}

		/// Sets the payload of the PayloadLayer to the given pointer. This will resize (extend/shorten) the underlying
		/// packet respectively if there is one.
		/// @param[in] newPayload New payload that shall be set
		/// @param[in] newPayloadLength New length of payload
		void setPayload(const uint8_t* newPayload, size_t newPayloadLength);

		std::string toString() const override;

		OsiModelLayer getOsiModelLayer() const override
		{
			return OsiModelApplicationLayer;
		}

		/// A static method that takes a byte array and detects if it can be parsed as a PayloadLayer.
		/// @param[in] data A byte array
		/// @param[in] dataLen The byte array size (in bytes)
		/// @return True if the data is non-nullptr and dataLen is greater than 0, or data is nullptr and dataLen is 0.
		static bool isDataValid(const uint8_t* data, size_t dataLen)
		{
			// PayloadLayer is special as it can be empty. So, it's valid if data is nullptr and dataLen is 0.
			return (data == nullptr) != (dataLen != 0);  // XOR
		};
	};
}  // namespace pcpp
