const express = require("express");
const http = require("http");
const socketIo = require("socket.io");

const port = process.env.PORT || 4001;
const index = require("./routes/index");

const app = express();
app.use(index);

const server = http.createServer(app);

const io = socketIo(server);

let interval;


let lat = 1;
let long = 1;

io.on("connection", (socket) => {
  console.log("New client connected");
  if (interval) {
    clearInterval(interval);
  }
  interval = setInterval(() => getApiAndEmit(socket), 1000);
  socket.on("disconnect", () => {
    console.log("Client disconnected");
    clearInterval(interval);
  });
});

const getApiAndEmit = socket => {
  // const response = new Date();
  // Emitting a new message. Will be consumed by the client
  var fs = require('fs');
 
  try{
    var contents = fs.readFileSync('../../coordAktarim.json', 'utf8');

    var obj = JSON.parse(contents);

    lat = obj.lat;
    long = obj.long;
  }
  catch(e){
    console.log("ERR", e);
  }
  


  let response = {lat: lat, long: long};

  socket.emit("FromAPI", response);
};

server.listen(port, () => console.log(`Listening on port ${port}`));