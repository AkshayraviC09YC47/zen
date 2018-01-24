// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
// ZEN_MOD_START
#include <ctime> // used for chainsplit
// ZEN_MOD_END

#include <boost/assign/list_of.hpp>

#include "base58.h"

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
// ZEN_MOD_START
        strCurrencyUnits = "ZEN";
// ZEN_MOD_END
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidySlowStartInterval = 20000;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPowTargetSpacing = 2.5 * 60;
// ZEN_MOD_START
//        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nChainsplitIndex = 110000;
        consensus.nChainsplitTime = 1496187000;

        /**
         * ZEN Network Magic Start Value
         */
        pchMessageStart[0] = 0x63;
        pchMessageStart[1] = 0x61;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x68;
        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 9033;
        nMinerThreads = 0;
        std::time_t result = std::time(nullptr);
        if (result < consensus.nChainsplitTime + (24 * 60 * 60))
            nMaxTipAge = 10 * 24 * 60 * 60; // Allow blocks up to 10 days old to be the chain tip
        else
            nMaxTipAge = 24 * 60 * 60;
// ZEN_MOD_END
        nPruneAfterHeight = 100000;
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
         * >>> from pyblake2 import blake2s
         * >>> 'Zcash' + blake2s(b'The Economist 2016-10-29 Known unknown: Another crypto-currency is born. BTC#436254 0000000000000000044f321997f336d2908cf8c8d6893e88dbf067e2d949487d ETH#2521903 483039a6b6bd8bd05f0584f9a078d075e454925eb71c1f13eaff59b405a721bb DJIA close on 27 Oct 2016: 18,169.68').hexdigest()
         */
        const char* pszTimestamp = "Zcash0b9c4eef8b7cc417ee5001e3500984b6fea35683a7cac141a043c42064835d34";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime    = 1477641360;
        genesis.nBits    = 0x1f07ffff;
        genesis.nNonce   = uint256S("0x0000000000000000000000000000000000000000000000000000000000001257");
        genesis.nSolution = ParseHex("000a889f00854b8665cd555f4656f68179d31ccadc1b1f7fb0952726313b16941da348284d67add4686121d4e3d930160c1348d8191c25f12b267a6a9c131b5031cbf8af1f79c9d513076a216ec87ed045fa966e01214ed83ca02dc1797270a454720d3206ac7d931a0a680c5c5e099057592570ca9bdf6058343958b31901fce1a15a4f38fd347750912e14004c73dfe588b903b6c03166582eeaf30529b14072a7b3079e3a684601b9b3024054201f7440b0ee9eb1a7120ff43f713735494aa27b1f8bab60d7f398bca14f6abb2adbf29b04099121438a7974b078a11635b594e9170f1086140b4173822dd697894483e1c6b4e8b8dcd5cb12ca4903bc61e108871d4d915a9093c18ac9b02b6716ce1013ca2c1174e319c1a570215bc9ab5f7564765f7be20524dc3fdf8aa356fd94d445e05ab165ad8bb4a0db096c097618c81098f91443c719416d39837af6de85015dca0de89462b1d8386758b2cf8a99e00953b308032ae44c35e05eb71842922eb69797f68813b59caf266cb6c213569ae3280505421a7e3a0a37fdf8e2ea354fc5422816655394a9454bac542a9298f176e211020d63dee6852c40de02267e2fc9d5e1ff2ad9309506f02a1a71a0501b16d0d36f70cdfd8de78116c0c506ee0b8ddfdeb561acadf31746b5a9dd32c21930884397fb1682164cb565cc14e089d66635a32618f7eb05fe05082b8a3fae620571660a6b89886eac53dec109d7cbb6930ca698a168f301a950be152da1be2b9e07516995e20baceebecb5579d7cdbc16d09f3a50cb3c7dffe33f26686d4ff3f8946ee6475e98cf7b3cf9062b6966e838f865ff3de5fb064a37a21da7bb8dfd2501a29e184f207caaba364f36f2329a77515dcb710e29ffbf73e2bbd773fab1f9a6b005567affff605c132e4e4dd69f36bd201005458cfbd2c658701eb2a700251cefd886b1e674ae816d3f719bac64be649c172ba27a4fd55947d95d53ba4cbc73de97b8af5ed4840b659370c556e7376457f51e5ebb66018849923db82c1c9a819f173cccdb8f3324b239609a300018d0fb094adf5bd7cbb3834c69e6d0b3798065c525b20f040e965e1a161af78ff7561cd874f5f1b75aa0bc77f720589e1b810f831eac5073e6dd46d00a2793f70f7427f0f798f2f53a67e615e65d356e66fe40609a958a05edb4c175bcc383ea0530e67ddbe479a898943c6e3074c6fcc252d6014de3a3d292b03f0d88d312fe221be7be7e3c59d07fa0f2f4029e364f1f355c5d01fa53770d0cd76d82bf7e60f6903bc1beb772e6fde4a70be51d9c7e03c8d6d8dfb361a234ba47c470fe630820bbd920715621b9fbedb49fcee165ead0875e6c2b1af16f50b5d6140cc981122fcbcf7c5a4e3772b3661b628e08380abc545957e59f634705b1bbde2f0b4e055a5ec5676d859be77e20962b645e051a880fddb0180b4555789e1f9344a436a84dc5579e2553f1e5fb0a599c137be36cabbed0319831fea3fddf94ddc7971e4bcf02cdc93294a9aab3e3b13e3b058235b4f4ec06ba4ceaa49d675b4ba80716f3bc6976b1fbf9c8bf1f3e3a4dc1cd83ef9cf816667fb94f1e923ff63fef072e6a19321e4812f96cb0ffa864da50ad74deb76917a336f31dce03ed5f0303aad5e6a83634f9fcc371096f8288b8f02ddded5ff1bb9d49331e4a84dbe1543164438fde9ad71dab024779dcdde0b6602b5ae0a6265c14b94edd83b37403f4b78fcd2ed555b596402c28ee81d87a909c4e8722b30c71ecdd861b05f61f8b1231795c76adba2fdefa451b283a5d527955b9f3de1b9828e7b2e74123dd47062ddcc09b05e7fa13cb2212a6fdbc65d7e852cec463ec6fd929f5b8483cf3052113b13dac91b69f49d1b7d1aec01c4a68e41ce157");

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00040fe8ec8471911baa1db1266ea15dd06b4a8a5c453883c000b031973dce08"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4eaa58879081de3c24a7b117ed2b28300e7ec4c4c1dff1d3f1268b7857a4ddb"));

        vFixedSeeds.clear();
        vSeeds.clear();
