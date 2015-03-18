/* Add npm dependencies */

var app  = require('express')()
   , server = require('http').createServer(app)
   , io = require('socket.io').listen(server)
   ,client =require("redis").createClient()
   , $ = require('jquery');
var spawn = require('child_process').spawn;

/* reduce debug messages to level 1 */
io.set('log level', 1);  

/* Server setup */
server.listen(80);

/* sockets.io setup */
io.sockets.on('connection', function(socket) {

    socket.emit('conStatus', 'Connected Message');

    socket.on('clearCommands', function(data) {
	console.log("Cleared the NavCommands");
	client.del('NavCommands');
    });
    socket.on('triggerCPPDroneApp', function(data) {
        console.log("Fly Drone App");

        var child = spawn('./Build/DroneIT', '');

        child.stdout.on('data', function(chunk) {
            var message = chunk.toString('ascii');            
            console.log('Data from CPP',message);
            
            var splitResponse = message.split("\n");
            for (var i = 0; i < splitResponse.length; i++) {
                if (splitResponse[i].search("DroneIT") != -1) {
                    var droneResponse = splitResponse[i].substr(splitResponse[i].indexOf("::") + 1);
                    var droneResponseLen = droneResponse.length;
                    if (droneResponse != -1 && droneResponseLen == 14) {
                        console.log("call OCR Script");
			//./shellscript/doPull.sh
                    }
                    socket.emit('DroneStatus', droneResponse);
                }
            }

        });

        child.on('exit', function(code) {
            console.log('exit Cpp Program');
        });
    });

    socket.on('triggerFlightPlan1', function(data) {
        console.log("Fly Flight Plan 1");


        var splitFlightPlan1 = data.split("\n");
        for (var i = 0; i < splitFlightPlan1.length; i++) {
            client.lpush('NavCommands', splitFlightPlan1[i]);
        }

        var child = spawn('./Build/DroneIT', '');

        child.stdout.on('data', function(chunk) {
            //socket.emit('DroneStatus', chunk.toString('ascii'));
            var message = chunk.toString('ascii');
            console.log('Data from CPP FP1',message);
            var splitResponse = message.split("\n");
            for (var i = 0; i < splitResponse.length; i++) {
                if (splitResponse[i].search("DroneIT") != -1) {
                    var droneResponse = splitResponse[i].substr(splitResponse[i].indexOf("::") + 1)
                    var droneResponseLen = droneResponse.length;
                    console.log(droneResponse,droneResponse.length);
                    if (droneResponse != -1 && droneResponseLen == 15) {
                        console.log("callingOCR");
			var child2 = spawn('./shellscript/doPull.sh', '');
			child2.stdout.on('data', function(chunk) {
			    //socket.emit('DroneStatus', chunk.toString('ascii'));
			    var message2 = chunk.toString('ascii');
			    console.log('Data from OCR',message2);
			    var splitResponse2 = message2.split("\n");
			    for (var i = 0; i < splitResponse2.length; i++) {
				if (splitResponse2[i].search("DroneIT") != -1) {
				    var droneResponse2 = splitResponse2[i].substr(splitResponse2[i].indexOf("::") + 1)
				    socket.emit('DroneStatus', droneResponse2);
				}
			    }
			});

			child2.on('exit', function(code) {
			    console.log('exit OCR program');
			});

		 console.log("call OCR Script");
                    }
                    socket.emit('DroneStatus', droneResponse);
                }
            }
        });

        child.on('exit', function(code) {
            console.log('exit Cpp Program');
        });
    });



    socket.on('triggerFlightPlan2', function(data) {
        console.log("Fly Flight Plan 2");
	var child2 = spawn('./shellscript/doPull.sh', '');
			child2.stdout.on('data', function(chunk) {
			    //socket.emit('DroneStatus', chunk.toString('ascii'));
			    var message2 = chunk.toString('ascii');
			    console.log('Data from OCR FP2', message2);
			    var splitResponse2 = message2.split("\n");
			     console.log(splitResponse2)
			    for (var i = 0; i < splitResponse2.length; i++) {
				if (splitResponse2[i].search("DroneIT") != -1) {
				    var droneResponse2 = splitResponse2[i].substr(splitResponse2[i].indexOf("::") + 1)
				    socket.emit('DroneStatus', droneResponse2);
				}
			    }
			});

			child2.on('exit', function(code) {
			    console.log('exit OCR program');
			});

    });





    socket.on('overrideCommands', function(data) {
        console.log("Override Command :" + data);
        client.rpush('NavCommands', data);
    });
});
 

/* http portion of the server */
app.get('/', function (req, res) {
  res.sendfile(__dirname + '/index.html');
});


app.get('/socket.io.js', function(req, res){
	console.log("jquery js file call");
  res.sendfile(__dirname + '/socket.io.js');
});

app.get('/img/BebopDrone.jpg', function(req, res){
	console.log("image");
  res.sendfile('./img/BebopDrone.jpg');
});

app.get('/img/Intermodal2.jpg', function(req, res){
	console.log("image");
  res.sendfile('./img/Intermodal2.jpg');
});

app.get('/img/IntermodalTerminal3.png', function(req, res){
	console.log("image");
  res.sendfile('./img/IntermodalTerminal3.png');
});

app.get('/img/Intermodal4.jpg', function(req, res){
	console.log("image");
  res.sendfile('./img/Intermodal4.jpg');
});


app.get('/css/bootstrap.min.css', function(req, res){
	console.log("image");
  res.sendfile(__dirname + '/css/bootstrap.min.css');
});

app.get('/css/style.css', function(req, res){
	console.log("image");
  res.sendfile(__dirname + '/css/style.css');
});


app.get('/js/jquery-1.11.2.min.js', function(req, res){
	console.log("image");
  res.sendfile(__dirname + '/js/jquery-1.11.2.min.js');
});


app.get('/js/bootstrap.min.js', function(req, res){
	console.log("image");
  res.sendfile('./js/bootstrap.min.js');
});
app.get('/js/scripts.js', function(req, res){
	console.log("image");
  res.sendfile('./js/scripts.js');
});

app.get('/fonts/glyphicons-halflings-regular.eot', function(req, res){
	console.log("image");
  res.sendfile('./fonts/glyphicons-halflings-regular.eot');
});

app.get('/fonts/glyphicons-halflings-regular.svg', function(req, res){
	console.log("image");
  res.sendfile('./fonts/glyphicons-halflings-regular.svg');
});
app.get('/fonts/glyphicons-halflings-regular.ttf', function(req, res){
	console.log("image");
  res.sendfile('./fonts/glyphicons-halflings-regular.ttf');
});
app.get('/fonts/glyphicons-halflings-regular.woff', function(req, res){
	console.log("image");
  res.sendfile('./fonts/glyphicons-halflings-regular.woff');
});console.log('End of Server nodejs DroneApp');




