var express = require('express');
var router = express.Router();
const mqttHandler = require('../MQTTHandler');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.send();
});

router.get('/config', function(req, res, next) {} )

router.get('/connect_mqtt', function(req, res, next) {
  console.log('connect MQTT');

  mqttHandler.connect();
  res.status(200).send();
})


router.get('/disconnect_mqtt', function(req, res, next) {
  console.log('disconnect MQTT');

  mqttHandler.disconnect();
  res.status(200).send();
})

module.exports = router;
