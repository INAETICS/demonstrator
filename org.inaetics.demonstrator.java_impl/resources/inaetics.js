/*! ineatics.js - INAETICS specific stuff */

function getStats(url) {
	qwest.get(url, {}, { 'async': true })
		.then(function(response) {
			renderStats(response);
		})
		.catch(function(response) {
			console.log("Failed to get provider for " + url);
			console.log(response);
		})
}

function renderStats(stats) {
	var width = 400;
	var height = 250;

	var id = "st-" + stats.name.replace(/\W+/g, '');

	var el = $("#stats-container #" + id)
	if (el.length == 0) {
		$("#stats-container").append("<div id='" + id + "'></div>")
	} else {
		$("#stats-container #" + id + " svg").remove();
	}
	
	var valuesx = stats.timestamps;
	var valuesy = stats.values;
	// ensure we always start with zero...
	valuesx.unshift(valuesx[0] - 1);
	valuesy.unshift(0);

	var r = Raphael(id, 100 + width, 50 + height);
	r.text(160, 10, stats.name + " " + stats.type + " " + stats.unit).attr({'font-size': '12pt', 'font-family': '"Helvetica Neue", "Helvetica", sans-serif'});
	r.linechart(100, 10, width - 100, height - 10, valuesx, valuesy, {'axis': '0 0 1 1', 'axisxstep': 5, 'axisystep': 5, 'gutter': 10});
}

function renderAllStats() {
	qwest.get("/stats")
		.then(function(response) {
			$.each(response, function(idx, val) {
				getStats(val);
			});
		})
		.catch(function(response) { 
			console.log("Failed to get provider names!"); 
		})
		.complete(function(response) {
			// Reschedule a recurrent call...
			setTimeout(renderAllStats, 2000 /* ms */);
		});
}

$(document).ready(function() {
	// install window timeout
	setTimeout(renderAllStats, 2000 /* ms */);
});
