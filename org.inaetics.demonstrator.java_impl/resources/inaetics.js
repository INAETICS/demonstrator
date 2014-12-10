/*! ineatics.js - INAETICS specific stuff */
"use strict";

var interval = 5000; // ms

var chartNames = [];


function fmtChartJSPerso(config, value, fmt) {
	if (fmt == "DateTime") {
		return new Date(value).toLocaleTimeString('nl-NL', { hour12: false })
	}
	return value;
}

function getChartOpts(stats) {
	return { 
		canvasBorders: false,
		inGraphDataShow: false,
		annotateDisplay : false,
		responsive: false,
	    graphTitleFontSize: 16,
		scaleBeginAtZero: true, 
		pointDot: false, 
		bezierCurve: false,
		rotateLabels: 90,
		datasetFill: true,
		yAxisUnitFontSize: 12,
		yAxisLabel: stats.unit,
		fmtXLabel: "DateTime",
		graphTitle: stats.displayName + " " + stats.type
	};
}

function getData(stats) {
	return { 
		labels: stats.timestamps, 
		datasets: [ {
			fillColor: "rgba(151,187,205,0.5)",
	        strokeColor: "rgba(151,187,205,1)",
			data: stats.values
		} ] 
	}
}

function renderAllStats() {
	$.get("/stats")
		.done(function(response) {
			var total = response.length
			
			$.each(response, function(idx, val) {
				$.ajax({ url: val.url, async: false })
					.done(function(response) {
						var data = getData(response)
						var opts = getChartOpts(response)
						var id = val.name

						var el = $("#stats-container #" + id)
						if (el.length == 0) {
							$("#stats-container").append("<canvas id='" + id + "' width='" + 500 + "' height='" + 400 + "'></canvas>")
						}

						// Create our chart context...
						var chartCtx = document.getElementById(id).getContext('2d');
						var chart = new Chart(chartCtx);

						if (chartNames.indexOf(val.name) < 0) {
							opts.animate = true
	
							chart.Line(data, opts, chart);
							chartNames.push(val.name);
						} else {
							updateChart(chartCtx, data, opts, false /* animation */, false /* runanimationcompletefunction */);
						}
					})
					.fail(function(response) {
						console.log("Failed to get provider for " + val.url);
						console.log(response);
					})
			})
			
			// Reschedule a recurrent call...
			setTimeout(renderAllStats, interval);
		})
		.fail(function(response) { 
			console.log("Failed to get provider names!"); 
			// Reschedule a recurrent call...
			setTimeout(renderAllStats, interval);
		})
}

$(document).ready(function() {
	// install window timeout
	setTimeout(renderAllStats, 500);
});
