#include <univalue.h>
#include "dag_singleton.h"
#include "validation.h"
#include "crypto/egihash.h"

using namespace egihash;

UniValue getepoch(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 0) {
        throw std::runtime_error("getepoch\n"
                                 "\nReturns current epoch number");
    }
    return static_cast<int>(chainActive.Height() / constants::EPOCH_LENGTH);
}

UniValue getseedhash(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1) {
        throw std::runtime_error("getseedhash\n \"epoch\" "
                                 "\nReturns the hex encoded seedhash for specified epoch n or current epoch if n is not specified"
                                 "\nArguments"
                                 "\n\"epoch\" the epoch number");
    }
    int epoch = 0;
    if (params[0].isNull()) {
        epoch = static_cast<int>(chainActive.Height() / constants::EPOCH_LENGTH);
    } else {
        try {
            epoch = std::stoi(params[0].get_str());
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error("provided argument \'" + params[0].get_str() + "\' is not an integer");
        }
    }
    return uint256S(get_seedhash(epoch * constants::EPOCH_LENGTH)).GetHex();
}

UniValue getdagsize(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1) {
        throw std::runtime_error("getdagsize\n \"epoch\" "
                                 "\nReturns the size of the DAG in bytes for the specified epoch n or the current epoch if n is not specified"
                                 "\nArguments"
                                 "\n\"epoch\" the epoch number");
    }
    int epoch = 0;
    if (params[0].isNull()) {
        epoch = static_cast<int>(chainActive.Height() / constants::EPOCH_LENGTH);
    } else {
        try {
            epoch = std::stoi(params[0].get_str());
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error("provided argument \'" + params[0].get_str() + "\' is not an integer");
        }
    }
    return dag_t::get_full_size(epoch * constants::EPOCH_LENGTH);
}

UniValue getdagcachesize(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1) {
        throw std::runtime_error("getdagcachesize\n \"epoch\" "
                                 "\nReturns the size of the DAG chache in bytes for the specified epoch n or the current epoch if n is not specified"
                                 "\nArguments"
                                 "\n\"epoch\" the epoch number");
    }
    int epoch = 0;
    if (params[0].isNull()) {
        epoch = static_cast<int>(chainActive.Height() / constants::EPOCH_LENGTH);
    } else {
        try {
            epoch = std::stoi(params[0].get_str());
        } catch (const std::invalid_argument& ex) {
            throw std::runtime_error("provided argument \'" + params[0].get_str() + "\' is not an integer");
        }
    }
    return cache_t::get_cache_size(epoch * constants::EPOCH_LENGTH);
}

UniValue getdag(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1) {
        throw std::runtime_error("getdag\n \"epoch\" "
                                 "\nReturns a JSON object specifying DAG information"
                                 "\nArguments"
                                 "\n\"epoch\" the epoch number");
    }
    UniValue result(UniValue::VOBJ);
    int epoch = 0;
    try {
        epoch = std::stoi(params[0].get_str());
    } catch (const std::invalid_argument& ex) {
        throw std::runtime_error("provided argument \'" + params[0].get_str() + "\' is not an integer");
    }
    auto block_num = epoch * constants::EPOCH_LENGTH;
    result.push_back(Pair("epoch", epoch));
    result.push_back(Pair("seedhash", uint256S(get_seedhash(block_num)).GetHex()));
    result.push_back(Pair("size", dag_t::get_full_size(block_num)));
    result.push_back(Pair("cache_size", cache_t::get_cache_size(block_num)));
    return result;
}

UniValue getcache(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1) {
        throw std::runtime_error("getcache\n \"epoch\" "
                                 "\nReturns a JSON object specifying DAG cache information"
                                 "\nArguments"
                                 "\n\"epoch\" the epoch number");
    }
    UniValue result(UniValue::VOBJ);
    int epoch = 0;
    try {
        epoch = std::stoi(params[0].get_str());
    } catch (const std::invalid_argument& ex) {
        throw std::runtime_error("provided argument \'" + params[0].get_str() + "\' is not an integer");
    }
    auto block_num = epoch * constants::EPOCH_LENGTH;
    result.push_back(Pair("epoch", epoch));
    result.push_back(Pair("seedhash", uint256S(get_seedhash(block_num)).GetHex()));
    result.push_back(Pair("size", cache_t::get_cache_size(block_num)));
    return result;
}

UniValue getactivedag(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1) {
        throw std::runtime_error("getactivedag\n"
                                 "\nReturns a JSON list specifying loaded DAG");
    }
    const auto& dag = ActiveDAG();
    if (dag == nullptr) {
        throw std::runtime_error("there is no active dag");
    }
    using namespace egihash;
    UniValue result(UniValue::VOBJ);
    result.push_back(Pair("epoch", dag->epoch()));
    std::string seedhash = get_seedhash(dag->epoch()* constants::EPOCH_LENGTH);
    result.push_back(Pair("seedhash", seedhash.empty() ? "0" : uint256S(seedhash).GetHex()));
    result.push_back(Pair("size", dag->size()));
    return std::move(result);
}
