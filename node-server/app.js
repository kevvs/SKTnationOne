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
var bodyParser = require('body-parser');
app.use(bodyParser.json({limit: '50mb', parameterLimit: 10000}));
app.use(bodyParser.urlencoded({limit: '50mb', extended: true, parameterLimit: 10000}));
app.use(express.session({secret: 'nFNnnIBGgf'}));
app.use(app.router);

app.use(express.static(paths.statics));
app.use(express.static(paths.html));
app.use('/', express.static(paths.home));

var baseTask = {
  status: "edited", // started, finished, failed, saving
  started: null,
  finished: null,
  input: {
    json: null,
    "converted-string": null
  },
  output: null,
  bx_calculated: null,
  bx_analytical: null,
  console : {
    command: "a.exe input.txt output.txt",
    stdout: null,
    stderr: null,
    exec_error: null
  }
};

var task = baseTask;

var inputPath = "..\\console-application\\input.txt";
var outputPath = "..\\console-application\\output.txt";
var bxCPath = "..\\console-application\\bx_calculated.txt";
var bxAPath = "..\\console-application\\bx_analytical.txt";

app.post('/api/1.0/task', function (req, res) {
  task = baseTask; // reset server state
  var data = req.body;
  console.log(data);
  task.status = "saving";
  task.started = new Date();
  task.input.json = data;
  var input = convert.convertDataToString(data);
  task.input["converted-string"] = input;
  fs.writeFileSync(inputPath, input);
  task.status = "started";
  exec("cd ..\\console-application && " + task.console.command, function (err, so, se) {
    console.log('stdout: ' + so); task.console.stdout = so.split("\r\n");
    console.log('stderr: ' + se); task.console.stderr = se.split("\r\n");
    if (err !== null) {
      console.log('exec error: ' + err);
      task.status = "failed";
      task.console.exec_error = err.split("\r\n");
      return;
    }
    function stringToArrArr(str, must_split_in) {
        str = str.split("\r\n");
        str.pop();
        for(var i in str) {
            str[i] = str[i].split("\t");
            if(must_split_in) str[i].pop();
        }
        return str;
    }
    var output = fs.readFileSync(outputPath).toString();
    var bxc = fs.readFileSync(bxCPath).toString();
    var bxa = fs.readFileSync(bxAPath).toString();
    task.output = stringToArrArr(output, true);
    task.bx_calculated = stringToArrArr(bxc, false);
    task.bx_analytical = stringToArrArr(bxa, false);
    task.finished = new Date();
    task.status = "finished";
  });
  res.status(204).send();
});

app.get('/api/1.0/task', function (req, res) {
  res.send(JSON.stringify(task));
});

app.get('/api/1.0/raml', function (req, res) {
  fs.readFile('./api.raml', function (err, data) {
    if (err) throw err;
    res.type('text/plain').send(data);
  });

});

http
  .createServer(app)
  .listen(
    app.get('port'),
    function () {
      console.log('Express server listening on port ' + app.get('port'));
    }
  );
