const express = require("express");

const app = express();
const port = process.env.PORT || 4000;

let switchValues = [true, true, false, false];

function getMessage(currentSwitchValues) {
  return `Switch states are: ${currentSwitchValues} (${currentSwitchValues.map(
    (i) => (i ? "on" : "off")
  )})`;
}

function toggleSingleSwitch(index) {
  const indexIsValid = index < switchValues.length;
  if (!indexIsValid) return false;

  switchValues[index] = !switchValues[index];

  return true;
}

/**
 * @returns {Boolean} value changed
 */
function setSingleSwitch(index, value) {
  const indexIsValid = index < switchValues.length;
  if (!indexIsValid) return false;

  const existingValue = switchValues[index];
  switchValues[index] = value;
  const newValue = switchValues[index];

  return existingValue === newValue;
}

function setAll(value) {
  switchValues = switchValues.map((i) => value);
  return switchValues;
}

function getSwitchBooleanValue(value) {
  return value?.toLowerCase() === "on";
}

function togglePageMiddleware(req, res, next) {
  const message = getMessage(switchValues);

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

  return null;
}

app.get("/tog*/:index/:ignoreMe?", (req, res) => {
  // switchValues = switchValues.map((i) => !i); // toggle
  toggleSingleSwitch(req.params.index);

  // create message
  togglePageMiddleware(req, res);
});

app.get("/set/:switchIndex/:switchValue", (req, res) => {
  // switchValues = switchValues.map((i) => !i); // toggle
  setSingleSwitch(
    req.params.switchIndex,
    getSwitchBooleanValue(req.params.switchValue)
  );

  // create message
  togglePageMiddleware(req, res);
});

app.get("/all/:value", (req, res) => {
  setAll(getSwitchBooleanValue(req.params.value));

  togglePageMiddleware(req, res);
});

app.get("/manual", (req, res) => {
  setAll(getSwitchBooleanValue("on"));

  togglePageMiddleware(req, res);
});

app.use("/help", (req, res) => {
  res.send(`<ol><li>
  <p>Set a switch<p>
  <p>'set' - <pre><code>/set/:switchIndex/:switchValue</pre></code></p>
  <p>Example: /set/0/on, /set/1/off</p>
  </li>

  <li>
  <p>Set a switch<p>
  <p>'tog* (toggle)' - <pre><code>/tog/:switchIndex</pre></code></p>
 <p>Example: /tog/0, /toggle/1</p>
  </li>


  <li>
  <p>Set all switch (to given value)</p>
  <p>'all' - <pre><code>/all/:value</pre></code></p>
  <p>Example: /all/on, /all/off</p>
  </li>

  <li>
  <p>Manual (you wish to use physical buttons)</p>
  <p>'manual'<pre><code>/manual</pre></code></p>
  </li>

  </ol>`);
});

app.get("/", (req, res) => {
  // send response
  // const switchValues = [switchValue, switchValue, false, false]; // FIXME: hardcoded light
  const respValue = JSON.stringify(switchValues);
  res.send(respValue);

  // create and print log
  const message = getMessage(switchValues);
  console.log("GET /", { message }, new Date().toLocaleTimeString());
});

app.use("*", (req, res) => {
  res.send(JSON.stringify({ message: "Sanjar's server" }));
});

app.listen(port, () => console.log("Server started at", port));
