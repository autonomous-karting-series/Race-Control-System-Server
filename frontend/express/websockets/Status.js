
const WebSocket = require('ws');

class StatusSocket {

    constructor() {
        if (!StatusSocket.instance) {
            this.path = '/status';
            this.status = {
                mqtt: {
                    connected: false
                },
                rcs: {
                    connected: false
                }
            };

            this.socket = new WebSocket.Server({noServer: true});
            this.socket.on('connection', (socket) => {
                console.log('Connected to Status server');
                socket.send(JSON.stringify(this.status));

                // Debug only... disable for prod
                socket.on('message', (message) => {
                    console.log('Received:', message.toString());
                    this.status = JSON.parse(message.toString())
                    this.#broadcast();
                });

                socket.on('close', () => {
                    console.log('Client disconnected');
                });
            });

            StatusSocket.instance = this;
        }

        return StatusSocket.instance;
    }

    #broadcast() {
        this.socket.clients.forEach((client) => {
            if (client !== this.socket && client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(this.status));
            }
        });
    }

    update_mqtt(is_connected, host = null, port = null) {
        if (is_connected) {
            this.status.mqtt = {
                connected: is_connected,
                host: host,
                port: port
            }
        } else {
            this.status.mqtt = {
                connected: false
            }
        }

        this.#broadcast();
    }

    // get_socket() {
    //     return this.socket;
    // }
}

module.exports = new StatusSocket();