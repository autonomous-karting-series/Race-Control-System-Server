const mqtt = require('mqtt');
const StatusSocket = require('./websockets/Status');
const MQTTSocket = require('./websockets/MQTT');
const moment = require('moment-timezone');

class MqttHandler {
  constructor() {
    if (!MqttHandler.instance) {
        this.mqttClient = null;
        this.host = 'mqtt://localhost';
        this.username = ''; // mqtt credentials if these are needed to connect
        this.password = '';

        MqttHandler.instance = this;
    }

    return MqttHandler.instance;
  }

  connect() {

    if (this.mqttClient && this.mqttClient.connected) {
      console.log('MQTT Instance already exists and is connected...');
      console.log('  ...Doing nothing.');
      return;
    }
    else if (this.mqttClient && !this.mqttClient.connected) {
      console.log('MQTT Instance already exists but is not connected...');
      console.log('  ...Connecting now.');
    }
    else {
      console.log('MQTT Client not created yet...');
      console.log('  ...Creating and connecting now.');
    }

    // Connect mqtt with credentials (in case of needed, otherwise we can omit 2nd param)
    this.mqttClient = mqtt.connect(this.host, { username: this.username, password: this.password });

    // Mqtt error calback
    this.mqttClient.on('error', (err) => {
      console.log(err);
      this.mqttClient.end();
    });

    // Connection callback
    this.mqttClient.on('connect', () => {
      console.log(`mqtt client connected`);
      StatusSocket.update_mqtt(true, this.host, this.port);
    });

    // mqtt subscriptions
    this.mqttClient.subscribe('#', {qos: 0});

    // When a message arrives, console.log it
    this.mqttClient.on('message', function (topic, message) {
        console.log(message.toString());
        MQTTSocket.broadcast({
            'topic': topic,
            'payload': message.toString(),
            'timestamp': moment().tz("America/New_York").format()
        });
    });

    this.mqttClient.on('close', () => {
        console.log(`mqtt client disconnected`);
        StatusSocket.update_mqtt(false);
    });
  }

  disconnect() {
    this.mqttClient.end(true, (res) => {
      console.log(`Closed MQTT`);
    });
  }

  // Sends a mqtt message to topic: mytopic
  sendMessage(message) {
    this.mqttClient.publish('mytopic', message);
  }
}

module.exports = new MqttHandler();