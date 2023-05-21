#include "PacketUtils.h"
#include <cstring>
#include <cstddef>
#include <iostream>
#include <sstream>

const std::size_t PacketUtils::HEADER_PACKET_SIZE = sizeof(HeaderPacket);
const std::size_t PacketUtils::DATA_PACKET_FIXED_SIZE = sizeof(DataPacket) - sizeof(std::vector<uint64_t>) - sizeof(std::vector<std::complex<short>>);

std::size_t PacketUtils::getReceiverIdOffset()
{
    return offsetof(DataPacket, rx_id);
}

std::size_t PacketUtils::getGpsDataOffsetLatitude()
{
    return offsetof(DataPacket, latitude);
}

std::size_t PacketUtils::getGpsDataOffsetLongitude()
{
    return offsetof(DataPacket, longitude);
}

std::size_t PacketUtils::getGpsDataOffsetAltitude()
{
    return offsetof(DataPacket, altitude);
}

std::size_t PacketUtils::getPeakTimestampOffset()
{
    return offsetof(DataPacket, peak_timestamps);
}

std::size_t PacketUtils::getWaveformSamplesOffset()
{
    return offsetof(DataPacket, waveformSamples);
}

void PacketUtils::fillHeaderPacket(HeaderPacket& header, uint32_t packetId, uint64_t timestamp, uint8_t packetType, uint16_t packetLength)
{
    header.packet_id     = packetId;
    header.pkt_ts        = timestamp;
    header.packet_type   = packetType;
    header.packet_length = packetLength;
}

void PacketUtils::createHeaderPacket(const HeaderPacket& header, std::vector<char>& packet)
{
    // Copy the header structure into the packet buffer
     std::memcpy(packet.data(), &header, sizeof(HeaderPacket));
}

void PacketUtils::createDataPacket(const DataPacket& data, std::vector<char>& packet)
{
    // Copy the data structure into the packet buffer
    std::memcpy(packet.data(), &data, PacketUtils::DATA_PACKET_FIXED_SIZE);

    // Calculate the offset for peak timestamps
    std::size_t timestampsOffset = PacketUtils::DATA_PACKET_FIXED_SIZE;

    // Copy peak timestamps to the packet buffer
    std::memcpy(packet.data() + timestampsOffset, data.peak_timestamps.data(), data.peak_timestamps.size() * sizeof(double));

    // Calculate the offset for waveform samples
    std::size_t waveformOffset = timestampsOffset + data.peak_timestamps.size() * sizeof(uint64_t);

    // Copy waveform samples to the packet buffer
    std::memcpy(packet.data() + waveformOffset, data.waveformSamples.data(), data.waveformSamples.size() * sizeof(std::complex<short>));
}


const HeaderPacket& PacketUtils::parseHeaderPacket(const std::vector<char>& packet)
{
    if (packet.size() != sizeof(HeaderPacket)) 
    {
        throw std::runtime_error("Invalid header packet size");
    }

    return *reinterpret_cast<const HeaderPacket*>(packet.data());
}

void PacketUtils::parseDataPacket(const std::vector<char>& packet, DataPacket& data)
{
    if (packet.size() < DATA_PACKET_FIXED_SIZE) 
    {
        //throw std::runtime_error("Invalid data packet size");
    }

    //std::cout << "Data Packet Size = " << DATA_PACKET_FIXED_SIZE;
    std::memcpy(&data, packet.data(), DATA_PACKET_FIXED_SIZE);

    size_t num_peaks = data.numTimeSamples;

    uint64_t timesampleSize = num_peaks * sizeof(double);
    uint64_t waveformSamplesSize = packet.size() - DATA_PACKET_FIXED_SIZE - timesampleSize;
    size_t   waveformSamplesCount = waveformSamplesSize / sizeof(std::complex<short>);
    //std::cout << "time sample Size " << timesampleSize;
    //std::cout << "waveformSamplesSize " << waveformSamplesSize;
    data.peak_timestamps.resize(num_peaks);
    data.waveformSamples.resize(waveformSamplesCount);

    const char* packetDataPtr = packet.data() + DATA_PACKET_FIXED_SIZE;

    std::memcpy(data.peak_timestamps.data(), packetDataPtr, timesampleSize);
    packetDataPtr += timesampleSize;
    
    std::memcpy(data.waveformSamples.data(), packetDataPtr, waveformSamplesSize);
}

void PacketUtils::createControlPacket(const ControlMessage& message, std::vector<char>& packet)
{
    std::string serialized_message;
    serialized_message = serializeControlMessage(message);
    packet = std::vector<char>(serialized_message.begin(), serialized_message.end());

}

void PacketUtils::parseControlPacket(const std::vector<char>& packet, ControlMessage& message)
{
    const std::string serializedString =  std::string(packet.begin(), packet.end());
    deserializeControlMessage(serializedString, message);

}

std::string PacketUtils::serializeControlMessage(const ControlMessage& message)
{
    std::ostringstream oss;
    oss << message.rx_id << "$" << message.ack << "$";
    for (const auto& msg : message.message)
    {
        oss << msg.size() << ":" << msg << "$";
    }
    return oss.str();
}

void PacketUtils::deserializeControlMessage(const std::string& serializedMessage, ControlMessage& message)
{
    std::istringstream iss(serializedMessage);
    std::string rxIdStr, ackStr;
    std::getline(iss, rxIdStr, '$');
    std::getline(iss, ackStr, '$');
    message.rx_id = std::stoul(rxIdStr);
    message.ack = std::stoul(ackStr);

    message.message.clear();
    std::string msg;
    while (std::getline(iss, msg, ':'))
    {
        std::size_t size = std::stoul(msg);
        std::getline(iss, msg, '$');
        std::cout << "message: " << msg << std::endl;
        // Check if there is a newline character at the end of the message and remove it
        if (!msg.empty() && msg.back() == '\n')
        {
            msg.pop_back();
        }

        message.message.emplace_back(msg.substr(0, size));
    }
}