import { processTest, populateTransaction } from "./test.fixture";
import * as fs from "fs";


const contractName = "Paraswap";

const testLabel = "joinPoolViaSwaap"; // <= Name of the test
const testDirSuffix = "join_pool_swaap"; // <= directory to compare device snapshots to
const testNetwork = "polygon";
const signedPlugin = false;

// Swaap Router
const chainID = 137;
const contractAddr = "0xe63e0f7526397a2900f870ef9df6b52cb6330755";

// From : https://polygonscan.com/tx/0x6eec6d4bb6c42f910683ff3839a916384a94d9d8b237469b289c4128c4a4bcba
const inputData = "0x52dd2ee60000000000000000000000007f5f7411c2c7ec60e2db946abbe7dc354254870b0000000000000000000000007ceb23fd6bc0add59e62ac25578270cff1b9f6190000000000000000000000000000000000000000000000000002496d83138bf90000000000000000000000000000000000000000000000000006ea11ded7f28e0000000000000000000000000000000000000000000000000000000063b462ee";

// skip : 1 items
// joinAsset: WETH (0x7ceb23fd6bc0add59e62ac25578270cff1b9f619)
// joinAmount: 0.000643684652780537  (0x2496d83138bf9)  : 18 decimals

// Create serializedTx and remove the "0x" prefix
const serializedTx = populateTransaction(contractAddr, inputData, chainID);
// fs.writeFileSync('./joinPoolVia0x.txt' ,serializedTx)
//const serializedTx = fs.readFileSync('add_liquidity.txt').toString()
//console.log(serializedTx)

const devices = [
    {
        name: "nanos",
        label: "Nano S",
        steps: 4, // <= Define the number of steps for this test case and this device
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
