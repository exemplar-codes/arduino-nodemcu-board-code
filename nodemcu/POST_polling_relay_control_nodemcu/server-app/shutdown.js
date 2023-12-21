const { exec } = require("child_process");
const express = require("express");

const router = express.Router();
function checkLockStatus(returnBoolean = false) {
  const script = `
      function screenIsLocked { [ "$(/usr/libexec/PlistBuddy -c "print :IOConsoleUsers:0:CGSSessionScreenIsLocked" /dev/stdin 2>/dev/null <<< "$(ioreg -n Root -d1 -a)")" = "true" ] && return 0 || return 1; }
      function screenIsUnlocked { [ "$(/usr/libexec/PlistBuddy -c "print :IOConsoleUsers:0:CGSSessionScreenIsLocked" /dev/stdin 2>/dev/null <<< "$(ioreg -n Root -d1 -a)")" != "true" ] && return 0 || return 1; }

      if screenIsLocked; then
          echo "Screen locked";
      fi

      if screenIsUnlocked; then
          echo "Screen unlocked";
      fi

      if ! screenIsLocked; then
          echo "Screen unlocked (inverse logic)";
      fi

      if ! screenIsUnlocked; then
          echo "Screen locked (inverse logic)";
      fi
  `;

  return new Promise((resolve, reject) => {
    exec(`bash -c '${script}'`, (error, stdout) => {
      const status = stdout.trim();
      const timestamp = new Date().toUTCString();
      // const logMessage = `${timestamp}: ${status}\n`;

      if (error) reject(error);
      else resolve(status);
    });
  });
}

router.get("/locked", async (req, res, next) => {
  // logLockStatus()
  checkLockStatus()
    .then((argp) => {
      const locked = argp.toLowerCase().includes(" locked");
      res.json({ locked });
    })
    .catch((argp) => {
      res.json(argp);
    });
});

router.get("/locked/do", async (req, res, next) => {
  // logLockStatus()
  checkLockStatus()
    .then((argp) => {
      const locked = argp.toLowerCase().includes(" locked");
      if (!locked) {
        res.json({ message: "Device is unlocked, ignoring turn off command" });
      } else {
        const DEVICES = {
          LINUX: "Linux",
          MACOS: "Darwin",
        };
        const TURN_OFF_COMMANDS = {
          [DEVICES.LINUX]: "poweroff",
          [DEVICES.MACOS]: `osascript -e 'tell app "System Events" to shut down'`,
        };
        const getDeviceCommand = (device) => {
          return TURN_OFF_COMMANDS[device || DEVICES.MACOS];
        };
        const naggingProcessKillCommand = `kill -9 $(PROMPT_EOL_MARK=''; pgrep -i -a 'iterm|qemu' | tr '\n' ' ');`;

        // TODO, get rid of callback hell and solve the issue here
        exec(
          `${naggingProcessKillCommand} ${getDeviceCommand(DEVICES.LINUX)}`
          // (err, stdout) => {
          //   res.json({
          //     message: "Turn off command ran. Device shutting down",
          //     err,
          //     stdout,
          //   });
          // }
        );
        exec(
          `${naggingProcessKillCommand} ${getDeviceCommand(DEVICES.MACOS)}`
          // (err, stdout) => {
          //   res.json({
          //     message: "Turn off command ran. Device shutting down",
          //     err,
          //     stdout,
          //   });
          // }
        );

        res.json({
          message: "Turn off command ran. Device shutting down",
          // err,
          // stdout,
        });
      }
    })
    .catch((argp) => {
      console.log({ argp });
      res.json(argp);
      // next();
    });
});
// TODO: lock macos screen
// osascript -e 'tell application "System Events" to keystroke "q" using {control down, command down}'
module.exports = router;
