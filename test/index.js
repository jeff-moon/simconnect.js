const { SimConnectJs } = require("../build/Release/SimConnectJs.node");

const position = 1;
const requestId = 1;

const simconnect = new SimConnectJs();
let ret = simconnect.open("simconnectjs");
console.log(ret);

ret = simconnect.addToDataDefinition(position, "PLANE LATITUDE", "degrees", 4);
console.log(ret);

ret = simconnect.requestDataOnSimObjectType(1, position, 0, 0, () =>
  console.log("DID IT")
);

setInterval(() => console.log("HELLO"), 2000);