// ZEN_MOD_START
        vSeeds.push_back(CDNSSeedData("zencash.io", "mainnet-zen.zencash.io"));
        vSeeds.push_back(CDNSSeedData("zdeveloper.org", "mainnet-zen.zdeveloper.org"));
        vSeeds.push_back(CDNSSeedData("rotorproject.org", "mainnet-zen.rotorproject.org"));

        // guarantees the first 2 characters, when base58 encoded, are "zn"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x20,0x89};
        // guarantees the first 2 characters, when base58 encoded, are "zs"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x20,0x96};
// ZEN_MOD_END
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
// ZEN_MOD_START
        // guarantees the first 2 characters, when base58 encoded, are "zn"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xA5};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
// ZEN_MOD_END
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

// ZEN_MOD_START
        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock)
            ( 30000, uint256S("0x000000005c2ad200c3c7c8e627f67b306659efca1268c9bb014335fdadc0c392"))
            ( 96577, uint256S("0x0000000177751545bd1af3ccf276ec2920d258453ab01f3d2f8f7fcc5f3a37b8"))
            ( 110000, uint256S("0x000000003f5d6ba1385c6cd2d4f836dfc5adf7f98834309ad67e26faef462454")),
            1495142510,     // * UNIX timestamp of last checkpoint block
            276839,         // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            1398            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };
// ZEN_MOD_END

        // Founders reward script expects a vector of 2-of-3 multisig addresses
