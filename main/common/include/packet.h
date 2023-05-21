#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <vector>
#include <complex>
//add log entry exit
typedef enum class ControlMessageCommands_e
{
    ControlMessageCommandInvalid = -1,
    config,
    sync,
    stream,
    sendData,
    powerSave,
    MAXControlMessageCommand
}ControlMessageCommands;


typedef enum app_result_e
{
    APP_FAILURE_NOMEM   = -2,
    APP_FAILURE_GENERAL = -1,
    APP_SUCCESS         = 0,
    APP_RESULT_MAX
} app_result;

typedef enum packetType_e
{
    PACKET_TYPE_MIN_INVALID = -1,
    PACKET_TYPE_CONTROL_MESSAGE,
    PACKET_TYPE_CONTROL_MESSAGE_RESPONSE,
    PACKET_TYPE_DATA,
    PACKET_TYPE_MAX
} packetType;

typedef struct HeaderPacket_s
{
    uint64_t  packet_id;
    uint64_t  pkt_ts; //packet timestamp
    uint64_t  packet_type;
    uint64_t  packet_length;
} HeaderPacket;

typedef struct ControlMessage_s
{
    uint32_t rx_id;
    uint32_t ack;
    std::vector<std::string> message; //includes configuration/ack/nack/other information. Pattern: size:message.
}ControlMessage;

typedef struct DataPacket_s
{
    uint64_t rx_id;
    double   longitude;
    double   latitude;
    double   altitude;
    uint64_t numTimeSamples;
    std::vector<double> peak_timestamps;
    std::vector<std::complex<short>> waveformSamples;
    std::vector<std::complex<int16_t>>* waveformSamples;
} DataPacket;

#endif
