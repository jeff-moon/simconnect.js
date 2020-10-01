const simconnect = require("../build/Release/SimConnectJs.node");

(async () => {
  let ret;
  console.log("STARTING OPEN");
  ret = await simconnect.open("simconnect_test");
  console.log(ret);
  console.log("COMPLETED OPEN");

  ret = simconnect.createDataDefinition([
    { id: "ID1", units: "UNITS1", dataType: 1 },
    { id: "ID2", units: "UNITS2", dataType: 2 },
  ]);
})();
