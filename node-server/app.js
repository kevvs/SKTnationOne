var env = process.env;
var join = require('path').join;
var fs = require('fs');
var http = require('http');
var express = require('express');
var exec = require('child_process').exec;

var config = require('./config.js');
var convert = require('./convert.js');

var port = config.port;

var paths = {
  statics: join(__dirname, '../client'),
  html: join(__dirname, '../client/html'),
  home: join(__dirname, '../client/html/index.html')
};

console.log(JSON.stringify(paths));

var app = express();

app.set('port', port);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.session({secret: 'nFNnnIBGgf'}));
app.use(app.router);

app.use(express.static(paths.statics));
app.use(express.static(paths.html));
app.use('/', express.static(paths.home));

var task = {
  status: "edited", // started, finished, failed, saving
  started: null,
  finished: null,
  input: null,
  inputString: null,
  output: null,
  console : {
    command: "./a.exe input.txt output.txt"
  }
};

var inputPath = "../console-application/input.txt";
var outputPath = "../console-application/output.txt";

app.post('/api/1.0/task', function (req, res) {
  var data = req.body;
  task.status = "saving";
  task.started = new Date();
  task.input = data;
  var input = convert.convertDataToString(data);
  task.inputString = input;
  fs.writeFileSync(inputPath, input);
  task.status = "started";
  exec("cd ../console-application && ./a.exe input.txt output.txt", function (err, so, se) {
    console.log('stdout: ' + so);
    console.log('stderr: ' + se);
    if (error !== null) {
      console.log('exec error: ' + err);
      task.status = "failed";
      return;
    }
    task.output = fs.readFileSync(outputPath);
    task.finished = new Date();
    task.status = "finished";
  });
  res.status(204).send();
});

app.get('/api/1.0/task', function (req, res) {
  res.status(200).send(task);
});

http
  .createServer(app)
  .listen(
  app.get('port'),
  function () {
    console.log('Express server listening on port ' + app.get('port'));
  }
);