// ZEN_MOD_START
        vFoundersRewardAddress = {
        "zssEdGnZCQ9G86LZFtbynMn1hYTVhn6eYCL",
        "zsrCsXXmUf8k59NLasEKfxA7us3iNvaPATz",
        "zsnLPsWMXW2s4w9EmFSwtSLRxL2LhPcfdby",
        "zshdovbcPfUAfkPeEE2qLbKnoue9RsbVokU",
        "zsqmq97JAKCCBFRGvxgv6FiJgQLCZBDp62S",
        "zskyFVFA7VRYX8EGdXmYN75WaBB25FmiL3g",
        "zsmncLmwEUdVmAGPUrUnNKmPGXyej7mbmdM",
        "zsfa9VVJCEdjfPbku4XrFcRR8kTDm2T64rz",
        "zsjdMnfWuFi46VeN2HSXVQWEGsnGHgVxayY",
        "zseb8wRQ8rZ722oLX5B8rx7qwZiBRb9mdig",
        "zsjxkovhqiMVggoW7jvSRi3NTSD3a6b6qfd",
        "zsokCCSU3wvZrS2G6mEDpJ5cH49E7sDyNr1",
        "zt12EsFgkABHLMRXA7JNnpMqLrxsgCLnVEV",
        "zt39mvuG9gDTHX8A8Qk45rbk3dSdQoJ8ZAv",
        "zssTQZs5YxDGijKC86dvcDxzWogWcK7n5AK",
        "zsywuMoQK7Bved2nrXs56AEtWBhpb88rMzS",
        "zsxVS2w7h1fHFX2nQtGm4372pd4DSHzq9ee",
        "zsupGi7ro3uC8CEVwm9r7vrdVUZaXQnHF6T",
        "zshVZvW47dA5AB3Sqk1h7ytwWJeUJUJxxaE",
        "zsubBCjvDx252MKFsL4Dcf5rJU9Z9Upqr1N",
        "zsweaST3NcU4hfgkVULfCsfEq41pjgMDgcW",
        "zswz6Rxb1S33fUpftETZwtGiVSeYxNKq2xc",
        "zswnpHtiBbrvYDzbhPQshkgvLSfYhDMRJ4S",
        "zsjSYAWaEYj35Ht7aXrRJUGY6Dc8qCmgYqu",
        "zsvMv8fGroWR8epbSiGDCJHmfe6ec2uFQrt",
        "zsujxCT56BExQDAwKwktBjtnopYnw8BiKbg",
        "zsxeXc2FTAzmUmeZmqVsKVdwTMSvzyns4rT",
        "zsuLqgABNudD8bVPbVGeUjGqapuoXp68i7F",
        "zsoc39J1dCFK1U8kckZznvQkv8As7sajYLz",
        "zt21NFdu1KRPJ7VRKtrWugM2Jqe5ePNmU4T",
        "zsp15qbVcbx9ifcjKe6XZEJTvzsFUZ2BHLT",
        "zso2KvqH6yxLQEYggHdmfL3Tcd5V6E9tqhp",
        "zsnFG2W5ZHRYh3QucNze4mp31tBkemtfxdj",
        "zsex2CGJtxHyHbpLXm7kESBmp3vWRqUkJMy",
        "zsvtFv96nrgrXKUbtNe2BpCt8aQEp5oJ7F8",
        "zsk5KitThmhK9KBa1KDybPgEmGSFTHzhMVA",
        "zsuy4n48c4NsJyaCZEzwdAKULM1FqbB6Y4z",
        "zsgtQVMpX2zNMLvHHG2NDwfqKoaebvVectJ",
        "zszQqXRSPGdqsWw4iaMTNN6aJz4JjEzSdCF",
        "zst6dBLrTtaMQBX7BLMNjKLTGcP11PBmgTV",
        "zshD9r6Eb6dZGdzYW2HCb9CzkMokCT1NGJR",
        "zswUaj1TboEGmvSfF7fdoxWyH3RMx7MBHHo",
        "zsv8s4Poi5GxCsbBrRJ97Vsvazp84nrz5AN",
        "zsmmxrKU6dqWFwUKow1iyovg3gxrgXpEivr",
        "zskh1221aRC9WEfb5a59WxffeW34McmZZsw",
        "zssAhuj57NnVm4yNFT6o8muRctABkUaBu3L",
        "zsi5Yr4Z8HwBvdBqQE8gk7ahExDu95J4oqZ",
        "zsy6ryEaxfk8emJ8bGVB7tmwRwBL8cfSqBW",
        //"zsvM6GdLJWXAvKs9ruUDgEdKiJzN7qrtKcP",
        //"zsre8uXg4TJTuqSaiKLQYjMd5ST3UwYorTj",
        //"zsem4VjWQuzhhhWPLwQN39SewXV1xaCVrR4",
        //"zt17Ett8K57LnhMt5RjUeYrjXDocjqt2oja",
        //"zt2PZSoyKuigEgM6ss6id5wqem69mwSKSnP",
        //"zszxnNPj2zg81McDarbQi76y3NYeqj8PkwU",
        //"zsi3PoGMUzkj8kPAaq9YGYUS8Wp2pDRjR8X",
        };
