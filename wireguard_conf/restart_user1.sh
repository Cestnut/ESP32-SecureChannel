#!/bin/bash

wg-quick down ./wg_user1.conf
wg-quick up ./wg_user1.conf
