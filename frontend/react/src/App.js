import "./App.css";
import React, { Component } from "react";
import {
  Container,
  Row,
  Col,
  Card,
  Button,
  Alert,
  ListGroup,
} from "react-bootstrap";
import { LuRefreshCcw } from "react-icons/lu";

import MQTTFeed from "./components/MQTTFeed";
import TrackStatus from "./components/TrackStatus";
import RCSControls from "./components/RCSControls";
// import Toast from 'react-bootstrap/Toast';

class App extends Component {
  constructor(props) {
    super(props);

    this.mqtt_socket = null;
    this.status_socket = null;

    this.state = {
      socket_conn: {
        mqtt: 0,
        status: 0,
      },
      status: {
        mqtt: {
          connected: false,
        },
        rcs: {
          connected: false,
        },
      },
      messages: [],
    };

    console.log(this.state);

    this.startStatusSocket = this.startStatusSocket.bind(this);
    this.startMQTTSocket = this.startMQTTSocket.bind(this);
  }

  componentDidMount() {
    this.startStatusSocket();
    this.startMQTTSocket();
  }

  componentWillUnmount() {
    this.status_socket.close();
    this.mqtt_socket.close();
  }

  startMQTTSocket() {
    const mqtt_socket_url = `ws://${window.location.hostname}:9000/mqtt`;
    this.mqtt_socket = new WebSocket(mqtt_socket_url);

    this.mqtt_socket.onopen = () => {
      console.log("MQTT Socket Connected");
      this.setState((prev) => ({
        socket_conn: {
          mqtt: this.mqtt_socket.readyState,
          status: prev.socket_conn.status,
        },
        messages: prev.messages,
        status: prev.status,
      }));
    };

    this.mqtt_socket.onopen = () => {
      console.log("MQTT Socket Connected");
      this.setState((prev) => ({
        socket_conn: {
          mqtt: this.mqtt_socket.readyState,
          status: prev.socket_conn.status,
        },
        messages: prev.messages,
        status: prev.status,
      }));
    };

    this.mqtt_socket.onmessage = (msg) => {
      console.log(msg);
      const message = JSON.parse(msg.data);

      this.setState((prev) => ({
        socket_conn: prev.socket_conn,
        messages: [
          ...prev.messages,
          {
            timestamp: message.timestamp,
            topic: message.topic,
            payload: message.payload,
          },
        ],
        status: prev.status,
      }));
    };

    this.mqtt_socket.onclose = () => {
      console.log("MQTT Socket Disconnected");
      this.setState((prev) => ({
        socket_conn: {
          mqtt: this.mqtt_socket.readyState,
          status: prev.socket_conn.status,
        },
        messages: prev.messages,
        status: prev.status,
      }));
    };

    this.mqtt_socket.onerror = () => {
      this.mqtt_socket.close();
      this.mqtt_socket = new WebSocket(mqtt_socket_url);
    };
  }

  startStatusSocket = () => {
    const status_socket_url = `ws://${window.location.hostname}:9000/status`;
    this.status_socket = new WebSocket(status_socket_url);

    // Set connection status
    this.status_socket.onopen = () => {
      console.log("Status Socket Connected");
      this.setState((prev) => ({
        socket_conn: {
          mqtt: prev.socket_conn.mqtt,
          status: this.status_socket.readyState,
        },
        messages: prev.messages,
        status: prev.status,
      }));
    };

    this.status_socket.onmessage = (msg) => {
      const message = JSON.parse(msg.data);

      this.setState((prev) => ({
        socket_conn: prev.socket_conn,
        messages: prev.messages,
        status: message,
      }));
    };

    this.status_socket.onclose = () => {
      console.log("Status Socket Disconnected");
      this.setState((prev) => ({
        socket_conn: {
          mqtt: prev.socket_conn.mqtt,
          status: this.status_socket.readyState,
        },
        messages: prev.messages,
        status: prev.status,
      }));
    };

    this.status_socket.onerror = () => {
      this.status_socket.close();
      this.status_socket = new WebSocket(status_socket_url);
    };
  };

