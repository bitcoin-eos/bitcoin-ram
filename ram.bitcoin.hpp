#pragma once
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("ram.bitcoin")]] ram : public contract {
   public:
    using contract::contract;

    // CONTRACTS

    // BASE SYMBOLS

    struct block_info{
        uint64_t height;
        string content;
    };
    struct tx_info{
        uint64_t id;
        string content;
    };

    struct [[eosio::table]] block_row {
        uint64_t id;
        string content;
        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<"blocks"_n, block_row> block_table;

    struct [[eosio::table]] tx_row {
        uint64_t id;
        string content;
        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<"txs"_n, tx_row> tx_table;

    /**
    * ## TABLE `config`
    *
    * - `{map<name, name>} status` - contract status
    * - `{map<name, uint16_t>} ratios` - trade fee ratios (pips 100/10000 of 1%)
    * - `{map<name, name>} accounts` - account
    * - `{map<name, uint64_t>} next_table_ids` - next table id
    *
    * ### example
    *
    * ```json
    * {
    *   "status": {"bid": "enabled", "ask": "disabled"},
    *   "ratios": {"bid": 1, "ask": 1},
    *   "accounts": {"admin": "admin.ararat"},
    *   "next_table_ids": {"order": 8},
    * }
    * ```
    */
    struct [[eosio::table("config")]] config_row {
        map<name, name> status;  // enabled/disabled
        map<name, uint16_t> ratios;
        map<name, name> accounts;
        map<name, uint64_t> next_table_ids;
    };
    typedef eosio::singleton<"config"_n, config_row> config_table;

    [[eosio::action]]
    void syncblock(const vector<ram::block_info>& blocks);

    [[eosio::action]]
    void synctx(const uint64_t height, const vector<ram::tx_info>& txs);

    [[eosio::action]]
    void decode(const string& hex);

   private:
    tuple<bool, vector<tuple<string, string>>> decodep2tr(const string& data);
};
