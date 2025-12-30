#pragma once

#include <atomic>
#include <thread>
#include <zmq.hpp>
#include <cstdint>

#define P2P_MAX_DATA_SIZE 60

typedef struct _P2PPacket
{
  uint8_t size;                         //< Size of data
  uint8_t rssi;                         //< Received Signal Strength Intensity
  union {
    struct {
      uint8_t port;                 //< Header selecting channel and port
      uint8_t data[P2P_MAX_DATA_SIZE]; //< Data
    };
    uint8_t raw[P2P_MAX_DATA_SIZE+1];  //< The full packet "raw"
  };
} __attribute__((packed)) P2PPacket;

typedef void (*P2PCallback)(void* obj, P2PPacket *);

class RadioBus
{
  public:
      using socket_t = int;
      using Id = int;

    RadioBus() noexcept;

    Id id()const noexcept;

    void enstablis_connection(int num_clients) noexcept;

    bool radiolinkSendP2PPacketBroadcast(P2PPacket *p2pp) noexcept;
    void p2pRegisterCB(void* obj, P2PCallback cb) noexcept;

    ~RadioBus() noexcept;

  private:
    void _socket_setup(void) noexcept;
    static void _Accept(RadioBus* const self) noexcept;

  private:
    void* m_obj =nullptr;
    const Id m_id;
    int num_clients=0;
    P2PCallback m_callback = nullptr;

    std::thread receiver;
    std::atomic_bool running{true};

    zmq::context_t ctx{1};
    zmq::socket_t pub;
    zmq::socket_t sub;
};

