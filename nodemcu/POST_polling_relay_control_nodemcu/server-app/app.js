const express = require("express");

const app = express();
const port = process.env.PORT || 3000;

let switchValue = true;

app.get("/", (req, res) => {
  const message = `Switch state is: ${switchValue} (${
    switchValue ? "on" : "off"
  })`;
  console.log("GET /", { message });
  res.send(JSON.stringify({ message }));
});

app.get("/toggle", (req, res) => {
  switchValue = !switchValue;
  const message = `Switch state changed to: ${switchValue} (${
    switchValue ? "on" : "off"
  })`;
  console.log(message);
  res.send(JSON.stringify({ message }));
});

app.use("*", (req, res) => {
  res.send(JSON.stringify({ message: "Sanjar's server" }));
});

app.listen(port, () => console.log("Server started at", port));
