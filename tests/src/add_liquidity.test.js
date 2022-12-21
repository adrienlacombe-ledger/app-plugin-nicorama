import { processTest, populateTransaction } from "./test.fixture";
import * as fs from "fs";


const contractName = "Paraswap";

const testLabel = "joinPoolVia0x"; // <= Name of the test
const testDirSuffix = "join_pool"; // <= directory to compare device snapshots to
const testNetwork = "polygon";
const signedPlugin = false;

// Swaap Router
const contractAddr = "0xe63e0f7526397a2900f870ef9df6b52cb6330755";
const chainID = 137;

// From : https://polygonscan.com/tx/0x71ac96610fa24ceca7302453813bb3ae2af9e0f3ceb8897ccef0fe1ec7bbef6c
const inputData = "0x9d0472e20000000000000000000000007f5f7411c2c7ec60e2db946abbe7dc354254870b0000000000000000000000000000000000000000000000005ff25be9d009cd3a00000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000063999f5600000000000000000000000000000000000000000000000000000000000000030000000000000000000000000000000000000000000000000b83e67733bb6280000000000000000000000000000000000000000000000000000000003d42adbc00000000000000000000000000000000000000000000000000000000004260a6";

// Create serializedTx and remove the "0x" prefix
const serializedTx = populateTransaction(contractAddr, inputData, chainID);
//fs.writeFileSync('./add_liquidity.txt' ,serializedTx)
//const serializedTx = fs.readFileSync('add_liquidity.txt').toString()
//console.log(serializedTx)

const devices = [
    {
        name: "nanos",
        label: "Nano S",
        steps: 12, // <= Define the number of steps for this test case and this device
    },
    {
        name: "nanox",
        label: "Nano X",
        steps: 7, // <= Define the number of steps for this test case and this device
    },
];

devices.forEach((device) =>
    processTest(device, contractName, testLabel, testDirSuffix, "", signedPlugin, serializedTx, testNetwork)
);
