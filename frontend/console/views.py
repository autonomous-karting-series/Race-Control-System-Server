from django.shortcuts import render

from django.http import HttpResponse
from RCS_ControlPanel import settings
import paho.mqtt.client as mqtt

client = mqtt.Client()

def connect():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)
    client.connect(
        host=settings.MQTT_SERVER,
        port=settings.MQTT_PORT,
        keepalive=settings.MQTT_KEEPALIVE
    )

def index(request):

    return HttpResponse("Hello, world. You're at the polls index.")

# Create your views here.

def on_connect(mqtt_client, userdata, flags, rc):
   if rc == 0:
       print('Connected successfully')
       mqtt_client.subscribe('track')
   else:
       print('Bad connection. Code:', rc)

def on_message(mqtt_client, userdata, msg):
   print(f'Received message on topic: {msg.topic} with payload: {msg.payload}')
