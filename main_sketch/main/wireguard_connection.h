#include "WireGuard-ESP32.h"

static WireGuard wg;
char private_key[] = "SGpW8eVmtoyPn/qhjRYwoRau6Z0PfyC5jetjmh/mEUA=";
IPAddress local_ip(10, 0, 0, 2);
char public_key[] = "l8VxWYQHad3Ex5EdsiJxxMfKaybkND3FO1EBQKZ451I=";
char endpoint_address[] = "192.168.233.110";
int endpoint_port = 51820;

void init_wireguard_interface(){
  configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
  wg.begin(
    local_ip,          // IP address of the local interface
    private_key,        // Private key of the local interface
    endpoint_address,   // Address of the endpoint peer.
    public_key,         // Public key of the endpoint peer.
    endpoint_port);
}