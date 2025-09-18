const createError = require('http-errors');
const debug = require('debug')('rcs_express:server');

const express = require('express');
const http = require('http');
const url = require('url');
const WebSocket = require('ws');
const cors = require('cors');

const StatusSocket = require('./websockets/Status');
const MQTTSocket = require('./websockets/MQTT');

var indexRouter = require('./routes/index');

//#region Express Configure
const app = express();

app.use(cors());
app.use('/', indexRouter);

const port = normalizePort(process.env.PORT || '9000');
app.set('port', port);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  console.log(`error: ${err.message}`);

  // render the error page
  res.status(err.status || 500).send();
  // res.render('error');
});

//#endregion

//#region Server Launch
const server = http.createServer(app);

/**
 * Listen on provided port, on all network interfaces.
 */
server.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});
server.on('error', onError);
server.on('listening', onListening);

/**
 * Normalize a port into a number, string, or false.
 */
function normalizePort(val) {
  var port = parseInt(val, 10);

  if (isNaN(port)) {
    // named pipe
    return val;
  }

  if (port >= 0) {
    // port number
    return port;
  }

  return false;
}

/**
 * Event listener for HTTP server "error" event.
 */
function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }

  var bind = typeof port === 'string'
    ? 'Pipe ' + port
    : 'Port ' + port;

  // handle specific listen errors with friendly messages
  switch (error.code) {
    case 'EACCES':
      console.error(bind + ' requires elevated privileges');
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(bind + ' is already in use');
      process.exit(1);
      break;
    default:
      throw error;
  }
}

/**
 * Event listener for HTTP server "listening" event.
 */
function onListening() {
  var addr = server.address();
  var bind = typeof addr === 'string'
    ? 'pipe ' + addr
    : 'port ' + addr.port;
  debug('Listening on ' + bind);
}

//#endregion

//#region Websocket Configs

/**
 * Status Websocket broadcasts messages to the web dashboard using the information from the Control Websocket
 *
 * Potential room for error checking to ensure MQTT and Control are aligned.
 */
// const status_socket = new WebSocket.Server({noServer: true});
// status_socket.on('connection', (socket) => {
//   console.log('Connected to Status server');

//   // socket.on('message', (message) => {
//   //   console.log('Received:', message.toString());
//   //   socket.send(`Server received: ${message}`);
//   // });

//   socket.on('close', () => {
//     console.log('Client disconnected');
//   });
// });

/**
 * MQTT Websocket broadcasts messages once that are received from the broker to the web dashboard.
 *
 * MQTT Messages are logged to the database when received from the broker asynchronously.
 */
// const mqtt_socket = new WebSocket.Server({noServer: true});
// mqtt_socket.on('connection', (client) => {
//   console.log('Connected to MQTT server');

//   client.on('message', (message) => {
//     console.log('Received:', message.toString());
//     // client.send(`Server received: ${message}`);

//     mqtt_socket.clients.forEach((client) => {
//       if (client !== mqtt_socket && client.readyState === WebSocket.OPEN) {
//         client.send(message.toString());
//       }
//     });
//   });

//   client.on('close', () => {
//     console.log('Client disconnected');
//   });
// });

/**
 * Control Websocket handles the communications with the RCS State Machine and instructs state changes.
 */
const control_socket = new WebSocket.Server({noServer: true});

/**
 * Using the HTTP port already in use for the websockets.
 *
 * Upgrade handles the paths for the different sockets according to the WS BKM.
 * https://github.com/websockets/ws?tab=readme-ov-file#multiple-servers-sharing-a-single-https-server
 */
server.on('upgrade', (request, socket, head) => {
  const pathname = url.parse(request.url).pathname;

  switch (pathname) {
    case '/cotrol':
      control_socket.handleUpgrade(request, socket, head, socket => {
        control_socket.emit('connection', socket, request);
      });
      break;

    case StatusSocket.path:
      StatusSocket.socket.handleUpgrade(request, socket, head, socket => {
        StatusSocket.socket.emit('connection', socket, request);
      });
      break;


    case MQTTSocket.path:
      MQTTSocket.socket.handleUpgrade(request, socket, head, socket => {
        MQTTSocket.socket.emit('connection', socket, request);
      });
      break;

    default:
      socket.destroy();
      break;
  }
});

//#endregion