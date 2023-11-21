#include "WireGuard-ESP32.h"

static WireGuard wg;

void init_wireguard_interface(){
  IPAddress local_ip;
  local_ip.fromString(WIREGUARD_LOCAL_ADDRESS);
  wg.begin(
    local_ip,          // IP address of the local interface
    WIREGUARD_LOCAL_PRIVATE_KEY,        // Private key of the local interface
    WIREGUARD_ENDPOINT_ADDRESS,   // Address of the endpoint peer.
    WIREGUARD_REMOTE_PUBLIC_KEY,         // Public key of the endpoint peer.
    WIREGUARD_ENDPOINT_PORT);
}