const express = require("express");

const app = express();
const port = process.env.PORT || 4000;

let switchValue = true;

app.get("/toggle*", (req, res) => {
  switchValue = !switchValue; // switch state

  // create message
  const message = `Switch state changed to: ${switchValue} (${
    switchValue ? "on" : "off"
  })`;

  const isToggleQuick = req.url.includes("quick");
  if (isToggleQuick) {
    const script = `<script>window.close();</script>`;
    res.send(`
    <html>
      <pre><code>${JSON.stringify({ isToggleQuick })}</code></pre>
      <p>${message}</p>
      <script>window.close();</script>
    </html>`);
  } else {
    res.send(JSON.stringify({ message }));
  }

  // logg message
  console.log("GET /toggle*", { message }, new Date().toLocaleTimeString());
});

app.get("/", (req, res) => {
  // send response
  const switchValues = [switchValue, true]; // FIXME: hardcoded light
  const respValue = JSON.stringify(switchValues);
  res.send(respValue);

  // create and print log
  const message = `Switch states are: ${switchValues} (${switchValues.map((i) =>
    i ? "on" : "off"
  )})`;
  console.log("GET /", { message }, new Date().toLocaleTimeString());
});

app.use("*", (req, res) => {
  res.send(JSON.stringify({ message: "Sanjar's server" }));
});

app.listen(port, () => console.log("Server started at", port));
