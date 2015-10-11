var env = process.env;
var join = require('path').join;
var fs = require('fs');
var express = require('express');

var config = require('./config.js');

var port = config.port;

var paths = {
	statics: join(__dirname, '../client'),
	html: join(__dirname, '../client/html'),
	home: join(__dirname, '../client/html/index.html')
}

console.log(JSON.stringify(paths));

var app = express();

app.set('port', port);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.session({ secret: 'yourHIr3bn3rnInr32b023rLfm3rnFNnnIBGgf' }));
app.use(app.router);

app.use(express.static(paths.statics));
app.use(express.static(paths.html));
app.use('/', express.static(paths.home));

app.get('/api/1.0/*', function (req, res) {
  var path = req.path;
  var query = req.query;
});

app.post('/api/1.0/*', function (req, res) {
  var data = req.body;
});

app.put('/api/1.0/*', function (req, res) {
  var data = req.body;
});

app.delete('/api/1.0/*', function (req, res) {
  var path = req.path;
});

require('http')
	.createServer(app)
	.listen(
		app.get('port'),
		 function(){
		  console.log('Express server listening on port ' + app.get('port'));
		}
	);
