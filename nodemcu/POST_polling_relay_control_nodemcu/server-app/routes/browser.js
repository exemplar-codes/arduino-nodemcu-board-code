// Runs command sent by the browser
const express = require("express");

const router = express.Router();

// const shopController = require("../controllers/shop");

router.get("/ping", (req, res, next) => {
  res.json("Pong browser");
});

module.exports = router;
