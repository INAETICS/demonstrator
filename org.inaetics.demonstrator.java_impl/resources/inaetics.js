/*! ineatics.js - INAETICS specific stuff */
"use strict";

var interval = 500; // ms

var chartWidth = 450;
var chartHeight = 350;

function fmtChartJSPerso(config, value, fmt) {
	if (fmt == "DateTime") {
		return new Date(value).toLocaleTimeString('nl-NL', { hour12: false })
	}
	return value;
}

function getChartOpts(stats) {
	var opts = { 
		canvasBorders: false,
		inGraphDataShow: false,
		responsive: false,
	    graphTitleFontSize: 16,
		pointDot: false, 
		bezierCurve: false,
		rotateLabels: 60,
		datasetFill: true,
		yAxisUnitFontSize: 14,
		yAxisLabel: stats.type,
		yAxisUnit: stats.unit,
		fmtXLabel: "DateTime",
		graphTitle: stats.displayName
	};
	if (stats.type == "utilization") {
		opts.scaleOverride = true
		opts.scaleStartValue = 0
		opts.scaleSteps = 5
		opts.scaleStepWidth = 10
	}
	return opts;
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
							$("#stats-container").append("<canvas id='" + id + "' width='" + chartWidth + "' height='" + chartHeight + "'></canvas>")
						}

						// Create our chart context...
						var chartCtx = document.getElementById(id).getContext('2d');

						if (el.length == 0) {
							opts.animation = true
	
							var chart = new Chart(chartCtx);
							chart.Line(data, opts);
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
