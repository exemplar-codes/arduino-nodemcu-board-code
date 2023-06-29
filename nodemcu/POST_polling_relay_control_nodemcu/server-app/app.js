const express = require("express");

const app = express();
const port = process.env.PORT || 3000;

let switchValue = true;

app.get("/", (req, res) => {
  res.send(JSON.stringify(switchValue));
});

app.get("/toggle", (req, res) => {
  switchValue = !switchValue;
  res.send(JSON.stringify({ message: `Switch value is ${switchValue} now` }));
});

app.use("*", (req, res) => {
  res.send(JSON.stringify({ message: "Sanjar's server" }));
});

app.listen(port, () => console.log("Server started at", port));
