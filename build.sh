#!/bin/bash -i 
#cdt-cpp ram.bitcoin.cpp -I.  -I../libbitcoin-consensus/src/clone/ -DBITCOIN_SERIALIZE_H -DBITCOIN_SCRIPT_SCRIPT_H
cdt-cpp ram.bitcoin.cpp -I. -Iexternal -I../libbitcoin-consensus/src/clone -DCHAR_EQUALS_INT8 