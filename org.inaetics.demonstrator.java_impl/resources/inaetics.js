/*! ineatics.js - INAETICS specific stuff */
"use strict";

var interval = 1000; // ms

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
		responsive: true,
		animation: false,
		responsiveMinWidth: chartWidth,
		responsiveMaxWidth: chartWidth,
		responsiveMinHeight: chartHeight,
		responsiveMaxHeight: chartHeight,
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
		opts.scaleSteps = 10
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

function renderStats(e) {
	if (this.status != 200) {
		console.log("failed to obtain statistics!");
		return;
	}

	var total = this.response.length

	// wait until we at least 2 datasets, else ChartNew.js breaks
	if (total < 2) return;

	var container = document.querySelector('#stats-container')
	var charts = [].slice.call(container.children)

	for (var i = 0; i < total; i++) {
		var name = this.response[i].name
		var data = getData(this.response[i])
		var opts = getChartOpts(this.response[i])

		var statCanvas = null
		for (var j = 0, len = charts.length; j < len; j++) {
			if (charts[j].id == name) {
				statCanvas = charts.splice(j, 1)[0];
				break;
			}
		}
		
		if (!statCanvas) {
			statCanvas = document.createElement("canvas");
			statCanvas.id = name;
			statCanvas.setAttribute("width", chartWidth);
			statCanvas.setAttribute("height", chartHeight)

			container.appendChild(statCanvas);
			
			opts.animation = true;
		}

		// Create our chart context...
		var chartCtx = statCanvas.getContext('2d');

		if (opts.animation) {
			// New charts one...
			var chart = new Chart(chartCtx);
			chart.Line(data, opts);
		} else {
			updateChart(chartCtx, data, opts, false /* animation */, false /* runanimationcompletefunction */);
		}
	}

	// Remove old graphs...
	for (var i = 0, len = charts.length; i < len; i++) {
		container.removeChild(charts[i])
	}
	
	// Reschedule a recurrent call...
	setTimeout(getAndRenderStats, interval);
}

function getAndRenderStats() {
	var xhr = new XMLHttpRequest();
	xhr.open('GET', '/stats', true);
	xhr.responseType = 'json';
	xhr.onload = renderStats;
	xhr.send();
}

window.onload = function() {
	// install window timeout
	setTimeout(getAndRenderStats, interval);
};
