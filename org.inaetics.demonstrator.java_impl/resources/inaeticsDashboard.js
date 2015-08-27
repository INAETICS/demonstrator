/*! ineaticsDashboard.js - functions for rendering statistics - Copyright (C) 2015 - INAETICS */
"use strict";

const interval = 1000 // ms

function renderUtilisation(utilisation) {
	document.getElementById('utilisation').textContent = utilisation.name + " = " + utilisation.value + " " + utilisation.unit
}

function renderGraph(canvasId, stats) {
	var data = getData(stats)
	var opts = getChartOpts(stats)

	var statCanvas = document.getElementById(canvasId)
	var chartCtx = statCanvas.getContext('2d');

	if (statCanvas.hasAttribute("hasData")) {
		updateChart(chartCtx, data, opts, false /* animation */, false /* runanimationcompletefunction */);
	} else {
		var chart = new Chart(chartCtx);
		chart.Line(data, opts);
		// Mark the canvas as having data...
		statCanvas.setAttribute("hasData", "true")
	}
}

function renderUtilisationGraph(stats) {
	var queueIdx = stats[0].name == 'queue' ? 0 : 1

	renderGraph('queue-utilisation', stats[queueIdx])
	renderGraph('datastore-utilisation', stats[1 - queueIdx])
}

function getAndRenderUtilisation() {
	getJSON('/coordinator/utilisation', renderUtilisation, interval)
	getJSON('/coordinator/statistics?queue,data-store', renderUtilisationGraph, interval)
}

function renderSystemStats(stats) {
	var tbody = document.getElementById('sysStats')

	while (tbody.firstChild) {
		tbody.removeChild(tbody.firstChild);
	}

	for (var key in stats) {
		if (key === 'productionRate') {
			var val = stats[key]
			document.getElementById('slider-val').value = val
			document.getElementById('slider').value = val
		}
		if (stats.hasOwnProperty(key)) {
			var tr = tbody.insertRow(0);

			var th = document.createElement('th')
			th.textContent = key.capitalizeFirstLetter()
			var td = document.createElement('td')
			td.textContent = stats[key]

			tr.insertCell(0).appendChild(th)
			tr.insertCell(1).appendChild(td)
			tbody.appendChild(tr)
		}
	}
}

function getAndRenderSystemStats() {
	getJSON('/coordinator/systemStats', renderSystemStats, interval)
}

function postAndUpdateUtilisation(val) {
	document.getElementById('slider-val').value = val
	postJSON('/coordinator/utilisation', 'value=' + val)
}

function scaleProducer(val) {
	postJSON('/coordinator/scaleProducer', 'value=' + val)
}

window.onload = function() {
	setTimeout(getAndRenderUtilisation, interval)
	setTimeout(getAndRenderSystemStats, interval)
}
