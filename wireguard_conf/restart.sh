#!/bin/bash

wg-quick down ./wg_server.conf
wg-quick up ./wg_server.conf