#pragma once

#include <atomic>
#include <array>
#include <cstdint>
#include <optional>

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

    void enstablis_connection(void) noexcept;

    bool radiolinkSendP2PPacketBroadcast(P2PPacket *p2pp) noexcept;
    void p2pRegisterCB(void* obj, P2PCallback cb) noexcept;

  private:
    void _socket_setup(void) noexcept;
    static void _Accept(RadioBus* const self) noexcept;

  public:
    static constexpr std::int32_t m_max_clients = 25;

  private:
    void* m_obj =nullptr;
    const Id m_id;
    socket_t m_socket=0;
    std::array<std::optional<socket_t>, m_max_clients> m_clients{};
    std::atomic_uint8_t m_client_connected=0;
    P2PCallback m_callback = nullptr;
};