// ZEN_MOD_END
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
// ZEN_MOD_START
        strCurrencyUnits = "ZNT";
// ZEN_MOD_END
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
// ZEN_MOD_START
//        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nChainsplitIndex = 70000;
        consensus.nChainsplitTime = 1494616813; // 05/12/2017 @ 7:20pm (UTC)

        pchMessageStart[0] = 0xbf;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xcd;
        pchMessageStart[3] = 0xe6;
        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 18033;
//        nMinerThreads = 0;
// ZEN_MOD_END
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1477648033;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000006");
        genesis.nSolution = ParseHex("00a6a51259c3f6732481e2d035197218b7a69504461d04335503cd69759b2d02bd2b53a9653f42cb33c608511c953673fa9da76170958115fe92157ad3bb5720d927f18e09459bf5c6072973e143e20f9bdf0584058c96b7c2234c7565f100d5eea083ba5d3dbaff9f0681799a113e7beff4a611d2b49590563109962baa149b628aae869af791f2f70bb041bd7ebfa658570917f6654a142b05e7ec0289a4f46470be7be5f693b90173eaaa6e84907170f32602204f1f4e1c04b1830116ffd0c54f0b1caa9a5698357bd8aa1f5ac8fc93b405265d824ba0e49f69dab5446653927298e6b7bdc61ee86ff31c07bde86331b4e500d42e4e50417e285502684b7966184505b885b42819a88469d1e9cf55072d7f3510f85580db689302eab377e4e11b14a91fdd0df7627efc048934f0aff8e7eb77eb17b3a95de13678004f2512293891d8baf8dde0ef69be520a58bbd6038ce899c9594cf3e30b8c3d9c7ecc832d4c19a6212747b50724e6f70f6451f78fd27b58ce43ca33b1641304a916186cfbe7dbca224f55d08530ba851e4df22baf7ab7078e9cbea46c0798b35a750f54103b0cdd08c81a6505c4932f6bfbd492a9fced31d54e98b6370d4c96600552fcf5b37780ed18c8787d03200963600db297a8f05dfa551321d17b9917edadcda51e274830749d133ad226f8bb6b94f13b4f77e67b35b71f52112ce9ba5da706ad9573584a2570a4ff25d29ab9761a06bdcf2c33638bf9baf2054825037881c14adf3816ba0cbd0fca689aad3ce16f2fe362c98f48134a9221765d939f0b49677d1c2447e56b46859f1810e2cf23e82a53e0d44f34dae932581b3b7f49eaec59af872cf9de757a964f7b33d143a36c270189508fcafe19398e4d2966948164d40556b05b7ff532f66f5d1edc41334ef742f78221dfe0c7ae2275bb3f24c89ae35f00afeea4e6ed187b866b209dc6e83b660593fce7c40e143beb07ac86c56f39e895385924667efe3a3f031938753c7764a2dbeb0a643fd359c46e614873fd0424e435fa7fac083b9a41a9d6bf7e284eee537ea7c50dd239f359941a43dc982745184bf3ee31a8dc850316aa9c6b66d6985acee814373be3458550659e1a06287c3b3b76a185c5cb93e38c1eebcf34ff072894b6430aed8d34122dafd925c46a515cca79b0269c92b301890ca6b0dc8b679cdac0f23318c105de73d7a46d16d2dad988d49c22e9963c117960bdc70ef0db6b091cf09445a516176b7f6d58ec29539166cc8a38bbff387acefffab2ea5faad0e8bb70625716ef0edf61940733c25993ea3de9f0be23d36e7cb8da10505f9dc426cd0e6e5b173ab4fff8c37e1f1fb56d1ea372013d075e0934c6919393cfc21395eea20718fad03542a4162a9ded66c814ad8320b2d7c2da3ecaf206da34c502db2096d1c46699a91dd1c432f019ad434e2c1ce507f91104f66f491fed37b225b8e0b2888c37276cfa0468fc13b8d593fd9a2675f0f5b20b8a15f8fa7558176a530d6865738ddb25d3426dab905221681cf9da0e0200eea5b2eba3ad3a5237d2a391f9074bf1779a2005cee43eec2b058511532635e0fea61664f531ac2b356f40db5c5d275a4cf5c82d468976455af4e3362cc8f71aa95e71d394aff3ead6f7101279f95bcd8a0fedce1d21cb3c9f6dd3b182fce0db5d6712981b651f29178a24119968b14783cafa713bc5f2a65205a42e4ce9dc7ba462bdb1f3e4553afc15f5f39998fdb53e7e231e3e520a46943734a007c2daa1eda9f495791657eefcac5c32833936e568d06187857ed04d7b97167ae207c5c5ae54e528c36016a984235e9c5b2f0718d7b3aa93c7822ccc772580b6599671b3c02ece8a21399abd33cfd3028790133167d0a97e7de53dc8ff");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x05a60a92d99d85997cce3b87616c089f6124d7342af37106edc76126334a2c38"));

        vFixedSeeds.clear();
        vSeeds.clear();
