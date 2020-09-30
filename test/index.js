const { SimConnectJs } = require("../build/Release/SimConnectJs.node");

const position = 1;
const requestId = 1;

const simconnect = new SimConnectJs();
let ret = simconnect.open("simconnectjs");
console.log(ret);


const defId = simconnect.createDataDefinition([
  {
    datumName: "PLANE ALTITUDE",
    units: "degrees",
    dataType: 4
  },
  {
    datumName: "PLANE LONGITUDE",
    units: "degrees",
    dataType: 4
  }
]);

(async () => {
  const values = await simconnect.requestDataOnSimObjectType(defId);
})();
