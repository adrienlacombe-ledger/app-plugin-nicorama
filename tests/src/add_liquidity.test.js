import { processTest, populateTransaction } from "./test.fixture";
import * as fs from "fs";


const contractName = "Paraswap";

const testLabel = "joinPoolVia0x"; // <= Name of the test
const testDirSuffix = "join_pool"; // <= directory to compare device snapshots to
const testNetwork = "polygon";
const signedPlugin = false;

// Swaap Router
const chainID = 137;
const contractAddr = "0xe63e0f7526397a2900f870ef9df6b52cb6330755";

// From : https://polygonscan.com/tx/0x3ea1e6f00b5c63d05286c7c4f31f749d674c12680059a4e3a1d134e76d96a139
const inputData = "0x75fed9a00000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000018000000000000000000000000000000000000000000000000000000000000002000000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa8417400000000000000000000000000000000000000000000000000000000f7520e560000000000000000000000007f5f7411c2c7ec60e2db946abbe7dc354254870b0000000000000000000000000000000000000000000000008a3252541767c80000000000000000000000000000000000000000000000000000000000639a6a2800000000000000000000000000000000000000000000000000000000000000030000000000000000000000007ceb23fd6bc0add59e62ac25578270cff1b9f6190000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000001bfd67037b42cf73acf2047067bd4f2c47d9bfd6000000000000000000000000000000000000000000000000000000000000000300000000000000000000000000000000000000000000000010964e584883b70000000000000000000000000000000000000000000000000000000000585c8a7b00000000000000000000000000000000000000000000000000000000005f6aeb000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000aa0000000000000000000000000000000000000000000000000000000005d0290530000000000000000000000007ceb23fd6bc0add59e62ac25578270cff1b9f61900000000000000000000000000000000000000000000000010783b5642389500000000000000000000000000def1c0ded9bec7f1a1670819833240f027b25eff00000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000988415565b00000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000007ceb23fd6bc0add59e62ac25578270cff1b9f619000000000000000000000000000000000000000000000000000000005d02905300000000000000000000000000000000000000000000000010783b56423895b300000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000003800000000000000000000000000000000000000000000000000000000000000720000000000000000000000000000000000000000000000000000000000000000f000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000002c0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf12700000000000000000000000000000000000000000000000000000000000000140000000000000000000000000000000000000000000000000000000000000028000000000000000000000000000000000000000000000000000000000000002800000000000000000000000000000000000000000000000000000000000000240000000000000000000000000000000000000000000000000000000005d02905300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000280000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000001f574f4f46690000000000000000000000000000000000000000000000000000000000000000000000000000005d0290530000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000200000000000000000000000007400b665c8f4f3a951a99f1ee9872efb8778723d000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000f00000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000340000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf12700000000000000000000000007ceb23fd6bc0add59e62ac25578270cff1b9f61900000000000000000000000000000000000000000000000000000000000001400000000000000000000000000000000000000000000000000000000000000300000000000000000000000000000000000000000000000000000000000000030000000000000000000000000000000000000000000000000000000000000002c0ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000012556e6973776170563300000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00000000000000000000000000000000000000000000000010783b56423895b3000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000e592427a0aece92de3edee1f18e0157c058615640000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000002b0d500b1d8e8ef31e21c99d1db9a6444d3adf12700001f47ceb23fd6bc0add59e62ac25578270cff1b9f619000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000000030000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf1270000000000000000000000000eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee0000000000000000000000000000000000000000000000000000000000000000869584cd000000000000000000000000100000000000000000000000000000000000001100000000000000000000000000000000000000000000007b9277e723639a631d0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000042369c260000000000000000000000001bfd67037b42cf73acf2047067bd4f2c47d9bfd600000000000000000000000000000000000000000000000000000000005ebe85000000000000000000000000def1c0ded9bec7f1a1670819833240f027b25eff00000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000009a8415565b00000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000001bfd67037b42cf73acf2047067bd4f2c47d9bfd60000000000000000000000000000000000000000000000000000000042369c2600000000000000000000000000000000000000000000000000000000005ebe2600000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000003800000000000000000000000000000000000000000000000000000000000000740000000000000000000000000000000000000000000000000000000000000000f000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000002c0000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf127000000000000000000000000000000000000000000000000000000000000001400000000000000000000000000000000000000000000000000000000000000280000000000000000000000000000000000000000000000000000000000000028000000000000000000000000000000000000000000000000000000000000002400000000000000000000000000000000000000000000000000000000042369c2600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000280000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000001f574f4f466900000000000000000000000000000000000000000000000000000000000000000000000000000042369c260000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000200000000000000000000000007400b665c8f4f3a951a99f1ee9872efb8778723d000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000f00000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000360000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf12700000000000000000000000001bfd67037b42cf73acf2047067bd4f2c47d9bfd600000000000000000000000000000000000000000000000000000000000001400000000000000000000000000000000000000000000000000000000000000320000000000000000000000000000000000000000000000000000000000000032000000000000000000000000000000000000000000000000000000000000002e0ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003200000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000012556e6973776170563300000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff00000000000000000000000000000000000000000000000000000000005ebe25000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000c0000000000000000000000000e592427a0aece92de3edee1f18e0157c05861564000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000420d500b1d8e8ef31e21c99d1db9a6444d3adf12700001f47ceb23fd6bc0add59e62ac25578270cff1b9f6190001f41bfd67037b42cf73acf2047067bd4f2c47d9bfd6000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000c000000000000000000000000000000000000000000000000000000000000000030000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa841740000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf1270000000000000000000000000eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee0000000000000000000000000000000000000000000000000000000000000000869584cd00000000000000000000000010000000000000000000000000000000000000110000000000000000000000000000000000000000000000e5ee4ea5d9639a631d000000000000000000000000000000000000000000000000";

// skip : 3 items
// joinAsset: USDC (0x2791bca1f2de4661ed88a30c99a7a9449aa84174)
// joinAmount: 4149.349974  (0xf7520e56)  : 6 decimals
// output token: SPT : (0x7f5f7411c2c7ec60e2db946abbe7dc354254870b)
// poolAmountOut : 9.958112247192995840 (0x8a3252541767c800) : 18 decimals

// Create serializedTx and remove the "0x" prefix
const serializedTx = populateTransaction(contractAddr, inputData, chainID);
fs.writeFileSync('./joinPoolVia0x.txt' ,serializedTx)
//const serializedTx = fs.readFileSync('add_liquidity.txt').toString()
//console.log(serializedTx)

const devices = [
    {
        name: "nanos",
        label: "Nano S",
        steps: 2, // <= Define the number of steps for this test case and this device
    },
    /*{
        name: "nanox",
        label: "Nano X",
        steps: 7, // <= Define the number of steps for this test case and this device
    },*/
];

devices.forEach((device) =>
    processTest(device, contractName, testLabel, testDirSuffix, "", signedPlugin, serializedTx, testNetwork)
);
