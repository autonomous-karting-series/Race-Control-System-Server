
const WebSocket = require('ws');

class MQTTSocket {

    constructor() {
        if (!MQTTSocket.instance) {
            this.path = '/mqtt';
            // this.message = {
            //     'topic': '',
            //     'payload': '',
            //     'timestamp': ''
            // };

            this.socket = new WebSocket.Server({noServer: true});
            this.socket.on('connection', (socket) => {
                console.log('Connected to MQTT server');

                socket.on('close', () => {
                    console.log('Client disconnected');
                });
            });

            MQTTSocket.instance = this;
        }

        return MQTTSocket.instance;
    }

    broadcast(message) {
        this.socket.clients.forEach((client) => {
            if (client !== this.socket && client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(message));
            }
        });
    }

    // update_mqtt(is_connected, host = null, port = null) {
    //     if (is_connected) {
    //         this.status['MQTT'] = {
    //             'Connected': is_connected,
    //             'Host': host,
    //             'Port': port
    //         }
    //     } else {
    //         this.status['MQTT'] = {
    //             'Connected': false
    //         }
    //     }

    //     this.#broadcast();
    // }

    // get_socket() {
    //     return this.socket;
    // }
}

module.exports = new MQTTSocket();