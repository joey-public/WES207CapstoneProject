#ifndef PACKETUTILS_H
#define PACKETUTILS_H

#include <vector>
#include <cstdint>
#include <packet.h>

class PacketUtils 
{
public:
    static const std::size_t HEADER_PACKET_SIZE;
    static const std::size_t DATA_PACKET_FIXED_SIZE;
    static std::size_t getReceiverIdOffset();
    static std::size_t getGpsDataOffsetLatitude();
    static std::size_t getGpsDataOffsetLongitude();
    static std::size_t getGpsDataOffsetAltitude();
    static std::size_t getPeakTimestampOffset();
    static std::size_t getWaveformSamplesOffset();
    static std::size_t getTimestampsOffset();

    static void fillHeaderPacket(HeaderPacket& header, uint32_t packetId, uint64_t timestamp, uint8_t packetType, uint16_t packetLength);
    static void createHeaderPacket(const HeaderPacket& header, std::vector<char>& packet);
    static void createDataPacket(const DataPacket& data, std::vector<char>& packet);
    static const HeaderPacket& parseHeaderPacket(const std::vector<char>& packet);
    static void parseDataPacket(const std::vector<char>& packet, DataPacket& data);
};

#endif//PACKETUTILS_H