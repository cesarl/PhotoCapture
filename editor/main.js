global.$ = $;

var fs = require('fs')

$(document).ready(function() {

    fs.readFile('./config.json', 'utf8', function (err,data) {
	if (err) {
	    return console.log(err);
	}
	data = JSON.parse(data);
	js2form(document.getElementById('form'), data);
    });


    $(".btn-success").click(function(e){
	e.preventDefault();
	var j = form2js("form", "." , false, null, true);
	fs.writeFile('./config.json', JSON.stringify(j, null, '\t'), function (err) {
	    if (err) return console.log(err);
	});
    });
});