// ZEN_MOD_START
        vSeeds.push_back(CDNSSeedData("zencash.io", "testnet-zen.zencash.io"));
        vSeeds.push_back(CDNSSeedData("zdeveloper.org", "testnet-zen.zdeveloper.org"));
        vSeeds.push_back(CDNSSeedData("rotorproject.org", "testnet-zen.rotorproject.org"));

        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x20,0x98};
        // guarantees the first 2 characters, when base58 encoded, are "zr"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x20,0x92};
// ZEN_MOD_END
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock)
            (38000, uint256S("0x001e9a2d2e2892b88e9998cf7b079b41d59dd085423a921fe8386cecc42287b8")),
            1486897419,  // * UNIX timestamp of last checkpoint block
            47163,       // * total number of transactions between genesis and last checkpoint
                         //   (the tx=... number in the SetBestChain debug.log lines)
            715          //   total number of tx / (checkpoint block height / (24 * 24))
        };

// ZEN_MOD_START
        // Founders reward script expects a vector of 3-of-4 multisig addresses
        vFoundersRewardAddress = {
            "zrH8KT8KUcpKKNBu3fjH4hA84jZBCawErqn", "zrGsMC4ou1r5Vxy7Dnxg4PfKpansx83BM8g", "zr6sB2Az36D8CqzeZNavB11WbovsGtJSAZG", "zrBAG3pXCTDq14nivNK9mW8SfwMNcdmMQpb",
            "zrRLwpYRYky4wsvwLVrDp8fs89EBTRhNMB1", "zrLozMfptTmE3zLP5SrTLyB8TXqH84Agjrr", "zrMckkaLtVTEUvxj4ouU7BPDGa8xmdTZSVE", "zrFc897wJXmF7BcEdbvi2mS1bLrcuWYK6hm",
            "zrHEnni486u9SNcLWacroSgcdyMA33tfM92", "zrJ3ymPV3R8Xk4N3BdNb898xvZvARm5K7mq", "zrDj3P6trx73291krxU51U9QnfkbGxkyJ6E", "zrJs3vMGFJi9pQCireeSLckJonamBnwTSrY",
            "zrKFdXQoAkkycy52EFoWARyzZWx6Kat2Som", "zrEXbSe79FXA9KRMnJTZUWZdZhNnjcnAdrq", "zr7iAwfNgJsMpSCmijK3TuVDuNvSmLr1rUz", "zrDEK7K6cftqSjeyVUH1WqJtBUkXN7GidxH",
            "zrRennuq75hyBVU4JymtZk8UcQ1vRPKpmpj", "zr9HRTL79pKmn5R8fvkC9kucZ4u1bQruLTD", "zrML8KXpJsa1NVnbJuawX86ZvAn543tWdTT", "zrLBAkQoxpEtnztSUEcdxnEvuwtgxyAMGX7",
            "zr6kPnVzFBYmcBDsWoTrPHRuBxLq21o4zvT", "zrMY3vdvqs9KSvx9TawvcyuVurt1Jj6GPVo", "zr9WB1qBpM4nwi1mudUFfjtMNmqzaBQDsXn", "zrAHbtHDPAqmzWJMQqSYzGyFnDWN3oELZRs",
            "zrH1f5K3z7EQ6RWWZ7StCDWHTZwFChBVA2W", "zrNTacAid9LS4kAqzM4sw1YcF7gLFrzVM7U", "zrFyZpMVKMeDqbn6A2uUiL9mZmgxuR1pUBg", "zrD1cqGFGzBcPogFHJvnN4XegvvmbTjA43t",
            "zr5A1D7czWkB4pAWfGC5Pux5Ek7anYybdPK", "zr8yTAxCy6jAdsc6qPvmVEQHbYo25AJKhy9", "zrFW2YjQw4cABim5kEDwapbSqTz3wW7cWkk", "zr9nJvNbsrvUTZD41fhqAQeUcgMfqZmAweN",
            "zrCx4dXZd5b2tD483Ds4diHpo1QxBMJ76Jr", "zr6eVeRwU6Puob3K1RfWtva1R458oj8pzkL", "zr7B92iHtQcobZjGCXo3DAqMQjsn7ka31wE", "zr8bcemLWAjYuphXSVqtqZWEnJipCB9F5oC",
            "zrFzsuPXb7HsFd3srBqtVqnC9GQ94DQubV2", "zr4yiBobiHjHnCYi75NmYtyoqCV4A3kpHDL", "zrGVdR4K4F8MfmWxhUiTypK7PTsvHi8uTAh", "zr7WiCDqCMvUdH1xmMu8YrBMFb2x2E6BX3z",
            "zrEFrGWLX4hPHuHRUD3TPbMAJyeSpMSctUc", "zr5c3f8PTnW8qBFX1GvK2LhyLBBCb1WDdGG", "zrGkAZkZLqC9QKJR3XomgxNizCpNuAupTeg", "zrM7muDowiun9tCHhu5K9vcDGfUptuYorfZ",
            "zrCsWfwKotWnQmFviqAHAPAJ2jXqZYW966P", "zrLLB3JB3jozUoMGFEGhjqyVXTpngVQ8c4T", "zrAEa8YjJ2f3m2VsM1Xa9EwibZxEnRoSLUx", "zrAdJgp7Cx35xTvB7ABWP8YLTNDArMjP1s3"
        };
