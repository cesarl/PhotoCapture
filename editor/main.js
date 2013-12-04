global.$ = $;

var fs = require('fs')
var exec = require('child_process').exec;

$(document).ready(function() {

    fs.readFile('./config.json', 'utf8', function (err,data) {
	if (err) {
	    return console.log(err);
	}
	data = JSON.parse(data);
	js2form(document.getElementById('form'), data);
    });


    $(".save").click(function(e){
		e.preventDefault();
		var j = form2js("form", "." , false, null, true);
		fs.writeFile('./../CAPTURE/config.json', JSON.stringify(j, null, '\t'), function (err) {
	    	if (err) return console.log(err);
		});
		fs.writeFile('./config.json', JSON.stringify(j, null, '\t'), function (err) {
	   		if (err) return console.log(err);
		});
    });

    $(".save-and-launch").click(function(e){
		e.preventDefault();
		var j = form2js("form", "." , false, null, true);
		fs.writeFile('./../CAPTURE/config.json', JSON.stringify(j, null, '\t'), function (err) {
	    	if (err) return console.log(err);
		});
		fs.writeFile('./config.json', JSON.stringify(j, null, '\t'), function (err) {
	   		if (err) return console.log(err);
		});
		exec('sh ../CAPTURE/launch', function callback(error, stdout, stderr){
    	// result
		});
    });

    $(".launch").click(function(e){
		e.preventDefault();
		exec('sh ../CAPTURE/launch', function callback(error, stdout, stderr){
    	// result
		});
    });
});
