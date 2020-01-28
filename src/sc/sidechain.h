#ifndef _SIDECHAIN_CORE_H
#define _SIDECHAIN_CORE_H

#include "sc/sidechaintypes.h"
#include <coins.h>

//------------------------------------------------------------------------------------
class CTransaction;
class CValidationState;
class uint256;
class CTxMemPool;
class CBlockUndo;

namespace Sidechain
{
// Validation functions
bool checkTxSemanticValidity(const CTransaction& tx, CValidationState& state);
bool anyForwardTransaction(const CTransaction& tx, const uint256& scId);
bool hasScCreationOutput(const CTransaction& tx, const uint256& scId); // return true if the tx is creating the scid

bool existsInMempool(const CTxMemPool& pool, const CTransaction& tx, CValidationState& state);
// End of Validation functions

class CSidechainsViewCache : public CCoinsViewBacked
{
public:
    CSidechainsViewCache(CCoinsView *cView);
    CSidechainsViewCache(const CSidechainsViewCache&) = delete;             //as in coins, forbid building cache on top of another
    CSidechainsViewCache& operator=(const CSidechainsViewCache &) = delete;
    bool HaveDependencies(const CTransaction& tx);

    bool HaveScInfo(const uint256& scId) const;
    bool GetScInfo(const uint256 & scId, ScInfo& targetScInfo) const;
    bool queryScIds(std::set<uint256>& scIdsList) const; //Similar to queryHashes

    bool UpdateScInfo(const CTransaction& tx, const CBlock&, int nHeight);
    bool ApplyMatureBalances(int nHeight, CBlockUndo& blockundo);

    bool RevertTxOutputs(const CTransaction& tx, int nHeight);
    bool RestoreImmatureBalances(int nHeight, const CBlockUndo& blockundo);

    bool BatchWrite(CCoinsMap &mapCoins, const uint256 &hashBlock,
                    const uint256 &hashAnchor, CAnchorsMap &mapAnchors,
                    CNullifiersMap &mapNullifiers, CSidechainsMap& sidechainMap);
    bool Flush();

private:
    mutable CSidechainsMap cacheSidechains;
    CSidechainsMap::const_iterator FetchSidechains(const uint256& scId) const;
    void Dump_info() const;
};

}; // end of namespace

#endif // _SIDECHAIN_CORE_H