// ZEN_MOD_END
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
// ZEN_MOD_START
        consensus.nSubsidyHalvingInterval = 2000;
// ZEN_MOD_END
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
// ZEN_MOD_START
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.nChainsplitIndex = 1000;
        consensus.nChainsplitTime = 0;

        pchMessageStart[0] = 0x2f;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0x9d;
//        nMinerThreads = 1;
// ZEN_MOD_END
        nMaxTipAge = 24 * 60 * 60;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
// ZEN_MOD_START
        genesis.hashPrevBlock.SetNull();
        genesis.nTime = 1494548150;
        genesis.nBits = 0x200f0f0f;
        genesis.nVersion = 4;
        genesis.nNonce = uint256S("0x000000000000000000000000000000000000000000000000000000000000003d");
        genesis.nSolution = ParseHex("00CBA7185285F4FF37432E1F3AA7A569FBC81B5A0876F23DA8D38840B0130C74E68297B5");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18133;
        assert(consensus.hashGenesisBlock == uint256S("0x0da5ee723b7923feb580518541c6f098206330dbc711a6678922c11f2ccf1abb"));
// ZEN_MOD_END

        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
// ZEN_MOD_START
            ( 0, uint256S("0x0da5ee723b7923feb580518541c6f098206330dbc711a6678922c11f2ccf1abb")),
// ZEN_MOD_END
            0,
            0,
            0
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
// ZEN_MOD_START
        vFoundersRewardAddress = { "zrKmSdqZKZjnARd5e8FfRg4v1m74X7twxGa" };
// ZEN_MOD_END
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CBitcoinAddress address(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(address.IsValid());
    assert(address.IsScript());
    CScriptID scriptID = get<CScriptID>(address.Get()); // Get() returns a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}
