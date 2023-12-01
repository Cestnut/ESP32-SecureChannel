#!/bin/bash

wg-quick down ./wg_user2.conf
wg-quick up ./wg_user2.conf
