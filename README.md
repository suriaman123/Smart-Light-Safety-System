# Smart-Light-Safety-System

An Arduino-based system that monitors temperature near a candle and automatically flame off the candle by turning on a cooling fan if it gets too hot. Designed as a safety-first automation project — extinguish logic comes first, auto-reignition is an experimental future goal.

## Current Status

- **Auto-extinguish on overheat** — working
- **Auto-reignite after cooldown** — in progress / experimental

## How It Works

1. The DHT22 continuously monitors ambient temperature near the candle.
2. If temperature crosses a defined threshold, the system:
   - Turns on the DC fan (via MOSFET) to cool the area down
3. The IR sensor is used as a secondary input to detect flame presence/absence, independent of temperature, so the system isn't relying on temperature alone to infer candle state.
4. Once temperature drops back to a safe level, the fan turns off.

## Hardware Used

| Component | Purpose |
|---|---|
| Arduino Uno | Main controller |
| DHT22 | Temperature  sensing |
| IR sensor | Flame detection |
| MOSFET  | Switches DC fan on/off from a digital pin |
| DC fan + dedicated power supply | Active cooling |
| Nichrome wire *(current)* | Auto  ignite flame  |

### Wiring Notes

- The fan is not connected directly to an Arduino pin. It's switched via a MOSFET , with a flyback diode across the fan terminals and a pull-down resistor on the gate, since the fan's power supply is separate from the Arduino.
- DHT22 and IR sensor both run off the Arduino's 5V/GND, with their signal pins on separate digital/analog pins.



## Future Goals

### Auto-reignition after cooldown

The long-term goal is to have the candle automatically relight once the system confirms it's safe to do so.

**Current Idea: Nichrome wire**
A nichrome wire positioned at the wick can ignite it. This is the current experimental setup, but I have encountered two limitations :

1. **Too Hot To Handle** — the heating wire itself can register as "too hot" on the DHT22, which would re-trigger the fan. Planned fix: a dedicated `IGNITING` state in the code that temporarily suspends temperature-based fan triggering during an ignition attempt, with the IR flame sensor used afterward to confirm success.
2. **Fixed mounting height** — as the candle burns down, the wick sits progressively lower, and a fixed-position wire will eventually be too far away to ignite it. There's no clean software fix for this; it's a mechanical limitation of the current approach.

**Planned upgrade: Piezo igniter + servo**
To get around both issues above, the plan is to move to a servo-actuated piezo lighter, where a servo arm presses the lighter's trigger on command. This sidesteps the wick-height problem (a momentary spark near the wick works across a range of heights) and the sensor feedback issue (no sustained heating element to confuse the temperature sensor).

### Planned state machine (high level)

```
IDLE → MONITORING → OVERHEAT_DETECTED → FAN_ON + EXTINGUISH
     → COOLDOWN → IGNITION_ATTEMPT → CONFIRM_VIA_IR_SENSOR → MONITORING
```

A retry limit (e.g. max 2 ignition attempts) is planned so the system doesn't loop indefinitely if ignition keeps failing.

