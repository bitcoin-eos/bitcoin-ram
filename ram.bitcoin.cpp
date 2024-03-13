#include <nlohmann/json.hpp>
#include <script/script.cpp>
#include <ram.bitcoin.hpp>
#include <util.hpp>

using json = nlohmann::json;

[[eosio::action]]
void ram::syncblock(const vector<ram::block_info>& blocks) {
    ram::block_table _block(get_self(), get_self().value);
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        check(json::accept(it->content), "ram::syncblock: not json format");
        auto itr = _block.find(it->height);
        if (itr != _block.end()) {
            _block.modify(itr, same_payer, [&](auto& row) { row.content = it->content; });
        } else {
            _block.emplace(get_self(), [&](auto& row) {
                row.id = it->height;
                row.content = it->content;
            });
        }
    }
}

[[eosio::action]]
void ram::synctx(const uint64_t height, const vector<ram::tx_info>& txs) {
    ram::tx_table _tx(get_self(), height);
    for (auto it = txs.begin(); it != txs.end(); ++it) {
        auto itr = _tx.find(it->id);
        check(json::accept(it->content), "ram::synctx: not json format");
        if (itr != _tx.end()) {
            _tx.modify(itr, same_payer, [&](auto& row) { row.content = it->content; });
        } else {
            _tx.emplace(get_self(), [&](auto& row) {
                row.id = it->id;
                row.content = it->content;
            });
        }
    }
}

[[eosio::action]]
void ram::decode(const string& hex) {
    auto result = decodep2tr(hex);
    if (get<0>(result)) {
        auto p2tr = get<1>(result);
        print_f("p2tr: %, inscription: %", util::p2trToString(p2tr), util::hexStringToAscii(std::get<1>(p2tr[p2tr.size() - 2])));
    }
}

tuple<bool, vector<tuple<string, string>>> ram::decodep2tr(const string& data) {
    vector<tuple<string, string>> p2tr;
    vector<unsigned char> scriptBytes = util::hexStringToBytes(data);
    CScript script = CScript(scriptBytes.begin(), scriptBytes.end());
    if (!script.HasValidOps()) {
        return make_tuple(false, p2tr);
    }

    prevector<28, unsigned char>::const_iterator programCounter = script.begin();
    opcodetype nextOpcode;
    vector<unsigned char> returnData;
    while (programCounter != script.end()) {
        while (programCounter != script.end()) {
            if (*programCounter >= 0x01 && *programCounter <= 0x4b) {
                size_t numBytes = (size_t)*programCounter;
                programCounter++;

                string value;
                for (size_t i = 0; i < numBytes && programCounter != script.end(); i++) {
                    value += util::hexCharToString(*programCounter);
                    programCounter++;
                }
                p2tr.push_back(make_tuple("OP_PUSHBYTES_" + to_string(numBytes), value));
            } else {
                script.GetOp(programCounter, nextOpcode, returnData);

                string key = GetOpName(nextOpcode);
                string value;
                if (!returnData.empty()) {
                    for (auto it = returnData.begin(); it != returnData.end(); it++) {
                        value += util::hexCharToString(*it);
                    }
                }
                p2tr.push_back(make_tuple(key, value));
            }
        }
    }
    return make_tuple(true, p2tr);
}
