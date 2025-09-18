# Communication Structure



# Websocket Message Templates

## /Control

Purpose is to instruct the RCS backend state machine when to start different procedures. Will send until controller returns ACK message

#### RCS Trigger
```json
{
    "Trigger": "RCS", // RCS | Track
    "Action": "Initialize" // Initialize | Start | Autonomous | RC | Stop | Restart
}
```

#### Track Trigger
```json
{
    "Trigger": "Track", // RCS | Track
    "Action": "Red" // Red | Yellow | Orange | Green | White | Chequered
}
```

<!-- #### Kart Trigger
```json
{
    "Trigger": "Kart", // RCS | Track | Kart
    "Action": "Red" // Green | Black | White | Chequered
}
``` -->

## /Status

Purpose is to provide live updates to the control panel for status indicators. The Web App listens to this socket only.

#### Message when Disconnected
```json
{
    "MQTT": {
        "Connected": false, // true | false
    },
    "RCS": {
        "Connected": false, // true | false
    }
}
```

#### Message when Components are connected
```json
{
    "MQTT": {
        "Connected": true, // true | false
        "Host": "IP", // localhost
        "Port": 1883
    },
    "RCS": {
        "Connected": true, // true | false
        "State": "Uninitiated", // Uninitiated | Initiated | Healthy_RC | Healthy_Autonomous | Unhealthy | Completed | Exited
        "Track": {
            "State": "Red", // Red | Yellow | Orange | Green | Black | White | Chequered
            "Sectors": [ "Red", "Red"]
        },
        "Karts": [
            {
                "Team": "UCSD",
                "Status": "Connected", // Connected | Disconnected
                "ControlMode": "RC" // RC | Autonomous | Both
            }
        ]
    }
}
```

## /MQTT

Purpose is to provide live MQTT message feed in parallel with the backend storing messages. The Web App listens to this socket only.

#### Track Message
```json
{
    "topic": "track",
    "timestamp": "2020-05-06T22:12:00.000000+0100",
    "payload": {
        "AllowedControl": "RC", // RC | Autonomous
        "State": "Red", // Red | Yellow | Orange | Green | Black | White | Chequered
        "GPS": ""
    }
}
```

#### Kart Message
```json
{
    "topic": "kart",
    "timestamp": "2020-05-06T22:12:00.000000+0100",
    "payload": {
        "Control": "RC", // RC | Autonomous
        "Health": "Connected", // Connected | Disconnected
        "Speed": 3.2, // m/s
        "Steering": 0.2 // -1 to 1 (full left -> full right)
    }
}
```