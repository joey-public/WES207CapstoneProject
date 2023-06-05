#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include "PacketUtils.h"

int main() 
{
    // Create a sample header packet
    HeaderPacket header;
    header.packet_id = 1;
    header.pkt_ts = 1234567890;
    header.packet_type = PACKET_TYPE_CONTROL_MESSAGE_RESPONSE;
    

    // Create a sample data packet
    DataPacket data;
    data.rx_id = 1;
    std::vector<double> ts = {1,2,3,4,5};
    data.peak_timestamps = std::move(ts);
    data.numTimeSamples = data.peak_timestamps.size();

    data.latitude  = -10.2;
    data.longitude = -11.2;
    data.altitude  =  10;
    std::vector<std::complex<short>> wv = {1,2,3,4,5};
    data.waveformSamples = std::move(wv);    

    std::vector<uint64_t> p_sid = {1234,23444554,23,23,4};
    data.peak_ts_idx = std::move(p_sid);
    
    header.packet_length = PacketUtils::DATA_PACKET_FIXED_SIZE+data.peak_timestamps.size()*sizeof(double)
    +data.waveformSamples.size()*sizeof(std::complex<int16_t>) + data.peak_ts_idx.size()*sizeof(uint64_t);

    size_t header_size = PacketUtils::HEADER_PACKET_SIZE;

    //std::cout << "Parsed Header Packet size:" << header_size << std::endl;
    // Create a packet buffer to store the header packet
    std::vector<char> headerPacketBuffer(header_size);
    PacketUtils::createHeaderPacket(header, headerPacketBuffer);

    // Create a packet buffer to store the data packet
    std::size_t packetSize = PacketUtils::DATA_PACKET_FIXED_SIZE 
        + data.peak_timestamps.size() * sizeof(double) 
        + data.waveformSamples.size() * sizeof(std::complex<int16_t>)
        + data.peak_ts_idx.size() * sizeof(uint64_t);
    std:: cout << "Packet size: " << packetSize << std::endl;
    std::vector<char> dataPacketBuffer(packetSize);
    PacketUtils::createDataPacket(data, dataPacketBuffer);

    // Parse the header packet
    HeaderPacket parsedHeader;
    parsedHeader = PacketUtils::parseHeaderPacket(headerPacketBuffer);

        // Print the parsed header packet
    std::cout << "Parsed Header Packet:" << std::endl;
    std::cout << "Packet ID: " << parsedHeader.packet_id << std::endl;
    std::cout << "Packet Timestamp: " << parsedHeader.pkt_ts << std::endl;
    std::cout << "Packet Type: " << parsedHeader.packet_type << std::endl;
    std::cout << "Packet Length: " << parsedHeader.packet_length << std::endl;


    // Parse the data packet
    DataPacket parsedData;
    PacketUtils::parseDataPacket(dataPacketBuffer, parsedData);



    // Print the parsed data packet
    std::cout << "Parsed Data Packet:" << std::endl;
    std::cout << "Receiver ID: "       << parsedData.rx_id << std::endl;
    std::cout << "GPS Data latitude: " << parsedData.latitude << std::endl;
    std::cout << "GPS Data longitude:" << parsedData.longitude << std::endl;
    std::cout << "GPS Data altitude: " << parsedData.altitude << std::endl;
    std::cout << "Total peak samples: " << parsedData.numTimeSamples <<std::endl;
    std::cout << "Packet Timestamp 0: " << parsedData.peak_timestamps.at(0) << std::endl;
    std::cout << "Packet Timestamp 1: " << parsedData.peak_timestamps.at(1) << std::endl;
    std::cout << "Packet Timestamp 2: " << parsedData.peak_timestamps.at(2) << std::endl;

    std::cout << "Waveform Samples:";
    for (std::complex<short> sample : parsedData.waveformSamples) 
    {
        std::cout << " " << sample;
    }
    std::cout << std::endl;
    std::cout << "Timestamps:";
    for (uint64_t timestamp :parsedData.peak_timestamps) 
    {
        std::cout << " " << timestamp;
    }
    std::cout << std::endl;
    std::cout << "Peak time sample id:";
    for(uint64_t id :parsedData.peak_ts_idx)
    {
        std::cout << " " << id; 
    }
    std::cout << std::endl;

    // Create control packet
    ControlMessage cm;
    cm.rx_id = 1;
    cm.ack = 0;
    cm.message.emplace_back("config");
    cm.message.emplace_back("ack");
    cm.message.emplace_back("sync");

    std::vector<char> controlPacketBuffer;
    PacketUtils::createControlPacket(cm, controlPacketBuffer);

    // Parse control packet
    // Parse the data packet
    ControlMessage parsedCM;
    PacketUtils::parseControlPacket(controlPacketBuffer, parsedCM);

    //Print parsed control message;
    std::cout <<"Parsed rx id: " << parsedCM.rx_id << std::endl;
    std::cout <<"Parsed ack  : " << parsedCM.ack << std::endl;
    std::cout <<"Parsed messages: " << std::endl;
    for(std::string msg:parsedCM.message)
    {
        std::cout << msg<<std::endl;
    }
    
    return 0;
}
