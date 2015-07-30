/*! ineaticsStats.js - functions for rendering statistics - Copyright (C) 2015 - INAETICS */
"use strict";

var interval = 1000; // ms

function renderStats(stats) {
	var total = stats.length

	var container = document.querySelector('#stats-container')
	var charts = [].slice.call(container.children)

	for (var i = 0; i < total; i++) {
		var name = stats[i].name
		var data = getData(stats[i])
		var opts = getChartOpts(stats[i])

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
}

function getAndRenderStats() {
	getJSON('/coordinator/statistics', renderStats, interval)
}

window.onload = function() {
	// install window timeout
	setTimeout(getAndRenderStats, interval);
};
