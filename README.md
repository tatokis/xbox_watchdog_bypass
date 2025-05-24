Have an original XBOX that is FRAGing (two restarts and then Flashing Red And Green)? Are you frustrated that the SMC restarts it and then holds everything in reset while you're trying to figure out why?

Then this is for you!

A hacky Arduino sketch that satisfies the SMC challenge-response sequence and keeps the XBOX running. Tested against a v1.0 XBOX. Other versions should work, but are untested.

The sketch is written for an Arduino Micro, but it should work with any Arduino (pins will be different).

On the Micro, all needed pins are next to each other. NOTE: The [LPC header](https://xboxdevwiki.net/LPC_Debug_Port)'s pin 1 is at the bottom right with the front of the console facing the user.

|Arduino Micro Pin | Function | XBOX LPC Pin|
|------------------|----------|-------------|
|       GND        |   GND    |      12     |
|       D2         |   SDA    |      14     |
|       D3         |   SCL    |      13     |
|       D4         |  LRESET  |      5      |


Thanks to [Cromwell](https://github.com/XboxDev/cromwell/blob/bf9dc2a5738124113a1cb099835610f7b6ba9184/boot_rom/2bPicResponseAction.c#L93) for the algorithm.