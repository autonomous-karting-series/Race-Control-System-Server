import React, { useState, useEffect, useRef } from 'react';
import { User, Plus, Edit2, Trash2, Save, X, Wifi, WifiOff, Activity } from 'lucide-react';

const API_BASE_URL = 'http://localhost:9000/api';
const WEBSOCKET_URL = 'http://localhost:9000';

// Simple WebSocket client (in production, you'd use socket.io-client)
class SimpleWebSocket {
  constructor(url) {
    this.url = url;
    this.socket = null;
    this.connected = false;
    this.listeners = new Map();
  }

  connect() {
    return new Promise((resolve, reject) => {
      try {
        // Simulate Socket.IO connection
        this.socket = new WebSocket(this.url.replace('http', 'ws') + '/socket.io/?EIO=4&transport=websocket');

        this.socket.onopen = () => {
          this.connected = true;
          console.log('WebSocket connected');
          resolve();
        };

        this.socket.onmessage = (event) => {
          try {
            const data = JSON.parse(event.data);
            if (data.type && this.listeners.has(data.type)) {
              this.listeners.get(data.type).forEach(callback => callback(data.payload));
            }
          } catch (e) {
            console.error('Error parsing WebSocket message:', e);
          }
        };

        this.socket.onclose = () => {
          this.connected = false;
          console.log('WebSocket disconnected');
        };

        this.socket.onerror = (error) => {
          this.connected = false;
          console.error('WebSocket error:', error);
          reject(error);
        };
      } catch (error) {
        reject(error);
      }
    });
  }

  emit(event, data) {
    if (this.connected && this.socket.readyState === WebSocket.OPEN) {
      this.socket.send(JSON.stringify({ type: event, payload: data }));
    }
  }

  on(event, callback) {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, []);
    }
    this.listeners.get(event).push(callback);
  }

  disconnect() {
    if (this.socket) {
      this.socket.close();
    }
  }
}