  render() {
    return (
      <Container
        fluid
        style={{ height: "100vh", padding: "1vh", overflow: "hidden" }}
      >
        <Row style={{ height: "48vh", margin: "1vh" }}>
          {/* Status Cards */}
          <Col
            className="h-100"
            style={{
              width: "20%",
              maxWidth: "20%",
              marginRight: "calc(var(--bs-gutter-x) * .5)",
            }}
          >
            <Row style={{ height: "49%", maxHeight: "49%" }}>
              <Card className="h-100" style={{ padding: "0" }}>
                <Card.Header>Race Control System</Card.Header>
                <Card.Body style={{ flex: "1 1 auto", overflow: "auto" }}>
                  <ListGroup variant="flush">
                    <ListGroup.Item
                      key="mqtt"
                      as="li"
                      className="status-item"
                      style={{ width: "100%" }}
                    >
                      {this.state.socket_conn.mqtt === 1 ? (
                        <div className="connection-indicator status-online" />
                      ) : this.state.socket_conn.mqtt === 0 ? (
                        <div className="connection-indicator status-connecting" />
                      ) : (
                        <div className="connection-indicator status-offline" />
                      )}
                      <div className="flex-fill">MQTT Websocket</div>
                      <Button
                        variant="outline-primary"
                        onClick={() => {
                          this.mqtt_socket.close();
                          this.startMQTTSocket();
                        }}
                      >
                        <LuRefreshCcw />
                      </Button>
                    </ListGroup.Item>
                    <ListGroup.Item
                      key="status"
                      as="li"
                      className="status-item"
                    >
                      {this.state.socket_conn.status === 1 ? (
                        <div className="connection-indicator status-online" />
                      ) : this.state.socket_conn.status === 0 ? (
                        <div className="connection-indicator status-connecting" />
                      ) : (
                        <div className="connection-indicator status-offline" />
                      )}
                      <div className="flex-fill">Status Websocket</div>
                      <Button
                        variant="outline-primary"
                        onClick={() => {
                          this.status_socket.close();
                          this.startStatusSocket();
                        }}
                      >
                        <LuRefreshCcw />
                      </Button>
                    </ListGroup.Item>
                  </ListGroup>
                </Card.Body>
              </Card>
            </Row>
            <Row style={{ height: "49%", maxHeight: "49%", marginTop: "1vh" }}>
              <Card className="h-100" style={{ padding: "0" }}>
                <Card.Header>MQTT Config</Card.Header>
                <Card.Body style={{ flex: "1 1 auto", overflow: "auto" }}>
                  {this.state.status.mqtt.connected ? (
                    // MQTT is connected
                    <div className="gap-2 d-md-flex">
                      <Button variant="outline-primary" disabled>
                        Connect
                      </Button>
                      <Button
                        variant="outline-danger"
                        active
                        onClick={() => {
                          fetch(
                            `http://${window.location.hostname}:9000/disconnect_mqtt`
                          );
                        }}
                      >
                        Disconnect
                      </Button>
                    </div>
                  ) : (
                    // MQTT is not connected
                    <div className="gap-2 d-md-flex">
                      <Button
                        variant="outline-primary"
                        active
                        onClick={() => {
                          fetch(
                            `http://${window.location.hostname}:9000/connect_mqtt`
                          );
                        }}
                      >
                        Connect
                      </Button>
                      <Button variant="outline-danger" disabled>
                        Disconnect
                      </Button>
                    </div>
                  )}
                </Card.Body>
              </Card>
            </Row>
          </Col>

          {/* Track Card */}
          <Col className="h-100" style={{ width: "30%", maxWidth: "30%" }}>
            <Card className="h-100">
              <Card.Header>Track Status</Card.Header>
              {this.state.status.rcs.connected ? (
                <TrackStatus track={this.state.status.rcs.track} />
              ) : (
                <Card.Body>
                  <Alert variant="danger">
                    RCS is not connected. Cannot display Track info.
                  </Alert>
                </Card.Body>
              )}
            </Card>
          </Col>

          {/* Kart Card */}
          <Col className="h-100" style={{ width: "30%", maxWidth: "30%" }}>
            <Card className="h-100">
              <Card.Header>Kart Status</Card.Header>
              <Card.Body>
                {this.state.status.rcs.connected ? (
                  <Alert variant="secondary">
                    RCS is connected. Not Implemented Yet
                  </Alert>
                ) : (
                  <Alert variant="danger">
                    RCS is not connected. Cannot display Kart info.
                  </Alert>
                )}
              </Card.Body>
            </Card>
          </Col>

          {/* RCS Controls */}
          <Col className="h-100" style={{ width: "30%", maxWidth: "30%" }}>
            <Card className="h-100">
              <Card.Header>RCS Controls</Card.Header>

              {this.state.status.rcs.connected ? (
                <RCSControls rcs={this.state.status.rcs} />
              ) : (
                <Card.Body>
                  <Alert variant="danger">
                    RCS is not connected. Cannot display RCS info.
                  </Alert>
                </Card.Body>
              )}
            </Card>
          </Col>
        </Row>
        <Row style={{ height: "48vh", margin: "1vh" }}>
          <Col className="h-100" style={{ paddingLeft: "0" }}>
            <Card className="h-100">
              <Card.Header>MQTT Live Feed</Card.Header>
              <MQTTFeed messages={this.state.messages} />
            </Card>
          </Col>
        </Row>
      </Container>
    );
  }
}

export default App;
